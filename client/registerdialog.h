#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr); // 防止构造函数被隐式地用于类型转换
    ~RegisterDialog();

private slots:
    void on_get_btn_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_cofirm_btn_clicked();
    void on_return_btn_clicked();
    void on_cancel_btn_clicked();

private:
    void initHttpHandlers();
    void showTip(QString str, bool b_ok);
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkVerifyValid();
    bool checkConfirmValid();
    void ChangeTipPage();
    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;  //引用减少参数传递开销
    QMap<TipErr, QString> _tip_errs;
    QTimer* _countdown_timer;
    int _counter;
signals:
    void sigSwitchLogin();
};

#endif // REGISTERDIALOG_H
