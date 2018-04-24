#ifndef _COMMON_H 
#define _COMMON_H
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <iostream>
using namespace boost::asio::ip;
using namespace std;
#define READ_BLOCK_SIZE 4096
#ifdef BOOST_ASIO_HAS_IOCP
#define SD_SOCKET_USE_IOCP
#endif
struct PacketHeader
{
	uint32 Size;
	uint16 Command;

	bool IsValidSize() { return Size < 0x10000; }
	bool IsValidOpcode()
	{
		return Command < 1024;
	}
};
enum class ReadDataHandlerResult
{
	Ok = 0,
	Error = 1,
	WaitingForQuery = 2
};
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
