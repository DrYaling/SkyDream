/*
created by zxb @2018-4-19
*/

#ifndef __UDP_SOCKET_SERVER_H__
#define __UDP_SOCKET_SERVER_H__

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
#include "Opcode/Opcode.h"
using boost::asio::ip::udp;
class UdpSocketServer
{

public:
	std::string name;
	explicit UdpSocketServer(boost::asio::io_service& service);
	~UdpSocketServer();
	//开始连接服务器或者客户端
	void Start(boost::asio::ip::address addr, uint16 port, int32 client);

	virtual bool Update();

	boost::asio::ip::address GetRemoteIpAddress() const;
	boost::asio::ip::udp::endpoint GetRemoteEndpoint()const;

	uint16 GetRemotePort() const;

	void AsyncRead();
	void QueuePacket(MessageBuffer&& buffer);

	bool IsOpen() const;

	void CloseSocket();

	/// Marks the socket for closing after write buffer becomes empty
	void DelayedCloseSocket();

	MessageBuffer& GetReadBuffer();

protected:
	virtual void OnClose();

	void ReadHandler();
	bool ReadHeaderHandler();

	ReadDataHandlerResult ReadDataHandler();
	void SendPacket(const char*  packet, size_t size, int cmd);


	bool AsyncProcessQueue();
private:
	void ReadHandlerInternal(boost::system::error_code error, size_t transferredBytes);

#ifdef SD_SOCKET_USE_IOCP

	void WriteHandler(boost::system::error_code error, std::size_t transferedBytes);

#else

	void WriteHandlerWrapper(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/);

	bool HandleQueue();

#endif
	std::vector<punch_content> _punchWaitList;
	MessageBuffer _readBuffer;
	std::queue<MessageBuffer> _writeQueue;
	udp::endpoint _remoteEndpoint;
	std::shared_ptr<udp::socket> _socket;
	std::mutex _sendLock;
	std::mutex _receiveLock;
	MessageBuffer _headerBuffer;
	MessageBuffer _packetBuffer;
	std::size_t _sendBufferSize;


	std::atomic<bool> _closed;
	std::atomic<bool> _closing;
	bool _isWritingAsync;
	bool _isConnected;
};

#endif // __UDP_SOCKET_H__
