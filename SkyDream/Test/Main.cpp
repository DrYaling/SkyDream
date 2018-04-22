#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <proto/test.pb.h>
#include <Socket/WorldSocket.h>
#include <Asio/IoContext.h>
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
int main()
{
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
	boost::asio::async_connect(sock, iter, end, connect_callback);
	contex.run();
	while (1) Sleep(1);
}