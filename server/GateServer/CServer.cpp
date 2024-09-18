#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOServicePool.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port) : _ioc(ioc), 
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)) {

}

void CServer::Start()
{
	auto self = shared_from_this();  //防止回调过程中类被析构释放
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);

	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code err) {
		try {
			//出错则放弃这个连接，继续监听
			if (err) {
				self->Start();
				return;
			}

			//处理新链接，创建HpptConnection类管理新连接
			/*std::make_shared<HttpConnection>(std::move(self->_socket))->Start();*/
			new_con->Start();
			self->Start();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
			self->Start();
		}
	});
}


