#pragma once
#include "const.h"

/*
* �����¼� Start()
* ��ʱ���� CheckDeadline()
* ��Ӧ���� WriteResponse()
* �������� HandleReq()
*/
class HttpConnection: public std::enable_shared_from_this<HttpConnection>
{
public:
	friend class LogicSystem;
	HttpConnection(tcp::socket socket);
	void Start();
private:
	void CheckDeadline();
	void WriteResponse();
	void HandleReq();
	tcp::socket _socket;
	// buffer
	beast::flat_buffer _buffer{ 8192 };
	// request message
	http::request<http::dynamic_body> _request;
	// response message
	http::response<http::dynamic_body> _response;
	// timer for deadline
	net::steady_timer deadline_{
		_socket.get_executor(), std::chrono::seconds(60)
	};

};

