/*
created by zxb @2018-4-19
*/

#ifndef __UDP_SOCKET_CLIENT_H__
#define __UDP_SOCKET_CLIENT_H__

#include "../Common/MessageBuffer.h"
#include "Common/Common.h"
#include <atomic>
#include <queue>
#include <memory>
#include <functional>
#include <type_traits>
#include <boost/asio/ip/udp.hpp>
#include <mutex>
#include <boost/bind.hpp>
#include <iostream>
#include "proto/punch.pb.h"
#include "Opcode/Opcode.h"
using boost::asio::ip::udp;
class UdpSocketClient
{

public:
	std::string name;
	explicit UdpSocketClient(boost::asio::io_service& service) :
		_socket(std::make_shared<udp::socket>(service)),
		_readBuffer(),
		_punchWaitList(),
		_closed(false),
		_closing(false),
		_isWritingAsync(false),
		_sendLock(),
		_receiveLock(),
		_handTryCount(6),
		_isConnected(false),
		name()
	{
		_readBuffer.Resize(READ_BLOCK_SIZE);
	}
	virtual ~UdpSocketClient()
	{
		_closed = true;
		boost::system::error_code error;
		_socket->close(error);
	}
	//��ʼ��c-c����
	void Start(const char* addr, uint16_t port)
	{
		_isC2SUdp = false;
		CloseSocket();
		_remoteEndpoint = udp::endpoint(boost::asio::ip::address::from_string(addr), port);
		//_socket->set_option(boost::asio::socket_base::reuse_address(true));
		std::cout << "udp c-c ed " << _remoteEndpoint << std::endl;
		SkyDream::IntValue iv;
		iv.set_value(0);
		char buff[16] = { 0 };
		int16 size = iv.ByteSize();
		iv.SerializePartialToArray(buff, size);
		SendPacket(buff, size, C2C_Opcode::C2C_HAND);
		std::thread t = std::thread(std::bind(&UdpSocketClient::AsyncWait, this));
		AsyncRead();
	}
	//��ʼ���ӷ��������߿ͻ���
	void Start(boost::asio::ip::address addr, uint16 port, int32 punch_client, uint16_t native_client)
	{
		_isC2SUdp = true;
		_remoteEndpoint = udp::endpoint(addr, port + 302);
		boost::asio::ip::udp::endpoint local_add(boost::asio::ip::address::from_string("127.0.0.1"), 7474 + punch_client);

		_socket->open(local_add.protocol());
		_socket->bind(local_add);

		_socket->set_option(boost::asio::socket_base::reuse_address(true));
		SkyDream::C2S_Punch punch;
		punch.set_from(native_client);
		punch.set_to(punch_client);
		char buff[16] = { 0 };
		int16 size = punch.ByteSize();
		punch.SerializePartialToArray(buff, size);
		SendPacket(buff, size, C2S_Opcode::C2S_PUNCH);
		AsyncRead();
	}
	void AsyncWait()
	{
		while (_handTryCount > 0)
		{
			_handTryCount--;
			Sleep(1000);
			if (_isConnected)
				return;
			SkyDream::IntValue iv;
			iv.set_value(0);
			char buff[16] = { 0 };
			int16 size = iv.ByteSize();
			iv.SerializePartialToArray(buff, size);
			std::cout << "hand again" << std::endl;
			SendPacket(buff, size, C2C_Opcode::C2C_HAND);
		}
		std::cout << "hand fail" << std::endl;

	}

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
		return _remoteEndpoint.address();
	}
	boost::asio::ip::udp::endpoint GetRemoteEndpoint()const
	{
		return _remoteEndpoint;
	}

	uint16 GetRemotePort() const
	{
		return _remoteEndpoint.port();
	}

	void AsyncRead()
	{
		if (!IsOpen())
			return;

		_readBuffer.Normalize();
		_readBuffer.EnsureFreeSpace();
		_socket->async_receive_from(boost::asio::buffer(_readBuffer.GetWritePointer(), _readBuffer.GetRemainingSpace())\
			, _receiveEndpoint, boost::bind(&UdpSocketClient::ReadHandlerInternal, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
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

		OnClose();
	}

	/// Marks the socket for closing after write buffer becomes empty
	void DelayedCloseSocket() { _closing = true; }

	MessageBuffer& GetReadBuffer() { return _readBuffer; }

protected:
	virtual void OnClose() { }

	void ReadHandler()
	{
		if (!IsOpen())
			return;

		MessageBuffer& packet = GetReadBuffer();
		while (packet.GetActiveSize() > 0)
		{
			if (_headerBuffer.GetRemainingSpace() > 0)
			{
				// need to receive the header
				std::size_t readHeaderSize = std::min(packet.GetActiveSize(), _headerBuffer.GetRemainingSpace());
				_headerBuffer.Write(packet.GetReadPointer(), readHeaderSize);
				packet.ReadCompleted(readHeaderSize);

				if (_headerBuffer.GetRemainingSpace() > 0)
				{
					// Couldn't receive the whole header this time.
					//ASSERT(packet.GetActiveSize() == 0);
					break;
				}

				// We just received nice new header
				if (!ReadHeaderHandler())
				{
					CloseSocket();
					return;
				}
			}

			// We have full read header, now check the data payload
			if (_packetBuffer.GetRemainingSpace() > 0)
			{
				// need more data in the payload
				std::size_t readDataSize = std::min(packet.GetActiveSize(), _packetBuffer.GetRemainingSpace());
				_packetBuffer.Write(packet.GetReadPointer(), readDataSize);
				packet.ReadCompleted(readDataSize);

				if (_packetBuffer.GetRemainingSpace() > 0)
				{
					// Couldn't receive the whole data this time.
					//ASSERT(packet.GetActiveSize() == 0);
					break;
				}
			}

			// just received fresh new payload
			ReadDataHandlerResult result = ReadDataHandler();
			_headerBuffer.Reset();
			if (result != ReadDataHandlerResult::Ok)
			{
				if (result != ReadDataHandlerResult::WaitingForQuery)
					CloseSocket();

				return;
			}
		}
		AsyncRead();
	}
	bool ReadHeaderHandler()
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());

		if (!header->IsValidSize() || !header->IsValidOpcode())
		{
			//TC_LOG_ERROR("network", "WorldSocket::ReadHeaderHandler(): punch_client %s sent malformed packet (size: %u, cmd: %u)",
			//	GetRemoteIpAddress().to_string().c_str(), header->Size, header->Command);
			return false;
		}

		_packetBuffer.Resize(header->Size);
		return true;
	}

	ReadDataHandlerResult ReadDataHandler();
	void SendPacket(const char*  packet, size_t size, int cmd)
	{
		if (!IsOpen())
			return;

		PacketHeader header;
		header.Command = cmd;
		header.Size = size;
		MessageBuffer buffer(size + sizeof(header));
		buffer.Write(&header, sizeof(header));
		buffer.Write(packet, size);
		QueuePacket(std::move(buffer));
	}


	bool AsyncProcessQueue()
	{
		if (_isWritingAsync)
			return false;

		_isWritingAsync = true;

#ifdef SD_SOCKET_USE_IOCP
		MessageBuffer& buffer = _writeQueue.front();
		std::cout << "send data to " << _remoteEndpoint << " ,size:" << buffer.GetActiveSize() << std::endl;
		_socket->async_send_to(boost::asio::buffer(buffer.GetReadPointer(), buffer.GetActiveSize()), _remoteEndpoint, \
			boost::bind(&UdpSocketClient::WriteHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
#else
		_socket.async_write_some(boost::asio::null_buffers(), std::bind(&TCPSocket<T>::WriteHandlerWrapper,
			this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
#endif

		return false;
	}
private:
	void ReadHandlerInternal(boost::system::error_code error, size_t transferredBytes)
	{
		std::cout << "read data " << error.message().c_str() << "\t size :" << transferredBytes << " from " << _receiveEndpoint << std::endl;
		if (error)
		{
			//CloseSocket();
			//Sleep(1000);
			SkyDream::IntValue iv;
			iv.set_value(0);
			char buff[16] = { 0 };
			int16 size = iv.ByteSize();
			iv.SerializePartialToArray(buff, size);
			SendPacket(buff, size, C2C_Opcode::C2C_HAND);
			AsyncRead();
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
			std::cout << "udp Client write bytes " << transferedBytes << ",active size " << _writeQueue.front().GetActiveSize() << std::endl;
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
		{
			std::cout << "write to remote fail " << error.message() << std::endl;
			//CloseSocket();
			Sleep(2000);
			AsyncProcessQueue();//��������
		}
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
	std::vector<punch_content> _punchWaitList;
	MessageBuffer _readBuffer;
	std::queue<MessageBuffer> _writeQueue;
	udp::endpoint _remoteEndpoint;
	udp::endpoint _receiveEndpoint;
	std::shared_ptr<udp::socket> _socket;
	std::mutex _sendLock;
	std::mutex _receiveLock;
	MessageBuffer _headerBuffer;
	MessageBuffer _packetBuffer;
	std::size_t _sendBufferSize;


	std::atomic<bool> _closed;
	std::atomic<bool> _closing;
	int16 _handTryCount;

	bool _isWritingAsync;
	bool _isConnected;
	bool _isC2SUdp;
};

#endif // __UDP_SOCKET_H__