#ifndef _CLIENT_H
#define _CLIENT_H
#include "TCPSocket.h"
#include "SocketMgr.h"
#include <google/protobuf/message.h>
#include "UdpSocketClient.h"
//const char* serverIp = "127.0.0.1";
class ClientSocket:public TCPSocket<ClientSocket> 
{
	typedef TCPSocket<ClientSocket> BaseSocket;
public:
	explicit ClientSocket(tcp::socket* socket);
	~ClientSocket() override;
	ClientSocket(ClientSocket const& right) = delete;
	ClientSocket& operator=(ClientSocket const& right) = delete;
	void ReadHandler() override;
	bool ReadHeaderHandler();

	ReadDataHandlerResult ReadDataHandler();
	void Start(const char* url, uint16_t port);
	void Start() override;
	void Bind(const char*url, uint16_t port);
	bool Update() override;

	void SendPacket(const char*  packet, size_t size,int cmd);

	void SetSendBufferSize(std::size_t sendBufferSize) { _sendBufferSize = sendBufferSize; }
	void StartC2CConnection(const char* addr, uint16 port);
protected:
	void AsyncConnectCallback(const  boost::system::error_code& error);
private:
	UdpSocketClient * _c2cSocket;
	UdpSocketClient * _c2sSocket;
	MessageBuffer _headerBuffer;
	MessageBuffer _packetBuffer;
	std::size_t _sendBufferSize;
	bool _c2cClosed;
	std::shared_ptr<boost::asio::io_service> _io;
	std::chrono::steady_clock::time_point _LastPingTime;
};
#endif