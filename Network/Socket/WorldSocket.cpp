
#include "WorldSocket.h"
#include <memory>
#include "proto/punch.pb.h"
#include "WorldSocketMgr.h"
#include "Opcode/Opcode.h"
using boost::asio::ip::tcp;

WorldSocket::WorldSocket(tcp::socket* socket)
	: TCPSocket(socket), _OverSpeedPings(0), _session(nullptr), _authed(false), _sendBufferSize(4096)
{
	_headerBuffer.Resize(sizeof(PacketHeader));
}
static int clientId = 0;
static std::mutex _clientIdMutex;
void WorldSocket::Start()
{
	_remoteEndpoint = _socket->remote_endpoint().address();
	_remotePort = _socket->remote_endpoint().port();
	std::string ip_address = GetRemoteIpAddress().to_string();
	//发送客户端Id包
	char data[50] = { 0 };
	{
		std::lock_guard<std::mutex> g(_clientIdMutex);
		_clientId = clientId++;
	}
	SkyDream::IntValue iv;
	iv.set_value(_clientId);
	auto size = iv.ByteSize();
	iv.SerializeToArray(data, size);
	SendPacket(data, size, S2C_Opcode::S2C_CLIENT_ID);
	_udpSocket = new UdpSocketServer(_socket->get_io_service());
	_udpSocket->Start(_socket->local_endpoint().address(), _socket->local_endpoint().port(), _clientId);
	AsyncRead();
	//_queryProcessor.AddQuery(LoginDatabase.AsyncQuery(stmt).WithPreparedCallback(std::bind(&WorldSocket::CheckIpCallback, this, std::placeholders::_1)));
}

bool WorldSocket::Update()
{
	EncryptablePacket* queued;
	MessageBuffer buffer(_sendBufferSize);
	while (_bufferQueue.Dequeue(queued))
	{
		uint32 packetSize = queued->size;
		if (buffer.GetRemainingSpace() < packetSize + SizeOfServerHeader)
		{
			QueuePacket(std::move(buffer));
			buffer.Resize(_sendBufferSize);
		}
		if (buffer.GetRemainingSpace() >= packetSize + SizeOfServerHeader)
			WritePacketToBuffer(*queued, buffer);
		else    // single packet larger than 4096 bytes
		{
			MessageBuffer packetBuffer(packetSize + SizeOfServerHeader);
			WritePacketToBuffer(*queued, packetBuffer);
			QueuePacket(std::move(packetBuffer));
		}

		delete queued;
	}

	if (buffer.GetActiveSize() > 0)
		QueuePacket(std::move(buffer));

	if (!BaseSocket::Update())
		return false;

	//_queryProcessor.ProcessReadyQueries();

	return true;
}


