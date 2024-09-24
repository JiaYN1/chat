#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include "QStyle"
#include <QRegularExpression>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDir>
#include <QSettings>

/*
 * 刷新状态
 */
extern std::function<void(QWidget*)> repolish;

extern std::function<QString(QString)> xorString;

enum ReqId{
    ID_GET_VARIFY_CODE = 1001,  // 获取验证码
    ID_REG_USER = 1002,         // 注册用户
    ID_RESET_PWD = 1003,        // 忘记密码
    ID_USER_LOGIN = 1004,       // 用户登录
};

enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON = 1,    // json解析失败
    ERR_NETWORK = 2,
};

enum Modules{
    REGISTERMOD = 0,
    RESETMOD = 1,
    LOGINMOD = 2,
};

enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};

enum ClickLbState{
    Normal = 0,
    Selected = 1
};

extern QString gate_url_prefix;

struct ServerInfo{
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

#endif // GLOBAL_H
