/*
created by zxb @2018-4-19
*/

#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__

#include "../Common/MessageBuffer.h"
#include "Common/Common.h"
#include <atomic>
#include <queue>
#include <memory>
#include <functional>
#include <type_traits>
#include <boost/asio/ip/tcp.hpp>
#include <mutex>
#include <boost/bind.hpp>
#include <iostream>
using boost::asio::ip::tcp;

template<class T>
class TCPSocket : public std::enable_shared_from_this<T>
{

public:
	std::string name;
	explicit TCPSocket(std::shared_ptr<tcp::socket>&& socket) :
		_socket(socket),
		_readBuffer(),
		_closed(false),
		_closing(false),
		_isWritingAsync(false),
		_sendLock(),
		_receiveLock(),
		name()
	{
		_readBuffer.Resize(READ_BLOCK_SIZE);
	}
	virtual ~TCPSocket()
	{
		_closed = true;
		//if (nullptr != _socket)
		//	_socket->close(error);
	}

	virtual void Start() = 0;

	virtual bool Update()
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

	boost::asio::ip::address GetRemoteIpAddress() const
	{
		return _remoteEndpoint;
	}

	uint16 GetRemotePort() const
	{
		return _remotePort;
	}

	int32 GetClientId() const { return _clientId; }
	void AsyncRead()
	{
		if (!IsOpen())
			return;

		_readBuffer.Normalize();
		_readBuffer.EnsureFreeSpace();
		_socket->async_read_some(boost::asio::buffer(_readBuffer.GetWritePointer(), _readBuffer.GetRemainingSpace()),
			std::bind(&TCPSocket<T>::ReadHandlerInternal, this, std::placeholders::_1, std::placeholders::_2));
	}

	void AsyncReadWithCallback(void (T::*callback)(boost::system::error_code, std::size_t))
	{
		if (!IsOpen())
			return;

		_readBuffer.Normalize();
		_readBuffer.EnsureFreeSpace();
		_socket->async_read_some(boost::asio::buffer(_readBuffer.GetWritePointer(), _readBuffer.GetRemainingSpace()),
			std::bind(callback, this, std::placeholders::_1, std::placeholders::_2));
	}

	void QueuePacket(MessageBuffer&& buffer)
	{
		{
			std::lock_guard<std::mutex> lock_guard(_sendLock);
			_writeQueue.push(std::move(buffer));
		}

#ifdef SD_SOCKET_USE_IOCP
		AsyncProcessQueue();
#endif
	}

	bool IsOpen() const { return !_closed && !_closing; }

	void CloseSocket()
	{
		if (_closed.exchange(true))
			return;

		boost::system::error_code shutdownError;
		_socket->shutdown(boost::asio::socket_base::shutdown_send, shutdownError);
		if (shutdownError)
			std::cout << "shutdown socket fail " << shutdownError.message() << std::endl;// SD_LOG_DEBUG("network", "TCPSocket::CloseSocket: %s errored when shutting down socket: %i (%s)", GetRemoteIpAddress().to_string().c_str(),
			//	shutdownError.value(), shutdownError.message().c_str());

		boost::system::error_code error;
		_socket->close(error);
		if (error)
			std::cout << "close error " << error.message() << std::endl;
		OnClose();
	}

	/// Marks the socket for closing after write buffer becomes empty
	void DelayedCloseSocket() { _closing = true; }

	MessageBuffer& GetReadBuffer() { return _readBuffer; }

protected:
	virtual void OnClose() { }

	virtual void ReadHandler() = 0;
	bool AsyncProcessQueue()
	{
		if (_isWritingAsync)
			return false;

		_isWritingAsync = true;

#ifdef SD_SOCKET_USE_IOCP
		MessageBuffer& buffer = _writeQueue.front();
		_socket->async_write_some(boost::asio::buffer(buffer.GetReadPointer(), buffer.GetActiveSize()), std::bind(&TCPSocket<T>::WriteHandler,
			this, std::placeholders::_1, std::placeholders::_2));
#else
		_socket.async_write_some(boost::asio::null_buffers(), std::bind(&TCPSocket<T>::WriteHandlerWrapper,
			this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
#endif

		return false;
	}

	void SetNoDelay(bool enable)
	{
		boost::system::error_code err;
		_socket->set_option(tcp::no_delay(enable), err);
		//if (err)
		//	SD_LOG_DEBUG("network", "TCPSocket::SetNoDelay: failed to set_option(boost::asio::ip::tcp::no_delay) for %s - %d (%s)",
		//		GetRemoteIpAddress().to_string().c_str(), err.value(), err.message().c_str());
	}
	boost::asio::ip::address _remoteEndpoint;
	uint16 _remotePort;
	std::shared_ptr<tcp::socket> _socket;
	std::mutex _sendLock;
	std::mutex _receiveLock;


private:
	void ReadHandlerInternal(boost::system::error_code error, size_t transferredBytes)
	{
		std::cout << name << "  read data " << error << "\t size :" << transferredBytes << std::endl;
		if (error)
		{
			CloseSocket();
			return;
		}

		_readBuffer.WriteCompleted(transferredBytes);
		ReadHandler();
	}

#ifdef SD_SOCKET_USE_IOCP

	void WriteHandler(boost::system::error_code error, std::size_t transferedBytes)
	{
		if (!error)
		{
			_isWritingAsync = false;
			std::cout << "write bytes " << transferedBytes << ",active size " << _writeQueue.front().GetActiveSize() << " to " << _clientId << std::endl;
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

	void WriteHandlerWrapper(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/)
	{
		_isWritingAsync = false;
		HandleQueue();
	}

	bool HandleQueue()
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
	MessageBuffer _readBuffer;
	std::queue<MessageBuffer> _writeQueue;

	bool _isWritingAsync;
protected:
	int32 _clientId;
	std::atomic<bool> _closed;
	std::atomic<bool> _closing;
};

#endif // __SOCKET_H__
