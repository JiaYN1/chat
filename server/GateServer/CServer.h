#include "const.h"
#pragma once
class CServer:public std::enable_shared_from_this<CServer>
{
public:
	// io_context 与底层通信 调度器轮询 事件出发回调 windows IOCP Linux epoll
	CServer(boost::asio::io_context& ioc, unsigned short& port); 
	void Start();
private:
	tcp::acceptor _acceptor;
	net::io_context& _ioc;
	//tcp::socket _socket;
};

