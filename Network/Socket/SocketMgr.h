#ifndef _SOCKET_MGR_H
#define _SOCKET_MGR_H
#include <boost/asio/io_service.hpp>
#include <boost/asio.hpp>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <memory>
#include <iostream>
#include "Socket.h"
using namespace std;
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
template<class T>
class SocketMgr
{
public:
	SocketMgr(size_t socketCount) :_io_service(1), _io_work(_io_service), _connections(0)
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
	void Update()
	{
		for (auto s : _connections)
		{
			if (s)
				s->Update();
		}
	}
	boost::asio::ip::tcp::socket* GetSocket()
	{
		for (auto sock = m_vSockets.begin(); sock != m_vSockets.end(); sock++)
		{
			if ((*sock).isAvaliable)
			{
				(*sock).isAvaliable = false;
				auto ret = (*sock).socket;
				m_vSockets.erase(sock);
				return ret;
			}
		}
		return new  boost::asio::ip::tcp::socket(_io_service);
	}
	void OnSocketConnect(Socket<T>* sock)
	{
		_connections.push_back(sock);
	}
	vector<Socket<T>*> GetConnections() const
	{
		return _connections;
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
protected:
	std::vector<Socket<T>*> _connections;
private:
	void Io_Service_Worker()
	{
		//std::cout << "IO_SREVICE_WORKER START" << std::endl;
		_io_service.run();
		//std::cout << "IO_SREVICE_WORKER END" << std::endl;
	}
	std::vector<SocketInstance> m_vSockets;
	boost::asio::io_service _io_service;
	boost::asio::io_service::work _io_work;

};
#endif
