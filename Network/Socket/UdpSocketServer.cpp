#include "UdpSocketServer.h"
#include "proto/punch.pb.h"
#include "WorldSocketMgr.h"	
#include "Opcode/Opcode.h"
UdpSocketServer::UdpSocketServer(boost::asio::io_service& service) :
	_socket(std::make_shared<udp::socket>(service)),
	_readBuffer(),
	_punchWaitList(),
	_closed(false),
	_closing(false),
	_isWritingAsync(false),
	_sendLock(),
	_receiveLock(),
	_isConnected(false),
	name()
{
	_readBuffer.Resize(READ_BLOCK_SIZE);
}
UdpSocketServer:: ~UdpSocketServer()
{
	_closed = true;
	boost::system::error_code error;
	_socket->close(error);
}
//开始连接服务器或者客户端
void UdpSocketServer::Start(boost::asio::ip::address addr, uint16 port, int32 client)
{
	auto ed = udp::endpoint(addr, port + 302);
	_clientId = client;
	boost::asio::ip::udp::endpoint local_add(addr, port + 302);

	_socket->open(local_add.protocol());
	_socket->bind(local_add);
	//_socket->set_option(boost::asio::socket_base::reuse_address(true));
	AsyncRead();
}

bool UdpSocketServer::Update()
{
	if (_closed)
		return false;

#ifndef SD_SOCKET_USE_IOCP
	if (_isWritingAsync || (_writeQueue.empty() && !_closing))
		return true;

	for (; HandleQueue();)
		;
#endif

	return true;
}

boost::asio::ip::address UdpSocketServer::GetRemoteIpAddress() const
{
	return _remoteEndpoint.address();
}
boost::asio::ip::udp::endpoint UdpSocketServer::GetRemoteEndpoint()const
{
	return _remoteEndpoint;
}

uint16 UdpSocketServer::GetRemotePort() const
{
	return _remoteEndpoint.port();
}

