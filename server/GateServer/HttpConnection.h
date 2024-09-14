#pragma once
#include "const.h"

/*
* 监听事件 Start()
* 超时处理 CheckDeadline()
* 响应请求 WriteResponse()
* 处理请求 HandleReq()
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
	void PreParseGetParam();
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
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
};

