#include "WorldSocketMgr.h"
static void OnSocketAccept(std::shared_ptr<tcp::socket> sock)
{
	//sWorldSocketMgr.OnSocketOpen(std::forward<tcp::socket>(sock), threadIndex);
	std::cout << "server accept callback " << sock->remote_endpoint() << std::endl;
	auto conn = std::make_shared<WorldSocket>(std::move(sock));
	conn->Start();
	sWorldSocketMgr->OnSocketConnect(conn);
}
void WorldSocketMgr::StartUp()
{
	auto sock = GetSocket();
	_acceptor = std::make_shared<AsyncAcceptor>(_io_service, "118.113.200.77", 8081);
	if (!_acceptor->Bind())
		std::cout << "error acceptor bind fail" << std::endl;
	_acceptor->AsyncAcceptWithCallback<&OnSocketAccept>();
	_udpServer = std::make_shared< UdpSocketServer>(_io_service);
	_udpServer->Start(_acceptor->GetBindAddress(), _acceptor->GetBindPort());
	boost::asio::ip::tcp::endpoint local_endp;
	auto endpoint = GetEndpoint(local_endp);
	boost::asio::ip::address local_address = local_endp.address();
	std::cout << "remote end " << endpoint << ",local " << local_endp << std::endl;
}

void WorldSocketMgr::Stop()
{
	_acceptor->Stop();
}
