#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <proto/test.pb.h>
#include <Socket/WorldSocket.h>
#include <Asio/IoContext.h>
#include "Common//Common.h"
#include "Socket/WorldSocket.h"
#include "Socket/Client.h"
SkyDream::Asio::IoContext contex;
boost::asio::ip::tcp::resolver resolver(contex);
boost::asio::ip::tcp::socket sock(contex);
void connect_callback(const  boost::system::error_code& error,
	boost::asio::ip::tcp::resolver::iterator i)
{
	std::cout << "connect callback error code " << error << i->endpoint() << std::endl;
	if (!error)
	{
	}
	std::cout << "socket end point " << sock.local_endpoint() << "\t" << sock.remote_endpoint() << std::endl;
}
bool InitAccepter()
{
	auto sock = sSocketMgr->GetSocket();
	auto pSock = sock;
	auto addr = boost::asio::ip::address_v4::from_string("127.0.0.1");
	auto end_p = boost::asio::ip::tcp::endpoint(addr, 8080);
	tcp::acceptor _acceptor(pSock->get_io_service());
	boost::system::error_code errorCode;
	_acceptor.open(boost::asio::ip::tcp::endpoint(addr, 8080).protocol(), errorCode);
	if (errorCode)
	{
		std::cout << "open" << errorCode.message().c_str() << std::endl;
		return false;
	}
	_acceptor.bind(end_p, errorCode);
	if (errorCode)
	{
		std::cout << "bind" << errorCode.message().c_str() << std::endl;
		return false;
	}
	_acceptor.listen(50, errorCode);
	if (errorCode)
	{
		std::cout<<"listen" << errorCode.message().c_str() << std::endl;
		return false;
	}
	auto func = std::bind([&]() 
	{
		_acceptor.async_accept(*pSock, [pSock](boost::system::error_code error)
		{
			if (!error)
			{
				try
				{
					pSock->non_blocking(true);
					std::cout << "accept from " << pSock->remote_endpoint() << std::endl;
				}
				catch (boost::system::system_error const& err)
				{
					std::cout<<"accept fail  " << err.what() << std::endl;
				}
			}
			else
				std::cout<< "async_accept fail :"<< error.message().c_str() << std::endl;
		});
	});
	func();
	while (true) Sleep(1);
	return true;
}
void testSockets()
{
	boost::system::error_code er;
	std::thread t = std::thread(InitAccepter);
	t.detach();
	auto client = ClientSocket::Create();
	Sleep(500);
	client->Start();
	//WorldSocket* server = new WorldSocket(std::move(*pSock));

}
int main()
{
	auto thread = std::thread(testSockets);
	thread.detach();
	while (true) Sleep(1);
	return 0;
	tutorial::Person* person = new tutorial::Person();
	person->set_age(23);
	person->set_name("nimamaipi");
	std::cout << person->age() << "\t" << person->name().c_str() << std::endl;
	char buff[250] = { 0 };
	int	size;
	if (person->SerializePartialToArray(buff, person->ByteSize()))
	{
		auto newPs = new tutorial::Person();
		newPs->ParseFromArray(buff, person->ByteSize());
		std::cout << buff << "\t" << sizeof(buff) << "\t" << person->ByteSize() << "\t" << newPs->age() << "\t" << newPs->name() << std::endl;
	}
	else
	{
		std::cout << "serialize fail!" << std::endl;
	}
	boost::asio::ip::tcp::resolver::query query("www.boost.org", "http");
	//resolver.async_resolve(query, resolve_handler);
	boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
	boost::asio::ip::tcp::resolver::iterator end; // End marker.
	boost::system::errc::errc_t er;
	//while (iter != end)
	{
		//	boost::asio::ip::tcp::endpoint endpoint = *iter++;
		//	std::cout << endpoint << std::endl;
	}
	//WorldSocket* socket = new WorldSocket(std::move(*sock));
	//boost::asio::async_connect(sock, iter, end, connect_callback);
	//auto connect = boost::asio::connect(sock, iter);
	boost::asio::ip::tcp::endpoint ed;
	//GetEndpoint(ed,"www.baidu.com");
	//std::cout<< GetEndpoint(ed) << std::endl << ed << std::endl;
	//contex.run();
	while (1) Sleep(1);
}