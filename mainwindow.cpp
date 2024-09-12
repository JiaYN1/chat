#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dlg = new LoginDialog();
    setCentralWidget(_login_dlg);
    _login_dlg->show();

    /*
     * 创建和注册消息的连接
     * QPushButton::clicked -> LoginDialog::switchRegister -> MainWindow::SlotSwitchReg
     */
    connect(_login_dlg, &LoginDialog::switchResiter, this, &MainWindow::SlotSwitchReg);
    _reg_dlg = new RegisterDialog();

}

MainWindow::~MainWindow()
{
    delete ui;
    // release
    if(_login_dlg){
        delete _login_dlg;
        _login_dlg = nullptr;
    }

    if(_reg_dlg){
        delete _reg_dlg;
        _reg_dlg = nullptr;
    }
}

void MainWindow::SlotSwitchReg()
{
    setCentralWidget(_reg_dlg);
    _login_dlg->hide();
    _reg_dlg->show();
}
