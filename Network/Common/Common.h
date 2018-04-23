#ifndef _COMMON_H 
#define _COMMON_H
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <iostream>
using namespace boost::asio::ip;
using namespace std;
#if __cplusplus
//extern "C"
//{
#endif
	static tcp::endpoint GetEndpoint(tcp::endpoint &local_endpoint,const char* url = "www.boost.org",const char* http = "http")
	{

		boost::asio::io_service contex;
		boost::asio::ip::tcp::resolver resolver(contex);
		boost::asio::ip::tcp::socket sock(contex);
		try 
		{
			std::cout << "try to connect remote url " << url << std::endl;
			boost::asio::ip::tcp::resolver::query query(url, http);
			boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
			boost::asio::ip::tcp::resolver::iterator end;
			if (iter == end)
			{
				std::cout << "error remote endpoint" << std::endl;
				return local_endpoint = tcp::endpoint();
			}
			auto connect = boost::asio::connect(sock, iter);
			if (connect != end)
			{
				local_endpoint = sock.local_endpoint();
				sock.close();
				return (*connect).endpoint();
			}
			else
			{
				sock.close();
				return local_endpoint = tcp::endpoint();
			}
		}
		catch(boost::system::system_error &e)
		{
			std::cout << e.what() << std::endl;
			return local_endpoint = tcp::endpoint();
		}
	}
#if __cplusplus
//}
#endif

#endif // 
