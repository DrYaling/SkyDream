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
	typedef void(*AcceptCallback)(tcp::socket* newSocket);
public:
	explicit AsyncAcceptor(tcp::socket* socket,const char* addr, int16_t port):
		_acceptor(socket->get_io_service()),_endpoint(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(addr), port)),
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
	//template<class T>
	void AsyncAccept();

	template<AcceptCallback acceptCallback>
	void AsyncAcceptWithCallback()
	{
		tcp::socket* _socket = new tcp::socket(_acceptor.get_io_service());
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
//template<class T>
void AsyncAcceptor::AsyncAccept()
{
	tcp::socket* _socket = new tcp::socket(_acceptor.get_io_service());
	_acceptor.async_accept(*_socket, [this, _socket](boost::system::error_code error)
	{
		if (!error)
		{
			try
			{
				_socket->non_blocking(true);
			}
			catch (boost::system::system_error const& err)
			{
				//std::string msg = "Failed to retrieve client's remote address " + err.what();
				std::cout << "Failed to retrieve client's remote address "<<err.what() << std::endl;
			}
		}

		// lets slap some more this-> on this so we can fix this bug with gcc 4.7.2 throwing internals in yo face
		if (!_closed)
			this->AsyncAccept();
	});
}
#endif