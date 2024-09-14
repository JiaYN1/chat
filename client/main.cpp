#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(":/style/stylesheet.qss");

    if(qss.open(QFile::ReadOnly))
    {
        qDebug("Open Success!");
        QString style = QLatin1String(qss.readAll());  // readAll return byteArray
        a.setStyleSheet(style);
        qss.close();
    }
    else{
        qDebug("Open Failed!");
    }

    QString appPath = QCoreApplication::applicationDirPath();
    QString fileName = "config.ini";
    QString configPath = QDir::toNativeSeparators(appPath + QDir::separator() + fileName);

    QSettings settings(configPath, QSettings::IniFormat);
    QString gateHost = settings.value("GateServer/host").toString();
    QString gatePort = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://"+gateHost+":"+gatePort;

    MainWindow w;
    w.show();
    return a.exec();
}
