#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <proto/test.pb.h>
#include <Socket/WorldSocket.h>
#include <Asio/IoContext.h>
#include "Common//Common.h"
#include "Socket/WorldSocket.h"
#include "Socket/Client.h"
#include "Socket/AsyncAcceptor.h"

static void OnSocketAccept(tcp::socket* sock)
{
	//sWorldSocketMgr.OnSocketOpen(std::forward<tcp::socket>(sock), threadIndex);
	std::cout << "SkyDream accept callback " << sock->remote_endpoint() << std::endl;
}
void ThreadAccepter()
{
	tcp::socket * sock = sSocketMgr->GetSocket();
	AsyncAcceptor* _acceptor = new AsyncAcceptor(sock, "0.0.0.0", 8080);
	if (!_acceptor->Bind())
		std::cout << "error acceptor bind fail" << std::endl;
	_acceptor->AsyncAcceptWithCallback<&OnSocketAccept>();
	auto client = ClientSocket::Create();
	Sleep(500);
	client->Start("127.0.0.1", 8080);
	while (1) Sleep(1);
	delete _acceptor;

}
int main()
{
	auto thread = std::thread(ThreadAccepter);
	thread.detach();
	while (true) Sleep(1);
}