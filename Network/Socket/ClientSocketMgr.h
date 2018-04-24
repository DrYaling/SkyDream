#ifndef _WORLDSOCKETMGR_H
#define _WORLDSOCKETMGR_H
#include "SocketMgr.h"
#include "Client.h"
#define sClientSocketMgr ClientSocketMgr::getInstance()
class ClientSocketMgr :public SocketMgr<ClientSocket>
{
public:
	static ClientSocketMgr* getInstance()
	{
		static ClientSocketMgr* instance = nullptr;
		if (nullptr == instance)
			instance = new ClientSocketMgr(10);
		return instance;
	}
public:
	ClientSocketMgr(size_t socket_count) :SocketMgr(socket_count)
	{

	}
	void OnConnectFail(ClientSocket* sock,boost::asio::ip::address addr,int16_t port)
	{
		std::cout << sock->name << "   connect fail" << std::endl;
		for (auto i  =_connections.begin();i!=_connections.end();i++)
		{
			//std::cout << "search sock " << (*i)->name << std::endl;
			if ((*i) == sock)
			{
				_connections.erase(i);
				//sock->CloseSocket();
				//delete sock;
				tcp::socket * nsock = sClientSocketMgr->GetSocket();
				ClientSocket* client = new ClientSocket(nsock);
				client-> name= std::move("remote client");
				std::cout << "remote client try connect to " << addr << ":" << port << std::endl;
				client->Start(addr.to_string().c_str(), port);
				sClientSocketMgr->OnSocketConnect(client);
				return;
			}
		}

	}
};
#endif