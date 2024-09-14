#include "HttpConnection.h"
#include "LogicSystem.h"

HttpConnection::HttpConnection(tcp::socket socket): _socket(std::move(socket)){

}

// ����������
void HttpConnection::Start()
{
	auto self = shared_from_this();
	http::async_read(_socket, _buffer, _request, [self](beast::error_code err,
		std::size_t bytes_transferred) {
		try {
			if (err) {
				std::cout << "http read err is " << err.what() << std::endl;
				return;
			}

			boost::ignore_unused(bytes_transferred);
			self->HandleReq();
			self->CheckDeadline();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
		}
	});
}

void HttpConnection::CheckDeadline()
{
	auto self = shared_from_this();

	deadline_.async_wait([self](beast::error_code err) {
		if (!err) {
			self->_socket.close(err);
		}
	}
	);
}

void HttpConnection::WriteResponse()
{
	auto self = shared_from_this();
	_response.content_length(_response.body().size());
	// http������ ������Ͽ����Ͷ� �յ����͵ĺ����ȡ����ʱ��
	http::async_write(
		_socket,
		_response,
		[self](beast::error_code err, std::size_t) {
		self->_socket.shutdown(tcp::socket::shutdown_send, err);
		self->deadline_.cancel();
	}
	);
}

void HttpConnection::HandleReq()
{
	// ���ð汾
	_response.version(_request.version());
	_response.keep_alive(false);

	// get
	if (_request.method() == http::verb::get) {
		// Ŀ��·��
		bool success = LogicSystem::GetInstance()->HandleGet(_request.target(), shared_from_this());
		if (!success) {
			_response.result(http::status::not_found); // 404
			_response.set(http::field::content_type, "text/plain");
			beast:ostream(_response.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}

		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}
}
