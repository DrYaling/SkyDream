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
void Help()
{
	std::cout << "-list to show connections" << std::endl;
	std::cout << "-local to show local addr" << std::endl;
}
void OnCmd(const char* cmd)
{
	
	if (strcmp(cmd, "-list") == 0)
	{
		for (auto itr : sWorldSocketMgr->GetConnections())
		{
			std::cout << itr.second->GetRemoteIpAddress() << ":" << itr.second->GetRemotePort() << ",id:"<< itr.second->GetClientId()<<std::endl;
		}
	}
	else if (strcmp(cmd, "-help") == 0)
	{
		Help();
	}
	else if (strcmp(cmd, "-local") == 0)
	{
		std::cout << "no saved data" << std::endl;
	}
}
void ThreadAccepter()
{
	sWorldSocketMgr->StartUp();
	//auto client = new  ClientSocket(sock);
	//Sleep(500);
	//client->Start("127.0.0.1", 8081);
	while (1)
	{
		sWorldSocketMgr->Update();
		char input[256];
		std::cin >> input;
		OnCmd(input);
		Sleep(6);
	}

}
int main()
{
	auto thread = std::thread(ThreadAccepter);
	thread.detach();
	while (true) Sleep(1);
}