#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    void initHead();
    bool checkUserValid();
    bool checkPwdValid();
    void initHttpHandlers();
    void showTip(QString str, bool b_ok);
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    bool enableBtn(bool enabled);
    Ui::LoginDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    QMap<TipErr, QString> _tip_errs;

private slots:
    void slot_forget_pwd();
    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes err);
    void on_login_btn_clicked();

signals:
    void switchRegister();
    void switchReset();
    void sig_connect_tcp(ServerInfo);
};

#endif // LOGINDIALOG_H
