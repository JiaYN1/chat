#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 连接按钮点击事件
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchResiter);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
