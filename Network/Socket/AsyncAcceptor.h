#ifndef __ASYNCACCEPT_H_
#define __ASYNCACCEPT_H_
#include <boost/asio/ip/tcp.hpp>
#include <functional>
#include <atomic>
#include <iostream>
using namespace boost::asio::ip;
#if BOOST_VERSION >= 106600
#define TRINITY_MAX_LISTEN_CONNECTIONS boost::asio::socket_base::max_listen_connections
#else
#define TRINITY_MAX_LISTEN_CONNECTIONS boost::asio::socket_base::max_connections
#endif
class AsyncAcceptor
{
	typedef void(*AcceptCallback)(std::shared_ptr<tcp::socket> newSocket);
public:
	explicit AsyncAcceptor(boost::asio::io_service& service,const char* addr, int16_t port):
		_acceptor(service),_endpoint(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		_closed(false),_closing(false)
	{
	}
	void Stop() {
		_acceptor.close();
	}
	boost::asio::ip::address GetBindAddress()
	{
		return _endpoint.address();
	}
	uint16 GetBindPort()
	{
		return _endpoint.port();
	}
	bool Bind(bool reuse_addr = true)
	{
		boost::system::error_code errorCode;
		_acceptor.open(_endpoint.protocol(), errorCode);
		if (errorCode)
		{
			std::cout << "open" << errorCode.message().c_str() << std::endl;
			return false;
		}
		_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(reuse_addr));
		_acceptor.bind(_endpoint, errorCode);
		if (errorCode)
		{
			std::cout << "bind" << errorCode.message().c_str() << std::endl;
			return false;
		}
		_acceptor.listen(50, errorCode);
		if (errorCode)
		{
			std::cout << "listen" << errorCode.message().c_str() << std::endl;
			return false;
		}
		return true;
	}

	template<AcceptCallback acceptCallback>
	void AsyncAcceptWithCallback()
	{
		auto _socket = std::make_shared<tcp::socket>(_acceptor.get_io_service());
		_acceptor.async_accept(*_socket, [this,_socket](boost::system::error_code error)
		{
			if (!error)
			{
				try
				{
					_socket->non_blocking(true);

					acceptCallback(_socket);
				}
				catch (boost::system::system_error const& err)
				{
					std::cout << "Failed to retrieve client's remote address "<<err.what() << std::endl;
				}
			}
			else
			{
				std::cout << "async_accept address fail " << error << std::endl;
			}
			if (!_closed)
				this->AsyncAcceptWithCallback<acceptCallback>();
		});
	}
private:

	tcp::acceptor _acceptor;
	//tcp::socket* _socket;
	tcp::endpoint _endpoint;
	bool _closed;
	bool _closing;
};
#endif