void WorldSocket::OnClose()
{
	{
		std::lock_guard<std::mutex> sessionGuard(_worldSessionLock);
		_session = nullptr;
	}
}
void WorldSocket::WritePacketToBuffer(EncryptablePacket const& packet, MessageBuffer& buffer)
{
	uint32 opcode = packet.opcode;
	uint32 packetSize = packet.size;

	// Reserve space for buffer
	uint8* headerPos = buffer.GetWritePointer();
	buffer.WriteCompleted(SizeOfServerHeader);

	/*if (packetSize > MinSizeForCompression && packet.NeedsEncryption())
	{
		CompressedWorldPacket cmp;
		cmp.UncompressedSize = packetSize + 2;
		cmp.UncompressedAdler = adler32(adler32(0x9827D8F1, (Bytef*)&opcode, 2), packet.contents(), packetSize);

		// Reserve space for compression info - uncompressed size and checksums
		uint8* compressionInfo = buffer.GetWritePointer();
		buffer.WriteCompleted(sizeof(CompressedWorldPacket));

		uint32 compressedSize = CompressPacket(buffer.GetWritePointer(), packet);

		cmp.CompressedAdler = adler32(0x9827D8F1, buffer.GetWritePointer(), compressedSize);

		memcpy(compressionInfo, &cmp, sizeof(CompressedWorldPacket));
		buffer.WriteCompleted(compressedSize);
		packetSize = compressedSize + sizeof(CompressedWorldPacket);

		opcode = SMSG_COMPRESSED_PACKET;
	}*/
	if (packet.size > 0)
		buffer.Write(packet.buff, packet.size);


	packetSize += 2 /*opcode*/;

	PacketHeader header;
	header.Size = packetSize;
	header.Command = opcode;

	memcpy(headerPos, &header, SizeOfServerHeader);
}
void WorldSocket::ReadHandler()
{
	if (!IsOpen())
		return;

	MessageBuffer& packet = GetReadBuffer();
	while (packet.GetActiveSize() > 0)
	{
		if (_headerBuffer.GetRemainingSpace() > 0)
		{
			// need to receive the header
			std::size_t readHeaderSize = std::min(packet.GetActiveSize(), _headerBuffer.GetRemainingSpace());
			_headerBuffer.Write(packet.GetReadPointer(), readHeaderSize);
			packet.ReadCompleted(readHeaderSize);

			if (_headerBuffer.GetRemainingSpace() > 0)
			{
				// Couldn't receive the whole header this time.
				//ASSERT(packet.GetActiveSize() == 0);
				break;
			}

			// We just received nice new header
			if (!ReadHeaderHandler())
			{
				CloseSocket();
				return;
			}
		}

		// We have full read header, now check the data payload
		if (_packetBuffer.GetRemainingSpace() > 0)
		{
			// need more data in the payload
			std::size_t readDataSize = std::min(packet.GetActiveSize(), _packetBuffer.GetRemainingSpace());
			_packetBuffer.Write(packet.GetReadPointer(), readDataSize);
			packet.ReadCompleted(readDataSize);

			if (_packetBuffer.GetRemainingSpace() > 0)
			{
				// Couldn't receive the whole data this time.
				//ASSERT(packet.GetActiveSize() == 0);
				break;
			}
		}

		// just received fresh new payload
		ReadDataHandlerResult result = ReadDataHandler();
		_headerBuffer.Reset();
		if (result != ReadDataHandlerResult::Ok)
		{
			if (result != ReadDataHandlerResult::WaitingForQuery)
				CloseSocket();

			return;
		}
	}

	AsyncRead();
}

bool WorldSocket::ReadHeaderHandler()
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());

	if (!header->IsValidSize() || !header->IsValidOpcode())
	{
		//TC_LOG_ERROR("network", "WorldSocket::ReadHeaderHandler(): client %s sent malformed packet (size: %u, cmd: %u)",
		//	GetRemoteIpAddress().to_string().c_str(), header->Size, header->Command);
		return false;
	}

	_packetBuffer.Resize(header->Size);
	return true;
}


ReadDataHandlerResult WorldSocket::ReadDataHandler()
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());
	std::unique_lock<std::mutex> sessionGuard(_worldSessionLock, std::defer_lock);
	std::cout << "read remote cmd " << header->Command << ",data size " << header->Size << std::endl;
	switch (header->Command)
	{
		//list
	case C2S_Opcode::C2S_CLIST:
	{
		auto connections = sWorldSocketMgr->GetConnections();
		SkyDream::ListConn conn;
		for (auto c : connections)
		{
			WorldSocket* w = dynamic_cast<WorldSocket*>(c);
			if (!w)
				continue;
			SkyDream::Person* person = conn.add_persons();
			person->set_ip(w->GetRemoteIpAddress().to_string());
			person->set_port(w->GetRemotePort());
			person->set_clientid(w->GetClientId());
			std::cout << "connect client " << person->clientid() << std::endl;
		}
		char data[4096] = { 0 };
		size_t size = conn.ByteSize();
		conn.SerializePartialToArray(data, size);
		SendPacket(data, size, S2C_Opcode::S2C_CLIST);		
		break;
	}
	default:
	{
		break;
	}
	}

	return ReadDataHandlerResult::Ok;
}

void WorldSocket::SendPacket(const char* packet, size_t size, int cmd)
{
	if (!IsOpen())
		return;

	PacketHeader header;
	header.Command = cmd;
	header.Size = size;
	MessageBuffer buffer(size + sizeof(header));
	buffer.Write(&header, sizeof(header));
	buffer.Write(packet, size);
	//std::cout << "WorldSocket SendPacket cmd " << cmd << ",pack size " << buffer.GetActiveSize() << ",real size " << size + sizeof(header) << std::endl;
	QueuePacket(std::move(buffer));
	/*if (sPacketLog->CanLogPacket())
		sPacketLog->LogPacket(packet, SERVER_TO_CLIENT, GetRemoteIpAddress(), GetRemotePort());

	_bufferQueue.Enqueue(new EncryptablePacket(packet, _authCrypt.IsInitialized()));*/
}
