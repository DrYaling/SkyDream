#include "Client.h"

ClientSocket::ClientSocket(tcp::socket * socket)
	:_io(&socket->get_io_service()), Socket(std::move(socket)), _sendBufferSize(4096)
{
	_remoteAddress = boost::asio::ip::address::from_string("127.0.0.1");
}

void ClientSocket::Start(const char* url,uint16_t port)
{
	//std::string ip_address = GetRemoteIpAddress().to_string();
	SetNoDelay(true);
	//_socket->non_blocking(true);
	auto ed = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(url), port);
	_socket->async_connect(ed, boost::bind(&ClientSocket::AsyncConnectCallback, this, boost::asio::placeholders::error));
}
void ClientSocket::Start()
{
	//std::string ip_address = GetRemoteIpAddress().to_string();
	SetNoDelay(true);
	//_socket->non_blocking(true);
	auto ed = boost::asio::ip::tcp::endpoint(_remoteAddress, 8080);
	_socket->async_connect(ed, boost::bind(&ClientSocket::AsyncConnectCallback, this, boost::asio::placeholders::error));
}

bool ClientSocket::Update()
{
	if (!BaseSocket::Update())
		return false;
	return true;
}

void ClientSocket::SendPacket(const char * packet, size_t size)
{
}

void ClientSocket::AsyncConnectCallback(const boost::system::error_code & er)
{
	string connect_info = "connect ret :" + er.message() + "\n";
	std::cout << connect_info.c_str();
}
