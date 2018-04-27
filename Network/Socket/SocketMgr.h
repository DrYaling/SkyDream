#ifndef _TCP_SOCKET_MGR_H
#define _TCP_SOCKET_MGR_H
#include <boost/asio/io_service.hpp>
#include <boost/asio.hpp>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <memory>
#include <iostream>
#include "TCPSocket.h"
using namespace std;
template<class T>
class SocketMgr
{
public:
	SocketMgr(size_t socketCount) :_io_service(1), _io_work(_io_service)
	{
		for (auto i = 0; i < socketCount; i++)
		{
			m_vSockets.push_back(std::make_shared<boost::asio::ip::tcp::socket>(_io_service));
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
	}
	std::shared_ptr<boost::asio::ip::tcp::socket> GetSocket()
	{
		auto itr = m_vSockets.begin();
		if (itr != m_vSockets.end())
			return *itr;
		return std::make_shared<tcp::socket>(_io_service);
	}
protected:
	boost::asio::io_service _io_service;
private:
	void Io_Service_Worker()
	{
		//std::cout << "IO_SREVICE_WORKER START" << std::endl;
		_io_service.run();
		//std::cout << "IO_SREVICE_WORKER END" << std::endl;
	}
	std::vector<std::shared_ptr<tcp::socket>> m_vSockets;
	boost::asio::io_service::work _io_work;

};
#endif
