#include "tcpmgr.h"
#include <QAbstractSocket>
#include "usermgr.h"

TcpMgr::~TcpMgr()
{

}

TcpMgr::TcpMgr():_host(""), _port(0), _b_recv_pending(false),
    _message_id(0), _message_len(0)
{
    QObject::connect(&_socket, &QTcpSocket::connected, [&](){
        qDebug() << "Connect to server!";
        //
        emit sig_con_success(true);
    });

    QObject::connect(&_socket, &QTcpSocket::readyRead, [&](){
        // 当有数据可读时，读取所有数据
        // 读取所有数据并追加到缓存区
        _buffer.append(_socket.readAll());

        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_6_0);

        forever{
            /*
            * tcp 传输包的内容为 id + len + data
            * 先解析头部
            * 判断剩余部分是否满足消息长度
            */
            if(!_b_recv_pending){
                // 检查缓冲区是否足够解析一个消息头
                if(_buffer.size() < static_cast<int>(sizeof(quint16) * 2)) {
                    return;
                }

                stream >> _message_id >> _message_len;

                // 移除消息头
                _buffer = _buffer.mid(sizeof(quint16) * 2);
                qDebug() << "Messgae ID: " << _message_id << " Length is : " << _message_len;
            }
            // 不满足长度
            if(_buffer.size() < _message_len) {
                _b_recv_pending = true;
                return;
            }

            _b_recv_pending = false;
            // 读取消息体
            QByteArray messageBody = _buffer.mid(0, _message_len);
            qDebug() << "receive body msg is " << messageBody;
            _buffer = _buffer.mid(_message_len);
            handleMsg(ReqId(_message_id), _message_len, messageBody);
        }
    });

    // 处理错误
    QObject::connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
                     [&](QAbstractSocket::SocketError socketError){
        Q_UNUSED(socketError);
        qDebug() << "Error: " << _socket.errorString();
    });
    // 处理连接断开
    QObject::connect(&_socket, &QTcpSocket::disconnected, [&](){
        qDebug() << "DisConnected from server.";
    });
    // 连接发送信号发送消息
    QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);
    initHandlers();
}

void TcpMgr::initHandlers()
{
    _handlers.insert(ID_CHAT_LOGIN_RSP, [this](ReqId id, int len, QByteArray data){
        qDebug() << "handle id is " << id << "data is " << data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        if(jsonDoc.isNull()){
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(!jsonObj.contains("error")){
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Login Failed, err is Json Parse Err" << err ;
            emit sig_login_failed(err);
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug() << "Login Failed, err is " << err ;
            emit sig_login_failed(err);
            return;
        }

        // UserMgr::GetInstance()->SetUid(jsonObj["uid"].toInt());
        // UserMgr::GetInstance()->SetName(jsonObj["name"].toString());
        // UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());
        emit sig_switch_chatdlg();
    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto find_iter = _handlers.find(id);
    if(find_iter == _handlers.end()){
        qDebug() << "not found id [" << id << "] in handlers";
        return;
    }
    find_iter.value()(id, len, data);
}

void TcpMgr::slot_tcp_connect(ServerInfo si)
{
    qDebug() << "receive tcp connect signal";
    qDebug() << "Connecting to server...";
    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toUInt());
    _socket.connectToHost(si.Host, _port);
}

void TcpMgr::slot_send_data(ReqId reqId, QString data)
{
    uint16_t id = reqId;
    QByteArray dataBytes = data.toUtf8();

    quint16 len = static_cast<quint16>(data.size());

    // 创建QByteArray存储发送的数据
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // 使用大端
    out.setByteOrder(QDataStream::BigEndian);
    out << id << len;
    block.append(dataBytes);

    _socket.write(block);
}
