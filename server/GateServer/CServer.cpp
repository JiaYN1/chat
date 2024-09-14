#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port) : _ioc(ioc), 
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc) {

}

void CServer::Start()
{
	auto self = shared_from_this();  //防止回调过程中类被析构释放
	_acceptor.async_accept(_socket, [self](beast::error_code err) {
		try {
			//出错则放弃这个连接，继续监听
			if (err) {
				self->Start();
				return;
			}

			//处理新链接，创建HpptConnection类管理新连接
			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
			self->Start();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
			self->Start();
		}
	});
}


