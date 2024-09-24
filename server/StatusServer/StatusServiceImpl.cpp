#include "StatusServiceImpl.h"
#include "ConfigMgr.h"
#include "const.h"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/random_generator.hpp"
#include "boost/uuid/uuid_io.hpp"

StatusServiceImpl::StatusServiceImpl():_server_index(0)
{
	auto& cfg = ConfigMgr::Inst();
	ChatServer server;
	server.port = cfg["ChatServer1"]["Port"];
	server.host = cfg["ChatServer1"]["Host"];
	_servers.push_back(server);

	server.port = cfg["ChatServer2"]["Port"];
	server.host = cfg["ChatServer2"]["Host"];
	_servers.push_back(server);
}

StatusServiceImpl::~StatusServiceImpl()
{
}

std::string generate_unique_string() {
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	// תΪstring
	std::string unique_string = to_string(uuid);

	return unique_string;
}

Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply)
{
	std::string prefix("chat status server has received : ");
	_server_index = (_server_index++) % (_servers.size());
	auto& server = _servers[_server_index];
	reply->set_host(server.host);
	reply->set_port(server.port);
	reply->set_error(ErrorCodes::Success);
	reply->set_token(generate_unique_string());
	return Status::OK;
}
