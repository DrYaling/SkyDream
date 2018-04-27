#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <proto/test.pb.h>
#include <Socket/WorldSocket.h>
#include <Asio/IoContext.h>
#include "Common//Common.h"
#include "Socket/Client.h"
#include "Socket/AsyncAcceptor.h"
#include "Socket/WorldSocketMgr.h"

void ThreadAccepter()
{
	sWorldSocketMgr->StartUp();
	//auto client = new  ClientSocket(sock);
	//Sleep(500);
	//client->Start("127.0.0.1", 8081);
	while (1)
	{
		sWorldSocketMgr->Update();
		Sleep(6);
	}

}
int main()
{
	auto thread = std::thread(ThreadAccepter);
	thread.detach();
	while (true) Sleep(1);
}