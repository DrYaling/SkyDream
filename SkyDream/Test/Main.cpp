#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <proto/test.pb.h>
#include <Socket/WorldSocket.h>
#include <Asio/IoContext.h>
#include "Common//Common.h"
#include "Socket/Client.h"
#include "Socket/WorldSocketMgr.h"
#include "Socket/AsyncAcceptor.h"
#include "Socket/ClientSocketMgr.h"
//p2p连接
std::vector<WorldSocket*> _p2pConnections;
void ThreadAccepter()
{
	//创建客户端监听
	tcp::socket * sock = sClientSocketMgr->GetSocket();
	//创建客户端连接
	auto client = new  ClientSocket(sock);
	client->name = std::move("native client");
	Sleep(500);
	//client->Start("118.113.200.77", 8081);
	client->Start("127.0.0.1", 8081);
	while (1)
	{
		Sleep(16);
	}
	client->CloseSocket();
	delete client;

}
int main()
{
	auto thread = std::thread(ThreadAccepter);
	thread.detach();
	while (true) Sleep(1);
	return 0;
}