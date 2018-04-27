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
		_connectingMapLock(),
		_punchWaitList(),
		_closed(false),
		_closing(false),
		_isWritingAsync(false),
		_sendLock(),
		_receiveLock(),
		_handTryCount(6),
		name()
	{
		std::lock_guard<std::mutex> _lock_guard(_connectingMapLock);
		_connectingMap.clear();
		_readBuffer.Resize(READ_BLOCK_SIZE);
		_headerBuffer.Resize(sizeof(PacketHeader));
	}
	virtual ~UdpSocketClient()
	{
		_closed = true;
		boost::system::error_code error;
		_socket->close(error);
	}
	void Bind(boost::asio::ip::address&& addr, uint16 port, int32 clientId);

	//开始做c-c连接
	void StartC2C(const char* addr, uint16_t port, int32 clientId);
	void Punch(boost::asio::ip::address&& addr, uint16 port, int32 punch_client, uint16_t native_client);
	void AsyncWait(int32 clientId)
	{
		while (true)
		{
			Sleep(1000);
			{
				std::lock_guard<std::mutex> _lock_guard(_connectingMapLock);
				auto itr = _connectingMap.find(clientId);
				if (itr == _connectingMap.end())
				{
					return;
				}
				int16 waitCount = itr->second;
				waitCount--;
				_connectingMap.erase(itr);
				if (waitCount <= 0)
				{
					break;
				}
				_connectingMap.insert(std::pair<int32, int16>(clientId, waitCount));
			}
			SkyDream::IntValue iv;
			iv.set_value(0);
			char buff[16] = { 0 };
			int16 size = iv.ByteSize();
			iv.SerializePartialToArray(buff, size);
			std::cout << "hand again" << std::endl;
			SendPacket(buff, size, C2C_Opcode::C2C_HAND, clientId);
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
		_socket->async_receive(boost::asio::buffer(_readBuffer.GetWritePointer(), _readBuffer.GetRemainingSpace()),\
			0, boost::bind(&UdpSocketClient::ReadHandlerInternal, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		//_socket->async_receive_from(boost::asio::buffer(_readBuffer.GetWritePointer(), _readBuffer.GetRemainingSpace())\
			, _receiveEndpoint, boost::bind(&UdpSocketClient::ReadHandlerInternal, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	void QueuePacket(udpSocketWriteBuffer&& buffer)
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

		boost::system::error_code err;
		_socket->shutdown(boost::asio::socket_base::shutdown_send, err);
		if (err)
			std::cout << "shutdown udp socket fail " << err.message() << std::endl;// SD_LOG_DEBUG("network", "TCPSocket::CloseSocket: %s errored when shutting down socket: %i (%s)", GetRemoteIpAddress().to_string().c_str(),
			//	err.value(), err.message().c_str());
		_socket->close(err);
		if (err)
			std::cout << "udp close err " << err.message() << std::endl;
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
		_packetBuffer.Reset();
		_packetBuffer.Resize(header->Size);
		return true;
	}

	ReadDataHandlerResult ReadDataHandler();
	void SendPacket(const char*  packet, size_t size, int cmd,int remoteId)
	{
		if (!IsOpen())
			return;

		PacketHeader header;
		header.Command = cmd;
		header.Size = size;
		MessageBuffer buffer(size + sizeof(header));
		buffer.Write(&header, sizeof(header));
		buffer.Write(packet, size);
		QueuePacket(udpSocketWriteBuffer(remoteId,buffer));
	}


	bool AsyncProcessQueue();
private:
	void ReadHandlerInternal(boost::system::error_code error, size_t transferredBytes);

#ifdef SD_SOCKET_USE_IOCP

	void WriteHandler(boost::system::error_code error, std::size_t transferedBytes)
	{
		if (!error)
		{
			std::cout << "udp client write to remote size " << transferedBytes << std::endl;
			_isWritingAsync = false;
			_writeQueue.front().buffer.ReadCompleted(transferedBytes);
			if (!_writeQueue.front().buffer.GetActiveSize())
				_writeQueue.pop();
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
			AsyncProcessQueue();//继续尝试
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
	std::queue<udpSocketWriteBuffer> _writeQueue;
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
	std::map<int32, int16> _connectingMap;
	std::mutex _connectingMapLock;
	int32 _clientId;
};

#endif // __UDP_SOCKET_H__
