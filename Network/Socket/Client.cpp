#include "Client.h"
#include "proto/punch.pb.h"
#include "ClientSocketMgr.h"
ClientSocket::ClientSocket(tcp::socket * socket)
	:_io(&socket->get_io_service()), Socket(std::move(socket)), _sendBufferSize(4096), _clientId(0)
{
	_remoteAddress = boost::asio::ip::address::from_string("127.0.0.1");
	_headerBuffer.Resize(sizeof(PacketHeader));
}

void ClientSocket::Start(const char* url, uint16_t port)
{
	//std::string ip_address = GetRemoteIpAddress().to_string();
	SetNoDelay(true);
	//_socket->non_blocking(true);
	_remoteAddress = boost::asio::ip::address::from_string(url);
	_remotePort = port;
	auto ed = boost::asio::ip::tcp::endpoint(_remoteAddress, port);
	_socket->async_connect(ed, boost::bind(&ClientSocket::AsyncConnectCallback, this, boost::asio::placeholders::error));
}
void ClientSocket::Start()
{
	std::cout << "client socket can't start with no endpoint" << std::endl;
}

void ClientSocket::Bind(const char * url, uint16_t port)
{
	auto ed = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(url), port);
	boost::system::error_code er;
	//_socket->set_option(boost::asio::socket_base::reuse_address(true));
	_socket->bind(ed, er);
	if (er)
	{
		std::cout << "socket fail to bind to endpoint " << ed << std::endl;
	}
}

void ClientSocket::ReadHandler()
{
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

bool ClientSocket::ReadHeaderHandler()
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

ReadDataHandlerResult ClientSocket::ReadDataHandler()
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());

	switch (header->Command)
	{
		//´ò¶´
	case 1:
	{
		SkyDream::Person remote;
		remote.ParseFromArray(_packetBuffer.GetReadPointer(), _packetBuffer.GetActiveSize());

		tcp::socket * sock = sClientSocketMgr->GetSocket();
		ClientSocket* client = new ClientSocket(sock);
		std::cout << "try connect to " << remote.ip() << ":" << remote.port() << std::endl;
		client->name = std::move("remote client");
		client->Start(remote.ip().c_str(), remote.port());
		sClientSocketMgr->OnSocketConnect(client);
		break;
	}
	//list
	case 2:
	{
		SkyDream::ListConn conn;
		conn.ParseFromArray(_packetBuffer.GetReadPointer(), _packetBuffer.GetActiveSize());
		for (auto p : conn.persons())
		{
			if (p.clientid() == _clientId)
				continue;
			std::cout << p.ip() << "\t" << p.port() << std::endl;
			char data[1024] = { 0 };
			size_t size = p.ByteSize();
			p.SerializePartialToArray(data, size);
			SendPacket(data, size, 1);
			break;
		}
		std::cout << "no other client connected to server " << std::endl;
		Sleep(3000);
		char data[] = { 0 };
		size_t size = 0;
		SendPacket(data, size, 2);
		break;
	}
	case 3:
	{
		SkyDream::IntValue iv;
		iv.ParseFromArray(_packetBuffer.GetReadPointer(), _packetBuffer.GetActiveSize());
		_clientId = iv.value();
		std::cout << "client id is " << _clientId << std::endl;
	}
	default:
	{
		break;
	}
	}
	_packetBuffer.Reset();
	return ReadDataHandlerResult::Ok;
}

bool ClientSocket::Update()
{
	if (!BaseSocket::Update())
		return false;
	return true;
}

void ClientSocket::SendPacket(const char * packet, size_t size, int cmd)
{
	PacketHeader header;
	header.Command = cmd;
	header.Size = size;
	MessageBuffer buffer(size + sizeof(header));
	buffer.Write(&header, sizeof(header));
	buffer.Write(packet, size);
	std::cout << "ClientSocket SendPacket cmd " << cmd << ",pack size " << buffer.GetActiveSize() << ",real size " << size + sizeof(header) << std::endl;
	QueuePacket(std::move(buffer));
}

void ClientSocket::AsyncConnectCallback(const boost::system::error_code & er)
{
	string connect_info = name+ "   connect ret :" + er.message() + "\n";
	std::cout << connect_info.c_str();
	if (!er)
	{
		//Start();
		//SkyDream::Person* person = new SkyDream::Person();
		//person->set_ip("221.10.7.226");
		//person->set_port(34089);
		char data[] = { 0 };
		size_t size = 0;// person->ByteSize();
		//person->SerializePartialToArray(data, size);
		AsyncRead();
		SendPacket(data, size, 2);
	}
	else
	{
		sClientSocketMgr->OnConnectFail(this, _remoteAddress, _remotePort);
	}
}
