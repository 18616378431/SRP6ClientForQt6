#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>

#include <cstring>
#include <arpa/inet.h>

#include <iostream>
#include <iomanip>

#include "libsrp6.h"
#include "types.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void printHex(const unsigned char* data, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            // 使用std::hex和std::setw来格式化输出
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i] << " ";
        }

        std::cout << std::endl;
    }

private slots:
    void on_logon_clicked();
    void onConnected();
    void onReadyRead();
    void onError();
    void onDisconnected();
    void on_showLog_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket* tcp_socket;//socket
    QString ip;
    quint16 port;
    BigNum A, M1, K;
    std::string  CRC_HASH, M2;
    std::string winCheckSumSeed = "CDCBBD5188315E6B4D19449D492DBCFAF156A347";
    std::string macCheckSumSeed = "B706D13FF2F4018839729461E3F8A0E2B5FDC034";
    std::string checkSumSeed;
    QString _os;
    std::string platform;
    bool showLog = false;

    bool HandlerSend(QTcpSocket* socket, uint8 process);
    uint32 IpToInt(const QString &ipAddress);
    QString getLocalIpAddress();
    void HexStrToByteArray(std::string_view str, uint8* out, size_t outlen, bool reverse = false);
};
#endif // MAINWINDOW_H
