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

enum ReqId{
    ID_GET_VARIFY_CODE = 1001,  // 获取验证码
    ID_REG_USER = 1002,         // 注册用户
};

enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON = 1,    // json解析失败
    ERR_NETWORK = 2,
};

enum Modules{
    RESIGERMOD = 0,
};

extern QString gate_url_prefix;

#endif // GLOBAL_H
