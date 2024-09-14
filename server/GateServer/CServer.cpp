#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port) : _ioc(ioc), 
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc) {

}

void CServer::Start()
{
	auto self = shared_from_this();  //��ֹ�ص��������౻�����ͷ�
	_acceptor.async_accept(_socket, [self](beast::error_code err) {
		try {
			//���������������ӣ���������
			if (err) {
				self->Start();
				return;
			}

			//���������ӣ�����HpptConnection�����������
			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
			self->Start();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
			self->Start();
		}
	});
}


