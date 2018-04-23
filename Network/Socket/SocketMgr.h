#ifndef _SOCKET_MGR_H
#define _SOCKET_MGR_H
#include <boost/asio/io_service.hpp>
#include <boost/asio.hpp>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <memory>
using namespace std;
#define sSocketMgr SocketMgr::getInstance()
struct SocketInstance {
public:
	bool isAvaliable;
	boost::asio::ip::tcp::socket* socket;
	explicit SocketInstance(boost::asio::ip::tcp::socket* sock) :socket(std::move(sock)), isAvaliable(true)
	{
	}
	SocketInstance(const SocketInstance& from) :isAvaliable(from.isAvaliable), socket(std::move(from.socket))
	{
	}
};
class SocketMgr
{
public:
	static SocketMgr* getInstance()
	{
		static SocketMgr* instance = nullptr;
		if (nullptr == instance)
			instance = new SocketMgr(2);
		return instance;
	}
	SocketMgr(size_t socketCount) :_io_service(1), _io_work(_io_service)
	{
		for (auto i = 0; i < socketCount; i++)
		{
			m_vSockets.push_back(SocketInstance(new  boost::asio::ip::tcp::socket(_io_service)));
		}
		std::thread thread(std::bind(&SocketMgr::Io_Service_Worker, this));
		thread.detach();
	}
	~SocketMgr()
	{
		m_vSockets.clear();
	}
	boost::asio::ip::tcp::socket* GetSocket()
	{
		for (auto sock : m_vSockets)
		{
			if (sock.isAvaliable)
			{
				sock.isAvaliable = false;
				return sock.socket;
			}
		}
	}
	bool ReleaseSocket(boost::asio::ip::tcp::socket* socket)
	{
		for (auto sock : m_vSockets)
		{
			if (!sock.isAvaliable && socket == sock.socket)
			{
				sock.isAvaliable = true;
				return true;
			}
		}
		return false;
	}
private:
	void Io_Service_Worker()
	{
		_io_service.run();
		std::cout << "IO_SREVICE_WORKER" << std::endl;
	}
	std::vector<SocketInstance> m_vSockets;
	boost::asio::io_service _io_service;
	boost::asio::io_service::work _io_work;

};
#endif
