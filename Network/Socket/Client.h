#ifndef _CLIENT_H
#define _CLIENT_H
#include "Socket.h"
#include "SocketMgr.h"
#include <google/protobuf/message.h>
//const char* serverIp = "127.0.0.1";
class ClientSocket:public Socket<ClientSocket> 
{
	typedef Socket<ClientSocket> BaseSocket;
public:
	explicit ClientSocket(tcp::socket* socket);

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
protected:
	void AsyncConnectCallback(const  boost::system::error_code& error);
private:

	MessageBuffer _headerBuffer;
	MessageBuffer _packetBuffer;
	std::size_t _sendBufferSize;
	int32 _clientId;
	std::shared_ptr<boost::asio::io_service> _io;
	std::chrono::steady_clock::time_point _LastPingTime;
};
#endif