#ifndef _WORLDSOCKETMGR_H
#define _WORLDSOCKETMGR_H
#include "SocketMgr.h"
#include "WorldSocket.h"
#include "AsyncAcceptor.h"
#include "UdpSocketServer.h"
#define sWorldSocketMgr WorldSocketMgr::getInstance()

class WorldSocketMgr :public SocketMgr<WorldSocket>
{
public:
	static WorldSocketMgr* getInstance()
	{
		static WorldSocketMgr* instance = nullptr;
		if (nullptr == instance)
			instance = new WorldSocketMgr(10);
		return instance;
	}
public:
	WorldSocketMgr(size_t socket_count) :SocketMgr(socket_count)
	{

	}
	~WorldSocketMgr()
	{
		if (nullptr != _acceptor)
		{
			_acceptor->Stop();
			delete _acceptor;
		}
		if (nullptr != _udpServer)
		{
			_udpServer->CloseSocket();
			delete _udpServer;
		}
	}
	//Æô¶¯
	void StartUp();
	void Stop();
private :
	AsyncAcceptor* _acceptor;
	UdpSocketServer* _udpServer;
};
#endif