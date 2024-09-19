#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QMutex>

static QMutex mutex;

void customeMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //加载样式表
    QFile file(":/css/index.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        file.close();
    }

    //注册消息处理函数
    // qInstallMessageHandler(customeMessageHandler);

    MainWindow w;
    w.show();
    return a.exec();
}


//将qDebug()日志重定向到文件
void customeMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString txt(date);

    switch (type)
    {
    case QtDebugMsg:
        txt += QString(" [debug] ");
        break;
    case QtInfoMsg:
        txt += QString(" [info] ");
        break;
    case QtWarningMsg:
        txt += QString(" [warning] ");
        break;
    case QtCriticalMsg:
        txt += QString(" [critical] ");
        break;
    case QtFatalMsg:
        txt += QString(" [fatal] ");
        break;
    default:
        txt += QString(" [trace] ");
        break;
    }

    txt.append(QString(" %1<%2> %3").arg(context.file).arg(context.line).arg(msg));

    mutex.lock();
    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream textStream(&file);
    textStream << txt << "\r\n";
    file.close();
    mutex.unlock();
}
