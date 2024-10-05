#include "LogicSystem.h"
#include "const.h"
#include "StatusGrpcClient.h"
#include "MysqlMgr.h"
#include "RedisMgr.h"
#include "UserMgr.h"
#include "ChatGrpcClient.h"

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
	// ��0��Ϊ1����֪ͨ�ź�
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
		// ����Ϊ�������ȴ�
		while (_msg_que.empty() && !_b_stop) {
			_consume.wait(unilock);
		}
		//�ж��Ƿ�Ϊ�ر�״̬���������߼�ִ��������˳�ѭ��
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
		//���û��ͣ������˵��������������
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
	auto token = root["token"].asString();
	std::cout << "user login uid is " << uid << " token is " <<
		token << std::endl;


	Json::Value  rtvalue;
	Defer defer([this, &rtvalue, session]() {
		std::string return_str = rtvalue.toStyledString();
		session->Send(return_str, MSG_CHAT_LOGIN_RSP);
	});
	//��redis��ȡ�û�token�Ƿ���ȷ
	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	std::string token_value = "";
	bool success = RedisMgr::GetInstance()->Get(token_key, token_value);
	if (!success) {
		rtvalue["error"] = ErrorCodes::UidInvalid;
		return;
	}

	if (token_value != token) {
		rtvalue["error"] = ErrorCodes::TokenInvalid;
		return;
	}

	rtvalue["error"] = ErrorCodes::Success;

	std::string base_key = USER_BASE_INFO + uid_str;
	auto user_info = std::make_shared<UserInfo>();
	bool b_base = ChatGrpcClient::GetInstance()->GetBaseInfo(base_key, uid, user_info);
	if (!b_base) {
		rtvalue["error"] = ErrorCodes::UidInvalid;
		return;
	}
	rtvalue["uid"] = uid;
	rtvalue["pwd"] = user_info->pwd;
	rtvalue["name"] = user_info->name;
	rtvalue["email"] = user_info->email;
	rtvalue["nick"] = user_info->nick;
	rtvalue["desc"] = user_info->desc;
	rtvalue["sex"] = user_info->sex;
	rtvalue["icon"] = user_info->icon;

	//�����ݿ��ȡ�����б�

	//��ȡ�����б�

	auto server_name = ConfigMgr::Inst().GetValue("SelfServer", "Name");
	//����¼��������
	auto rd_res = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server_name);
	int count = 0;
	if (!rd_res.empty()) {
		count = std::stoi(rd_res);
	}

	count++;

	auto count_str = std::to_string(count);
	RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, count_str);

	//session���û�uid
	session->SetUserId(uid);

	//Ϊ�û����õ�¼ip server������
	std::string  ipkey = USERIPPREFIX + uid_str;
	RedisMgr::GetInstance()->Set(ipkey, server_name);

	//uid��session�󶨹���,�����Ժ����˲���
	UserMgr::GetInstance()->SetUserSession(uid, session);

	return;
}
