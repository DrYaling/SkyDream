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
static void OnSocketAccept(tcp::socket* sock)
{
	std::cout << "client receive connection " << sock->remote_endpoint() << std::endl;
	WorldSocket* conn = new WorldSocket(sock);
	conn->Start();
	_p2pConnections.push_back(conn);
}
void ThreadAccepter()
{
	_p2pConnections.clear();
	//创建客户端监听
	tcp::socket * sock = sClientSocketMgr->GetSocket();
	//创建客户端连接
	auto client = new  ClientSocket(sock);
	client->name = std::move("native client");
	//client->Bind("127.0.0.1", 18081);
	Sleep(500);
	//client->Start("118.113.200.77", 8081);
	client->Start("127.0.0.1", 8081);
	sClientSocketMgr->client = client;
	while (1)
	{
		Sleep(1);
	}


}
int main()
{
	auto thread = std::thread(ThreadAccepter);
	thread.detach();
	while (true) Sleep(1);
	return 0;
}