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
public:
	explicit AsyncAcceptor(tcp::socket* socket,const char* addr, int16_t port):
		_socket(socket),_acceptor(socket->get_io_service()),_endpoint(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1"), 8080)),
		_closed(false),_closing(false)
	{

	}
	bool Start()
	{
		boost::system::error_code errorCode;
		_acceptor.open(_endpoint.protocol(), errorCode);
		if (errorCode)
		{
			std::cout << "open" << errorCode.message().c_str() << std::endl;
			return false;
		}
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
	template<class T>
	void AsyncAccept();

	template<AcceptCallback acceptCallback>
	void AsyncAcceptWithCallback()
	{
		_acceptor.async_accept(*_socket, [this](boost::system::error_code error)
		{
			if (!error)
			{
				try
				{
					_socket->non_blocking(true);

					acceptCallback(std::move(*_socket));
				}
				catch (boost::system::system_error const& err)
				{
					TC_LOG_INFO("network", "Failed to initialize client's socket %s", err.what());
				}
			}

			if (!_closed)
				this->AsyncAcceptWithCallback<acceptCallback>();
		});
	}
private:

	tcp::acceptor _acceptor;
	tcp::socket* _socket;
	tcp::endpoint _endpoint;
	bool _closed;
	bool _closing;
};
template<class T>
void AsyncAcceptor::AsyncAccept()
{
	_acceptor.async_accept(*_socket, [this](boost::system::error_code error)
	{
		if (!error)
		{
			try
			{
				_socket->non_blocking(true);
			}
			catch (boost::system::system_error const& err)
			{
				std::string msg = "Failed to retrieve client's remote address " + err.what();
				std::cout << msg << std::endl;
			}
		}

		// lets slap some more this-> on this so we can fix this bug with gcc 4.7.2 throwing internals in yo face
		if (!_closed)
			this->AsyncAccept<T>();
	});
}
#endif