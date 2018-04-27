#include "UdpSocketClient.h"
#include "ClientSocketMgr.h"
#include <stdlib.h>
void UdpSocketClient::Bind(boost::asio::ip::address&& addr, uint16 port, int32 clientId)
{
	auto endpoint = udp::endpoint(addr, clientId +52400);
	std::cout << "udp client  endpoint " << endpoint << std::endl;
	_socket->open(endpoint.protocol());
	_socket->bind(endpoint);
	_clientId = clientId;
	_socket->set_option(boost::asio::socket_base::reuse_address(true));
	AsyncRead();
}
void UdpSocketClient::StartC2C(const char * addr, uint16_t port, int32 clientId)//开始做c-c连接
{
	auto endpoint = udp::endpoint(boost::asio::ip::address::from_string("118.113.200.77"), port);
	std::cout << "udp c-c ed " << endpoint << ",id " << clientId << std::endl;
	sClientSocketMgr->OnConnectRemote(clientId, endpoint);
	SkyDream::C2S_Punch punch;
	char buff[32] = { 0 };
	punch.set_from(_clientId);
	punch.set_to(clientId);
	int16 size = punch.ByteSize();
	punch.SerializePartialToArray(buff, size);
	SendPacket(buff, size, C2C_Opcode::C2C_HAND, clientId);
	_connectingMap.insert(std::pair<int32, bool>(clientId, false));
	std::thread t = std::thread(std::bind(&UdpSocketClient::AsyncWait, this, clientId));
	t.detach();
}
void UdpSocketClient::Punch(boost::asio::ip::address&& addr, uint16 port, int32 punch_client, uint16_t native_client)
{
	try {
		//-1是服务器
		sClientSocketMgr->OnConnectRemote(-1, udp::endpoint(addr, port + 302));
		SkyDream::C2S_Punch punch;
		punch.set_from(native_client);
		punch.set_to(punch_client);
		char buff[32] = { 0 };
		int16 size = punch.ByteSize();
		punch.SerializePartialToArray(buff, size);
		SendPacket(buff, size, C2S_Opcode::C2S_PUNCH, -1);
	}
	catch (boost::system::error_code &err)
	{
		std::cout << "exception  " << err.message() << std::endl;
	}
}
ReadDataHandlerResult UdpSocketClient::ReadDataHandler()
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());
	std::cout << "UdpSocketClient read remote cmd " << header->Command << ",data size " << header->Size << std::endl;
	switch (header->Command)
	{
	case S2C_Opcode::S2C_PUNCH_COMFIRM:
	{
		SkyDream::Person remote;
		remote.ParseFromArray(_packetBuffer.GetReadPointer(), _packetBuffer.GetActiveSize());
		//Start()
		StartC2C(remote.ip().c_str(), remote.port(), remote.clientid());
		break;
	}
	case C2C_Opcode::C2C_HAND:
	{
		SkyDream::C2S_Punch punch;
		punch.ParseFromArray(_packetBuffer.GetReadPointer(), _packetBuffer.GetActiveSize());
		if (punch.to() != _clientId)
		{
			std::cout << "recv hand from " << punch.from() << "fail,target remote is not me!" << std::endl;
			return ReadDataHandlerResult::Error;
		}
		udp::endpoint from_ed;
		sClientSocketMgr->GetEndPointOf(punch.from(),from_ed);
		if (from_ed != _receiveEndpoint)
		{
			std::cout << "recv hand from " << punch.from() << "fail,target endpoint is not same as cached!"<<from_ed <<","<< _receiveEndpoint << std::endl;
			return ReadDataHandlerResult::Error;
		}

		auto itr = _connectingMap.find(punch.from());
		if (itr != _connectingMap.end())
		{
			_connectingMap.erase(itr);
		}
		std::cout << "receive hand pack" << std::endl;
		break;
	}
	case C2C_Opcode::C2C_PING:
	{
		break;
	}
	case C2C_Opcode::C2C_MSG:
	{
		break;
	}
	default:
	{
		break;
	}
	}

	return ReadDataHandlerResult::Ok;
}

bool UdpSocketClient::AsyncProcessQueue()
{
	if (_isWritingAsync)
		return false;

	_isWritingAsync = true;

#ifdef SD_SOCKET_USE_IOCP
	MessageBuffer& buffer = _writeQueue.front().buffer;
	udp::endpoint ed;
	if (!sClientSocketMgr->GetEndPointOf(_writeQueue.front().clientId, ed))
	{
		std::cout << "client does not exist connection of " << _writeQueue.front().clientId << std::endl;
		WriteHandler(boost::system::error_code(), _writeQueue.front().buffer.GetActiveSize());
		return true;
	}
	std::cout << "client udp send data to " << ed << " ,size:" << buffer.GetActiveSize() << std::endl;
	_socket->async_send_to(boost::asio::buffer(buffer.GetReadPointer(), buffer.GetActiveSize()), ed, \
		boost::bind(&UdpSocketClient::WriteHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
#else
	_socket.async_write_some(boost::asio::null_buffers(), std::bind(&TCPSocket<T>::WriteHandlerWrapper,
		this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
#endif

	return false;
}

void UdpSocketClient::ReadHandlerInternal(boost::system::error_code error, size_t transferredBytes)

{
	std::cout << " udp client read data " << error.message().c_str() << "\t size :" << transferredBytes << " from " << _receiveEndpoint << std::endl;
	if (error)
	{
		int32 clientId = -202;
		if (!sClientSocketMgr->GetClientIdOf(_receiveEndpoint, clientId))
		{
			std::cout << "error 202" << std::endl;
			return;
		}
		StartC2C(_receiveEndpoint.address().to_string().c_str(), _receiveEndpoint.port(), clientId);
		AsyncRead();
		return;
	}

	_readBuffer.WriteCompleted(transferredBytes);
	ReadHandler();
}