#include "StatusServiceImpl.h"
#include "ConfigMgr.h"
#include "const.h"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/random_generator.hpp"
#include "boost/uuid/uuid_io.hpp"

StatusServiceImpl::StatusServiceImpl()
{
	auto& cfg = ConfigMgr::Inst();
	ChatServer server;
	server.port = cfg["ChatServer1"]["Port"];
	server.host = cfg["ChatServer1"]["Host"];
	server.name = cfg["ChatServer1"]["Name"];
	_servers[server.name] = server;

	server.port = cfg["ChatServer2"]["Port"];
	server.host = cfg["ChatServer2"]["Host"];
	server.name = cfg["ChatServer2"]["Name"];
	_servers[server.name] = server;
}

StatusServiceImpl::~StatusServiceImpl()
{
}

std::string generate_unique_string() {
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	// 转为string
	std::string unique_string = to_string(uuid);

	return unique_string;
}

Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply)
{
	std::string prefix("chat status server has received : ");
	auto server = getChatServer();
	reply->set_host(server.host);
	reply->set_port(server.port);
	reply->set_error(ErrorCodes::Success);
	reply->set_token(generate_unique_string());
	insertToken(request->uid(), reply->token());
	return Status::OK;
}

Status StatusServiceImpl::Login(ServerContext* context, const LoginReq* request, LoginRsp* reply)
{
	auto uid = request->uid();
	auto token = request->token();
	std::lock_guard<std::mutex> guard(_token_mtx);
	auto iter = _tokens.find(uid);
	if (iter == _tokens.end()) {
		reply->set_error(ErrorCodes::UidInvalid);
		return Status::OK;
	}
	if (iter->second != token) {
		reply->set_error(ErrorCodes::TokenInvalid);
		return Status::OK;
	}
	reply->set_error(ErrorCodes::Success);
	reply->set_uid(uid);
	reply->set_token(token);
	return Status::OK;
}

void StatusServiceImpl::insertToken(int uid, std::string token)
{
	std::lock_guard<std::mutex> guard(_token_mtx);
	_tokens[uid] = token;
}

ChatServer StatusServiceImpl::getChatServer()
{
	std::lock_guard<std::mutex> lock(_server_mtx);
	auto minServer = _servers.begin()->second;
	for (const auto& server : _servers) {
		if (server.second.con_count < minServer.con_count) { // 找到连接数量最小的服务
			minServer = server.second;
		}
	}

	return minServer;
}
