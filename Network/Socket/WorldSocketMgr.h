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
		_connections.clear();

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
	void OnSocketConnect(WorldSocket* sock)
	{
		_connections.insert(std::pair<int32,WorldSocket*>(sock->GetClientId(),sock));
	}
	std::map<int32, WorldSocket*> GetConnections() const
	{
		return _connections;
	}
	bool GetEndpointOf(int32 clientId, boost::asio::ip::address& addr, uint16& port)
	{
		auto itr = _connections.find(clientId);
		if (itr != _connections.end())
		{
			WorldSocket* sock = itr->second;
			addr = sock->GetRemoteIpAddress();
			port = sock->GetRemotePort();
			return true;
		}
		return false;
	}
private :
	AsyncAcceptor* _acceptor;
	UdpSocketServer* _udpServer;
	std::map<int32,WorldSocket*> _connections;
};
#endif