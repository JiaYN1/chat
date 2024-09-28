#include "LogicSystem.h"
#include "const.h"
#include "StatusGrpcClient.h"
#include "MysqlMgr.h"
#include "RedisMgr.h"
#include "UserMgr.h"

LogicSystem::~LogicSystem()
{
	_b_stop = true;
	_consume.notify_one();
	_worker_thread.join();
}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> msg)
{
	std::unique_lock<std::mutex> unilock(_mutex);
	_msg_que.push(msg);
	// 由0变为1发送通知信号
	if (_msg_que.size() == 1) {
		unilock.unlock();
		_consume.notify_one();
	}
}

LogicSystem::LogicSystem():_b_stop(false)
{
	RegisterCallBacks();
	_worker_thread = std::thread (&LogicSystem::DealMsg, this);
}

void LogicSystem::DealMsg()
{
	for (;;) {
		std::unique_lock<std::mutex> unilock(_mutex);
		// 队列为空阻塞等待
		while (_msg_que.empty() && !_b_stop) {
			_consume.wait(unilock);
		}
		//判断是否为关闭状态，把所有逻辑执行完后则退出循环
		if (_b_stop) {
			while (!_msg_que.empty()) {
				auto msg_node = _msg_que.front();
				std::cout << "recv_msg is " << msg_node->_recvnode->_msg_id << std::endl;
				auto callbackIter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
				if (callbackIter == _fun_callbacks.end()) {
					_msg_que.pop();
					continue;
				}
				callbackIter->second(msg_node->_session, msg_node->_recvnode->_msg_id, std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
				_msg_que.pop();
			}
			break;
		}
		//如果没有停服，且说明队列中有数据
		auto msg_node = _msg_que.front();
		std::cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << std::endl;
		auto callbackIter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
		if (callbackIter == _fun_callbacks.end()) {
			_msg_que.pop();
			std::cout << "msg id [" << msg_node->_recvnode->_msg_id << "] handler not found" << std::endl;
			continue;
		}
		callbackIter->second(msg_node->_session, msg_node->_recvnode->_msg_id, std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
		_msg_que.pop();
	}
}

void LogicSystem::RegisterCallBacks()
{
	_fun_callbacks[MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LoginHandler, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::LoginHandler(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	auto uid = root["uid"].asInt();
	std::cout << "user login uid is " << uid << " token is " <<
		root["token"].asString() << std::endl;

	// 从状态服务器获取token匹配是否准确
	auto rsp = StatusGrpcClient::GetInstance()->Login(uid, root["token"].asString());
	Json::Value rtvalue;
	Defer defer([this, &rtvalue, session]() {
		std::string return_str = rtvalue.toStyledString();
		session->Send(return_str, MSG_CHAT_LOGIN_RSP);
	});

	rtvalue["error"] = rsp.error();
	if (rsp.error() != ErrorCodes::Success) {
		return;
	}

	// 内存中查询用户信息
	//auto find_iter = _users.find(uid);
	//std::shared_ptr<UserInfo> user_info = nullptr;
	//if (find_iter == _users.end()) {
	//	//查询数据库
	//	user_info = MysqlMgr::GetInstance()->GetUser(uid);
	//	if (user_info == nullptr) {
	//		rtvalue["error"] = ErrorCodes::UidInvalid;
	//		return;
	//	}

	//	_users[uid] = user_info;
	//}
	//else {
	//	user_info = find_iter->second;
	//}

	rtvalue["uid"] = uid;
	rtvalue["token"] = rsp.token();
	//rtvalue["name"] = user_info->name;
	return;
}
