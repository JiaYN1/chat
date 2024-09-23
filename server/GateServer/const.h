#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,  // json ��������
	RPCFailed = 1002,	// RPC�������
	VerifyExpired = 1003,  // ��֤�����
	VerifyCodeErr = 1004,	// ��֤�����
	UserExist = 1005,		// �û��Ѵ���
	PasswdErr = 1006,		// �������
	EmailNotMatch = 1007,	// ���䲻ƥ��
	PasswdUpFailed = 1008,	// �������ʧ��
	PasswdInvalid = 1009,	// ��������ʧ��
};

#define CODEPREFIX "code_"

class Defer {
public:
	// ����һ��lambda���ʽ����ָ��
	Defer(std::function<void()> func) : func_(func) {}
	// ����������ִ�д���ĺ���
	~Defer() {
		func_();
	}
private:
	std::function<void()> func_;
};