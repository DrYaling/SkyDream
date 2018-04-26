#include "WorldSocketMgr.h"
static void OnSocketAccept(tcp::socket* sock)
{
	//sWorldSocketMgr.OnSocketOpen(std::forward<tcp::socket>(sock), threadIndex);
	std::cout << "server accept callback " << sock->remote_endpoint() << std::endl;
	WorldSocket* conn = new WorldSocket(sock);
	conn->Start();
	sWorldSocketMgr->OnSocketConnect(conn);
}
void WorldSocketMgr::StartUp()
{
	auto sock = GetSocket();
	_acceptor = new AsyncAcceptor(sock, "0.0.0.0", 8081);
	if (!_acceptor->Bind())
		std::cout << "error acceptor bind fail" << std::endl;
	_acceptor->AsyncAcceptWithCallback<&OnSocketAccept>();
	_udpServer = new UdpSocketServer(sock->get_io_service());
	_udpServer->Start(_acceptor->GetBindAddress(), _acceptor->GetBindPort());
}

void WorldSocketMgr::Stop()
{
	_acceptor->Stop();
}