void UdpSocketServer::AsyncRead()
{
	if (!IsOpen())
		return;

	_readBuffer.Normalize();
	_readBuffer.EnsureFreeSpace();
	_socket->async_receive_from(boost::asio::buffer(_readBuffer.GetWritePointer(), _readBuffer.GetRemainingSpace())\
		, _remoteEndpoint, boost::bind(&UdpSocketServer::ReadHandlerInternal, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
void UdpSocketServer::QueuePacket(MessageBuffer&& buffer)
{
	{
		std::lock_guard<std::mutex> lock_guard(_sendLock);
		_writeQueue.push(std::move(buffer));
	}

#ifdef SD_SOCKET_USE_IOCP
	AsyncProcessQueue();
#endif
}

bool UdpSocketServer::IsOpen() const { return !_closed && !_closing; }

void UdpSocketServer::CloseSocket()
{
	if (_closed.exchange(true))
		return;

	boost::system::error_code shutdownError;
	_socket->shutdown(boost::asio::socket_base::shutdown_send, shutdownError);
	if (shutdownError)
		std::cout << "shutdown socket fail " << shutdownError.message() << std::endl;// SD_LOG_DEBUG("network", "TCPSocket::CloseSocket: %s errored when shutting down socket: %i (%s)", GetRemoteIpAddress().to_string().c_str(),
																					 //	shutdownError.value(), shutdownError.message().c_str());

	OnClose();
}

/// Marks the socket for closing after write buffer becomes empty
void UdpSocketServer::DelayedCloseSocket() { _closing = true; }

MessageBuffer& UdpSocketServer::GetReadBuffer() { return _readBuffer; }

void UdpSocketServer::OnClose() { }

void UdpSocketServer::ReadHandler()
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
bool UdpSocketServer::ReadHeaderHandler()
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

ReadDataHandlerResult UdpSocketServer::ReadDataHandler()
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());
	std::cout << "UdpSocket read remote cmd " << header->Command << ",data size " << header->Size << std::endl;
	switch (header->Command)
	{
		//打洞
	case C2S_Opcode::C2S_PUNCH:
	{
		std::cout << "punch" << std::endl;
		SkyDream::IntValue psn;
		psn.ParseFromArray(_packetBuffer.GetReadPointer(), _packetBuffer.GetActiveSize());
		auto connections = sWorldSocketMgr->GetConnections();
		for (auto c : connections)
		{
			WorldSocket* w = dynamic_cast<WorldSocket*>(c);
			if (w)
			{
				//取到要打洞的对象连接
				if (w->GetClientId() == psn.value())
				{
					std::cout <<_clientId<< " connecting to " << psn.value() << std::endl;
					bool comfirmed = false;
					punch_content content;
					for (auto itr = _punchWaitList.begin(); itr != _punchWaitList.end(); itr++)
					{
						if (psn.value() == (*itr).from_ && (*itr).to_ == _clientId)
						{
							comfirmed = true;
							content = *itr;
							_punchWaitList.erase(itr);
							break;
						}
					}
					//两边都确认了，开始打洞
					if (comfirmed)
					{
						SkyDream::Person person;
						person.set_ip(w->GetRemoteIpAddress().to_string().c_str());
						person.set_port(w->GetRemotePort());
						person.set_clientid(w->GetClientId());
						char data[4096] = { 0 };
						size_t size = person.ByteSize();
						person.SerializePartialToArray(data, size);
						//给当前连接的客户端发送一个确认包
						SendPacket(data, size, S2C_Opcode::S2C_PUNCH_COMFIRM);
						person.set_ip(GetRemoteIpAddress().to_string().c_str());
						person.set_port(GetRemotePort());
						person.set_clientid(_clientId);
						size = person.ByteSize();
						person.SerializePartialToArray(data, size);
						//通过和另一个客户端的连接给另一个客户端发送一个连接包
						w->GetUdpSocket()->SendPacket(data, size, S2C_Opcode::S2C_PUNCH_COMFIRM);
					}
					//需要另一方确认
					else
					{
						punch_content pcc;
						pcc.from_ = _clientId;
						pcc.to_ = psn.value();
						_punchWaitList.push_back(pcc);
						SkyDream::IntValue val;
						val.set_value(_clientId);
						char buff[16] = { 0 };
						auto size = val.ByteSize();
						val.SerializeToArray(buff, size);
						w->SendPacket(buff, size, S2C_Opcode::S2C_PUNCH);
						std::cout << "等待客户端 " << w->GetClientId() << " 连接" << std::endl;
					}
					break;
				}
			}
		}
		break;
	}
	default:
	{
		break;
	}
	}

	return ReadDataHandlerResult::Ok;
}
void UdpSocketServer::SendPacket(const char*  packet, size_t size, int cmd)
{
	if (!IsOpen())
		return;

	PacketHeader header;
	header.Command = cmd;
	header.Size = size;
	MessageBuffer buffer(size + sizeof(header));
	buffer.Write(&header, sizeof(header));
	buffer.Write(packet, size);
	QueuePacket(std::move(buffer));
}


bool UdpSocketServer::AsyncProcessQueue()
{
	if (_isWritingAsync)
		return false;

	_isWritingAsync = true;

#ifdef SD_SOCKET_USE_IOCP
	MessageBuffer& buffer = _writeQueue.front();
	_socket->async_send_to(boost::asio::buffer(buffer.GetReadPointer(), buffer.GetActiveSize()), _remoteEndpoint, \
		boost::bind(&UdpSocketServer::WriteHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
#else
	_socket.async_write_some(boost::asio::null_buffers(), std::bind(&TCPSocket<T>::WriteHandlerWrapper,
		this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
#endif

	return false;
}
void UdpSocketServer::ReadHandlerInternal(boost::system::error_code error, size_t transferredBytes)
{
	std::cout << "read data " << error << "\t size :" << transferredBytes << std::endl;
	if (error)
	{
		CloseSocket();
		return;
	}

	_readBuffer.WriteCompleted(transferredBytes);
	ReadHandler();
}

#ifdef SD_SOCKET_USE_IOCP

void UdpSocketServer::WriteHandler(boost::system::error_code error, std::size_t transferedBytes)
{
	if (!error)
	{
		_isWritingAsync = false;
		std::cout << "write bytes " << transferedBytes << ",active size " << _writeQueue.front().GetActiveSize() << std::endl;
		_writeQueue.front().ReadCompleted(transferedBytes);
		if (!_writeQueue.front().GetActiveSize())
			_writeQueue.pop();
		std::cout << _writeQueue.empty() << std::endl;
		if (!_writeQueue.empty())
			AsyncProcessQueue();
		else if (_closing)
			CloseSocket();
	}
	else
		CloseSocket();
}

#else

void UdpSocketServer::WriteHandlerWrapper(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/)
{
	_isWritingAsync = false;
	HandleQueue();
}

bool UdpSocketServer::HandleQueue()
{
	if (_writeQueue.empty())
		return false;

	MessageBuffer& queuedMessage = _writeQueue.front();

	std::size_t bytesToSend = queuedMessage.GetActiveSize();

	boost::system::error_code error;
	std::size_t bytesSent = _socket.write_some(boost::asio::buffer(queuedMessage.GetReadPointer(), bytesToSend), error);

	if (error)
	{
		if (error == boost::asio::error::would_block || error == boost::asio::error::try_again)
			return AsyncProcessQueue();

		_writeQueue.pop();
		if (_closing && _writeQueue.empty())
			CloseSocket();
		return false;
	}
	else if (bytesSent == 0)
	{
		_writeQueue.pop();
		if (_closing && _writeQueue.empty())
			CloseSocket();
		return false;
	}
	else if (bytesSent < bytesToSend) // now n > 0
	{
		queuedMessage.ReadCompleted(bytesSent);
		return AsyncProcessQueue();
	}

	_writeQueue.pop();
	if (_closing && _writeQueue.empty())
		CloseSocket();
	return !_writeQueue.empty();
}

#endif