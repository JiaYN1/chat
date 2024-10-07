#include "logindialog.h"
#include "ui_logindialog.h"
#include <QPainter>
#include <QPainterPath>
#include "httpmgr.h"
#include "tcpmgr.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 连接按钮点击事件
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
    ui->forget_label->SetState("normal", "hover", "selected", "selected hover", "");
    ui->forget_label->setCursor(Qt::PointingHandCursor);
    connect(ui->forget_label, &ClickedLable::clicked, this, &LoginDialog::slot_forget_pwd);
    initHead();

    initHttpHandlers();
    // 连接登录回包信息
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_login_mod_finish, this, &LoginDialog::slot_login_mod_finish);

    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->pass_visible->SetState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");

    connect(ui->pass_visible, &ClickedLable::clicked, this, [this](){
        auto state = ui->pass_visible->GetCurState();
        if(state == ClickLbState::Normal){
            ui->pass_edit->setEchoMode(QLineEdit::Password);
        }
        else{
            ui->pass_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    // 链接tcp链接请求的信号和槽函数
    connect(this, &LoginDialog::sig_connect_tcp, TcpMgr::GetInstance().get(), &TcpMgr::slot_tcp_connect);
    //连接tcp管理者发出的连接成功信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_con_success, this, &LoginDialog::slot_tcp_con_finish);
    //连接tcp管理者发出的登录失败信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_login_failed, this, &LoginDialog::slot_login_failed);
}

LoginDialog::~LoginDialog()
{
    delete ui;
    qDebug() << "destruct Login";
}

void LoginDialog::initHead()
{
    // 加载图片
    QPixmap originPixmap(":res/head_1.jpg");
    // 设置图片自动缩放
    qDebug() << originPixmap.size() << ui->head_label->size();
    originPixmap = originPixmap.scaled(ui->head_label->size(),
                                       Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 创建一个和原始图片大小相同的QPixmap，用于绘制圆角图片
    QPixmap roundedPixmap(originPixmap.size());
    roundedPixmap.fill(Qt::transparent); // 透明色填充

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing); //抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addRoundedRect(0, 0, originPixmap.width(), originPixmap.height(), 10, 10);
    painter.setClipPath(path);

    // 将原始图片绘制到roundedPixmap
    painter.drawPixmap(0, 0, originPixmap);

    ui->head_label->setPixmap(roundedPixmap);

}

bool LoginDialog::checkUserValid()
{
    auto user = ui->user_edit->text();
    if(user.isEmpty()){
        qDebug() << "User empty ";
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool LoginDialog::checkPwdValid()
{
    auto pwd = ui->pass_edit->text();
    if(pwd.length() < 6 || pwd.length() > 15){
        qDebug() << "Pass length invalid";
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6-15"));
        return false;
    }

    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$"); // ^ 字符串开始 $ 字符串结束
    bool match = regExp.match(pwd).hasMatch();
    if(!match){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符且长度为6-15"));
        return false;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

void LoginDialog::initHttpHandlers()
{
    _handlers.insert(ReqId::ID_USER_LOGIN, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        qDebug() << "error is : " << error;
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"), false);
            enableBtn(true);
            return;
        }
        auto user = jsonObj["user"].toString();

        // 发送信号通知HttpMgr发送长连接
        ServerInfo si;
        si.Uid = jsonObj["uid"].toInt();
        si.Host = jsonObj["host"].toString();
        si.Port = jsonObj["port"].toString();
        si.Token = jsonObj["token"].toString();

        _uid = si.Uid;
        _token = si.Token;

        showTip(tr("登录成功"), true);
        qDebug() << "user is " << user << "uid is " << si.Uid
                 << "token is " << si.Token << "host is " << si.Host
                 << "port is " << si.Port;
        emit sig_connect_tcp(si);
    });
}

void LoginDialog::showTip(QString str, bool b_ok)
{
    if(b_ok){
        ui->err_tip->setProperty("state", "normal");
    }
    else{
        ui->err_tip->setProperty("state", "err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

void LoginDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void LoginDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_errs.first(), false);
}

bool LoginDialog::enableBtn(bool enabled)
{
    ui->login_btn->setEnabled(enabled);
    ui->reg_btn->setEnabled(enabled);
    return true;
}

void LoginDialog::slot_forget_pwd()
{
    qDebug() << "slot forget pwd";
    emit switchReset();
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        showTip(tr("网络请求错误"), false);
        return;
    }
    // 解析JSON字符串 res需转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()){
        showTip(tr("json解析错误"),false);
        return;
    }

    if(!jsonDoc.isObject()){
        showTip(tr("json解析错误"),false);
        return;
    }

    _handlers[id](jsonDoc.object());
    return;
}

void LoginDialog::on_login_btn_clicked()
{
    qDebug() << "login btn clicked";
    if(!checkUserValid()){
        return;
    }

    if(!checkPwdValid()){
        return;
    }

    enableBtn(false);
    auto user = ui->user_edit->text();
    auto pwd = ui->pass_edit->text();

    QJsonObject jsonObj;
    jsonObj["user"] = user;
    jsonObj["passwd"] = xorString(pwd);
    qDebug() << jsonObj["user"] << jsonObj["passwd"];
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"), jsonObj,
                                        ReqId::ID_USER_LOGIN, Modules::LOGINMOD);
}

void LoginDialog::slot_tcp_con_finish(bool bsuccess)
{
    if(bsuccess){
        showTip(tr("聊天服务连接成功，正在登录..."), true);
        QJsonObject jsonObj;
        jsonObj["uid"] = _uid;
        jsonObj["token"] = _token;

        QJsonDocument doc(jsonObj);
        QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

        // 发送tcp请求给chat server
        TcpMgr::GetInstance()->sig_send_data(ReqId::ID_CHAT_LOGIN, jsonData);
    }
    else{
        showTip(tr("网络异常"), false);
        enableBtn(true);
    }
}

void LoginDialog::slot_login_failed(int err)
{
    QString result = QString("登录失败，err is %1").arg(err);
    showTip(result, false);
    enableBtn(true);
}

