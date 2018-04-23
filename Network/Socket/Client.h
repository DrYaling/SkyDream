#ifndef _CLIENT_H
#define _CLIENT_H
#include "Socket.h"
#include "SocketMgr.h"
//const char* serverIp = "127.0.0.1";
class ClientSocket:public Socket<ClientSocket> 
{
	typedef Socket<ClientSocket> BaseSocket;
public:
	static ClientSocket* Create()
	{
		auto sock = sSocketMgr->GetSocket();
		//sock->non_blocking(true);		
		auto clientSock =new ClientSocket(sock);
		return clientSock;
	}
	explicit ClientSocket(tcp::socket* socket);

	ClientSocket(ClientSocket const& right) = delete;
	ClientSocket& operator=(ClientSocket const& right) = delete;
	void ReadHandler() override {}
	void Start() override;
	bool Update() override;

	void SendPacket(const char*  packet, size_t size);

	void SetSendBufferSize(std::size_t sendBufferSize) { _sendBufferSize = sendBufferSize; }
protected:
	void AsyncConnectCallback(const  boost::system::error_code& error);
private:

	MessageBuffer _headerBuffer;
	MessageBuffer _packetBuffer;
	std::size_t _sendBufferSize;
	std::shared_ptr<boost::asio::io_service> _io;
	std::chrono::steady_clock::time_point _LastPingTime;
};
#endif