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
		}
		if (nullptr != _udpServer)
		{
			_udpServer->CloseSocket();
		}
	}
	//启动
	void StartUp();
	void Stop();
	void OnSocketConnect(WorldSocket* sock)
	{
		_connections.insert(std::pair<int32,WorldSocket*>(sock->GetClientId(),sock));
	}
	void OnSocketClosed(int32 clientId)
	{
		auto itr = _connections.find(clientId);
		if (itr != _connections.end())
			_connections.erase(itr);
	}
	std::map<int32, WorldSocket*> GetConnections() const
	{
		return _connections;
	}
	bool GetEndPointOf(int32 clientId, boost::asio::ip::address& addr, uint16& port)
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
	bool GetClientIdOf(udp::endpoint& endpoint, int32& clientId)
	{
		for (auto itr : _udpConnectionMap)
		{
			if (itr.second == endpoint)
			{
				clientId = itr.first;
				return true;
			}
		}
		return false;
	}
	bool GetEndPointOf(int32 clientId, udp::endpoint& endpoint)
	{
		auto itr = _udpConnectionMap.find(clientId);
		if (itr != _udpConnectionMap.end())
		{
			endpoint = itr->second;
			return true;
		}
		return false;


	}
	void OnConnectRemoteUdp(int32 clientId, udp::endpoint& endpoint)
	{
		auto itr = _udpConnectionMap.find(clientId);
		//从新添加，有肯能endpoint变了
		if (itr != _udpConnectionMap.end())
		{
			_udpConnectionMap.erase(itr);
		}
		_udpConnectionMap.insert(std::pair<int32, udp::endpoint>(clientId, endpoint));
	}
	void OnDisconnectRemoteUdp(int32 clientId)
	{
		auto itr = _udpConnectionMap.find(clientId);
		if (itr != _udpConnectionMap.end())
		{
			_udpConnectionMap.erase(itr);
		}
	}
private :
	std::shared_ptr<AsyncAcceptor> _acceptor;
	std::shared_ptr<UdpSocketServer> _udpServer;
	std::map<int32, WorldSocket*> _connections;
	std::map<int32, boost::asio::ip::udp::endpoint> _udpConnectionMap;
};
#endif