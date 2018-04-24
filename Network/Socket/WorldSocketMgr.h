#ifndef _WORLDSOCKETMGR_H
#define _WORLDSOCKETMGR_H
#include "SocketMgr.h"
#include "WorldSocket.h"
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
};
#endif