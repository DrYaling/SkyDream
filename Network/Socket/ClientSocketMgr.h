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
	ClientSocketMgr(size_t socket_count) :SocketMgr(socket_count)
	{
		_udpConnectionMap.clear();
	}
	void OnConnectRemote(int32 clientId, udp::endpoint endpoint)
	{
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
public :
	std::map<int32, boost::asio::ip::udp::endpoint> _udpConnectionMap;
};
#endif