
#include "WorldSocket.h"
#include <memory>

using boost::asio::ip::tcp;

WorldSocket::WorldSocket(tcp::socket* socket)
	: Socket(socket), _authSeed(0), _OverSpeedPings(0), _session(nullptr), _authed(false), _sendBufferSize(4096)
{
	//_headerBuffer.Resize(sizeof(ClientPktHeader));
}

void WorldSocket::Start()
{
	_remoteAddress = _socket->remote_endpoint().address();
	_remotePort = _socket->remote_endpoint().port();
	std::string ip_address = GetRemoteIpAddress().to_string();
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
	//ASSERT(_headerBuffer.GetActiveSize() == sizeof(ClientPktHeader));

	//_authCrypt.DecryptRecv(_headerBuffer.GetReadPointer(), sizeof(ClientPktHeader));


	PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());
	header->Size -= 2;

	if (!header->IsValidSize() || !header->IsValidOpcode())
	{
		//TC_LOG_ERROR("network", "WorldSocket::ReadHeaderHandler(): client %s sent malformed packet (size: %u, cmd: %u)",
		//	GetRemoteIpAddress().to_string().c_str(), header->Size, header->Command);
		return false;
	}

	_packetBuffer.Resize(header->Size);
	return true;
}


WorldSocket::ReadDataHandlerResult WorldSocket::ReadDataHandler()
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());
	//OpcodeClient opcode = static_cast<OpcodeClient>(header->Command);

	//WorldPacket packet(opcode, std::move(_packetBuffer), GetConnectionType());

	/*if (sPacketLog->CanLogPacket())
		sPacketLog->LogPacket(packet, CLIENT_TO_SERVER, GetRemoteIpAddress(), GetRemotePort(), GetConnectionType());
*/

	std::unique_lock<std::mutex> sessionGuard(_worldSessionLock, std::defer_lock);

	switch (0)
	{
	/*case CMSG_PING:
	{
		LogOpcodeText(opcode, sessionGuard);
		WorldPackets::Auth::Ping ping(std::move(packet));
		if (!ping.ReadNoThrow())
		{
			TC_LOG_ERROR("network", "WorldSocket::ReadDataHandler(): client %s sent malformed CMSG_PING", GetRemoteIpAddress().to_string().c_str());
			return ReadDataHandlerResult::Error;
		}
		if (!HandlePing(ping))
			return ReadDataHandlerResult::Error;
		break;
	}
	case CMSG_AUTH_SESSION:
	{
		LogOpcodeText(opcode, sessionGuard);
		if (_authed)
		{
			// locking just to safely log offending user is probably overkill but we are disconnecting him anyway
			if (sessionGuard.try_lock())
				TC_LOG_ERROR("network", "WorldSocket::ProcessIncoming: received duplicate CMSG_AUTH_SESSION from %s", _worldSession->GetPlayerInfo().c_str());
			return ReadDataHandlerResult::Error;
		}

		std::shared_ptr<WorldPackets::Auth::AuthSession> authSession = std::make_shared<WorldPackets::Auth::AuthSession>(std::move(packet));
		if (!authSession->ReadNoThrow())
		{
			TC_LOG_ERROR("network", "WorldSocket::ReadDataHandler(): client %s sent malformed CMSG_AUTH_SESSION", GetRemoteIpAddress().to_string().c_str());
			return ReadDataHandlerResult::Error;
		}
		HandleAuthSession(authSession);
		return ReadDataHandlerResult::WaitingForQuery;
	}
	case CMSG_AUTH_CONTINUED_SESSION:
	{
		LogOpcodeText(opcode, sessionGuard);
		if (_authed)
		{
			// locking just to safely log offending user is probably overkill but we are disconnecting him anyway
			if (sessionGuard.try_lock())
				TC_LOG_ERROR("network", "WorldSocket::ProcessIncoming: received duplicate CMSG_AUTH_CONTINUED_SESSION from %s", _worldSession->GetPlayerInfo().c_str());
			return ReadDataHandlerResult::Error;
		}

		std::shared_ptr<WorldPackets::Auth::AuthContinuedSession> authSession = std::make_shared<WorldPackets::Auth::AuthContinuedSession>(std::move(packet));
		if (!authSession->ReadNoThrow())
		{
			TC_LOG_ERROR("network", "WorldSocket::ReadDataHandler(): client %s sent malformed CMSG_AUTH_CONTINUED_SESSION", GetRemoteIpAddress().to_string().c_str());
			return ReadDataHandlerResult::Error;
		}
		HandleAuthContinuedSession(authSession);
		return ReadDataHandlerResult::WaitingForQuery;
	}
	case CMSG_KEEP_ALIVE:
		LogOpcodeText(opcode, sessionGuard);
		break;
	case CMSG_LOG_DISCONNECT:
		LogOpcodeText(opcode, sessionGuard);
		packet.rfinish();   // contains uint32 disconnectReason;
		break;
	case CMSG_ENABLE_NAGLE:
		LogOpcodeText(opcode, sessionGuard);
		SetNoDelay(false);
		break;
	case CMSG_CONNECT_TO_FAILED:
	{
		sessionGuard.lock();

		LogOpcodeText(opcode, sessionGuard);
		WorldPackets::Auth::ConnectToFailed connectToFailed(std::move(packet));
		if (!connectToFailed.ReadNoThrow())
		{
			TC_LOG_ERROR("network", "WorldSocket::ReadDataHandler(): client %s sent malformed CMSG_CONNECT_TO_FAILED", GetRemoteIpAddress().to_string().c_str());
			return ReadDataHandlerResult::Error;
		}
		HandleConnectToFailed(connectToFailed);
		break;
	}
	case CMSG_ENABLE_ENCRYPTION_ACK:
		LogOpcodeText(opcode, sessionGuard);
		HandleEnableEncryptionAck();
		break;*/
	default:
	{
		sessionGuard.lock();

		//LogOpcodeText(opcode, sessionGuard);

		if (!_session)
		{
			//TC_LOG_ERROR("network.opcode", "ProcessIncoming: Client not authed opcode = %u", uint32(opcode));
			return ReadDataHandlerResult::Error;
		}

	//	OpcodeHandler const* handler = opcodeTable[opcode];
	//	if (!handler)
		{
	//		TC_LOG_ERROR("network.opcode", "No defined handler for opcode %s sent by %s", GetOpcodeNameForLogging(static_cast<OpcodeClient>(packet.GetOpcode())).c_str(), _worldSession->GetPlayerInfo().c_str());
	//		break;
		}

		// Our Idle timer will reset on any non PING opcodes.
		// Catches people idling on the login screen and any lingering ingame connections.
		_session->ResetTimeOutTime();

		// Copy the packet to the heap before enqueuing
		_session->QueuePacket(nullptr);
		break;
	}
	}

	return ReadDataHandlerResult::Ok;
}

void WorldSocket::SendPacket(const char* packet, size_t size)
{
	if (!IsOpen())
		return;

	/*if (sPacketLog->CanLogPacket())
		sPacketLog->LogPacket(packet, SERVER_TO_CLIENT, GetRemoteIpAddress(), GetRemotePort());

	_bufferQueue.Enqueue(new EncryptablePacket(packet, _authCrypt.IsInitialized()));*/
}
