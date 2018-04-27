#ifndef _CLIENT_SOCKETMGR_H
#define _CLIENT_SOCKETMGR_H
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
	ClientSocketMgr(size_t socket_count) :SocketMgr(socket_count),_client(nullptr)
	{
		_udpConnectionMap.clear();
	}
	~ClientSocketMgr()
	{
		Stop();
	}
	void StartUp()
	{
		std::shared_ptr<tcp::socket> sock = GetSocket();
		_client = new  ClientSocket(std::move(sock));
		_client->name = std::move("native client");
		Sleep(500);
		//_client->Start("118.113.200.77", 8081);
		_client->Start("127.0.0.1", 8081);
	}
	void Stop()
	{
		if (nullptr != _client)
		{
			if (_client->IsOpen())
				_client->CloseSocket();
			delete _client;
			_client = nullptr;
		}
	}
	void OnConnectRemote(int32 clientId, udp::endpoint endpoint)
	{
		auto itr = _udpConnectionMap.find(clientId);
		//从新添加，有肯能endpoint变了
		if (itr != _udpConnectionMap.end())
		{
			_udpConnectionMap.erase(itr);
		}
		_udpConnectionMap.insert(std::pair<int32, udp::endpoint>(clientId, endpoint));
	}
	void OnDisconnectRemote(int32 clientId)
	{
		auto itr = _udpConnectionMap.find(clientId);
		if (itr != _udpConnectionMap.end())
		{
			_udpConnectionMap.erase(itr);
		}
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
	int32 GetNativeClientId()
	{
		return _client->GetClientId();
	}
public :
	std::map<int32, boost::asio::ip::udp::endpoint> _udpConnectionMap;
	ClientSocket* _client;
};
#endif