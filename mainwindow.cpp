#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QByteArray>
#include <QMessageBox>
#include <QHostAddress>
#include <QSysInfo>
#include <QNetworkInterface>

#include <string>

#include "types.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setMinimumSize(500, 50);
    ui->logo_label->setStyleSheet("image: url(:/img/logo.png);");
    this->resize(800, 600);
    this->setFixedSize(this->width(), this->height());

    ui->username->setFocus();

    ui->textEdit->setVisible(false);

    //network
    tcp_socket = new QTcpSocket(this);

    connect(tcp_socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(tcp_socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(tcp_socket, &QTcpSocket::errorOccurred, this, &MainWindow::onError);
    connect(tcp_socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);

    ip = "127.0.0.1";
    port = 1234;

    //os
    qDebug() << "os:" << QSysInfo::productType();

    _os = QSysInfo::productType();

    //crc_hash first_packet platform
    if (_os == "macos")
    {
        checkSumSeed = macCheckSumSeed;
        platform = "XSO";

        // checkSumSeed = winCheckSumSeed;
        // platform = "niW";
    }
    else if (_os == "win")
    {
        checkSumSeed = winCheckSumSeed;
        platform = "niW";
    }

    qDebug() << "platform:" << platform;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tcp_socket;
}

//登录操作
void MainWindow::on_logon_clicked()
{
    QString username = ui->username->text();
    QString passwd = ui->passwd->text();

    qDebug() << "点击了登录按钮";
    qDebug() << "用户名:" << username;
    qDebug() << "密码:" << passwd;

    if (username.isEmpty())
    {
        QMessageBox::information(this, "错误", "用户名不能为空");
        ui->username->setFocus();
        return ;
    }

    if (passwd.isEmpty())
    {
        QMessageBox::information(this, "错误", "密码不能为空");
        ui->passwd->setFocus();
        return ;
    }

    if (tcp_socket->isOpen())
    {
        tcp_socket->disconnectFromHost();
    }

    tcp_socket->connectToHost(ip, port);

    HandlerSend(tcp_socket, AUTH_LOGON_CHALLENGE);
}

void MainWindow::onConnected()
{
    qDebug() << "connected";
}

void MainWindow::onReadyRead()
{
    QByteArray inComingData = tcp_socket->readAll();
    qDebug() << "recv:" << inComingData;

    if (inComingData.size() < 2)
    {
        qDebug() << "error packet size:" << inComingData.size();
        return ;
    }

    uint8 cmd = inComingData.at(0);

    switch (cmd)
    {
    case AUTH_LOGON_CHALLENGE://logon challenge
        {
            //error
            uint8 error = inComingData.at(2);

            switch (error)
            {
            case WOW_FAIL_UNKNOWN_ACCOUNT:
                qDebug() << "未知账户";
                QMessageBox::critical(this, "错误", "未知账户");
                break;
            case WOW_FAIL_LOCKED_ENFORCED:
                qDebug() << "locked to IP";
                break;
            case WOW_FAIL_UNLOCKABLE_LOCK:
                qDebug() << "locked to country";
                break;
            case WOW_FAIL_BANNED:
                qDebug() << "Banned account";
                break;
            case WOW_FAIL_SUSPENDED:
                qDebug() << "Temporarily banned account";
                break;
            case WOW_FAIL_DB_BUSY:
                qDebug() << "invalid ciphertext for TOTP token key stored";
                break;
            case WOW_SUCCESS:
            {
                qDebug() << "WOW_SUCCESS" << inComingData.size() << sizeof(sAuthLogonChallenge_S);

                if (inComingData.size() < sizeof(sAuthLogonChallenge_S))
                {
                    qDebug() << "返回数据有误";
                    QMessageBox::critical(this, "错误", "返回数据有误");
                    return ;
                }

                sAuthLogonChallenge_S first_packet_s;
                std::memcpy(&first_packet_s, inComingData.constData(), sizeof(sAuthLogonChallenge_S));

                qDebug() << "first recv packet";

                BigNum B(reinterpret_cast<const unsigned char *>(first_packet_s.B), 32);
                BigNum g(reinterpret_cast<const unsigned char *>(first_packet_s.g), 1);
                BigNum N(reinterpret_cast<const unsigned char *>(first_packet_s.N), 32);
                BigNum s(reinterpret_cast<const unsigned char *>(first_packet_s.s), 32);                

                srp6_caculate client(N, g, s, B, ui->username->text().toLocal8Bit().constData(), ui->passwd->text().toLocal8Bit().constData());
                A = client.A;
                K = client.calculate_session_key();
                QMessageBox::information(this, "ok", "客户端session_key已保存");
                ui->textEdit->append(QString("客户端session_key:%1").arg(K.to_hex()));

                M1 = client.calculate_proof();
                unsigned char * m2_tmp = client.calculate_M2();

                M2 = std::string((const char *)m2_tmp, 20);

                ui->textEdit->append(QString("客户端M2:%1").arg(client.M2.to_hex()));

                delete [] m2_tmp;

                std::array<uint8, 20> out;
                HexStrToByteArray(checkSumSeed, out.data(), 20);
                unsigned char * crc_hash_tmp = client.calculate_crc_hash(std::string((const char *)out.data(), 20));
                CRC_HASH = std::string((const char *)crc_hash_tmp, 20);

                delete [] crc_hash_tmp;

                qDebug() << "A:" << std::string((const char *)A.to_bin(32), 32);
                qDebug() << "A:" << A.as_byte_array(0, false);

                HandlerSend(tcp_socket, AUTH_LOGON_PROOF);
            }
            break;
            }
        }
        break;
    case AUTH_LOGON_PROOF:
        uint8 error = inComingData.at(1);
        switch (error)
        {
        case WOW_FAIL_UNKNOWN_ACCOUNT:
            qDebug() << "WOW_FAIL_UNKNOWN_ACCOUNT_PASSWD_ERROR";
            QMessageBox::critical(this, "错误", "用户密码错误");
            ui->passwd->setFocus();
            break;
        case WOW_FAIL_VERSION_INVALID:
            qDebug() << "WOW_FAIL_VERSION_INVALID";
            QMessageBox::critical(this, "错误", "客户端版本错误");
            break;
        case WOW_SUCCESS:
            {
                sAuthLogonProof_S second_packet_s;
                qDebug() << "AUTH_LOGON_PROOF:WOW_SUCCESS" << inComingData.size() << sizeof(sAuthLogonProof_S);

                std::memcpy(&second_packet_s, inComingData.constData(), sizeof(sAuthLogonProof_S));
                std::string packet_proof_m2 = std::string((const char *)second_packet_s.M2, 20);

                qDebug() << "second_packet_s.cmd:" << second_packet_s.cmd;
                qDebug() << "second_packet_s.M2:" << std::string((const char *)second_packet_s.M2, 20);
                qDebug() << "client->M2:" << M2;

                if (M2 == packet_proof_m2)
                {
                    qDebug() << "客户端M2校验成功";
                    QMessageBox::information(this, "ok", "客户端M2校验成功");
                }
            }

            break;
        }
        break;
    }
}

void MainWindow::onError()
{
    qDebug() << "socket发生错误" << tcp_socket->errorString();
    tcp_socket->disconnectFromHost();
}

bool MainWindow::HandlerSend(QTcpSocket* socket, uint8 process)
{
    QByteArray send_data;
    QDataStream out(&send_data, QIODevice::WriteOnly);

    QString username = ui->username->text();
    uint32 I_len = username.length();
    uint32 total_length = 0;

    sAuthLogonChallenge_C* first_packet = nullptr;
    sAuthLogonProof_C second_packet;
    QString ip_addr = getLocalIpAddress();

    switch (process)
    {
    case AUTH_LOGON_CHALLENGE:
        qDebug() << "send first struct";

        total_length = sizeof(sAuthLogonChallenge_C) - 1 + I_len;

        qDebug() << "I_len" << I_len;
        qDebug() << "total_length:" << total_length;

        first_packet = (sAuthLogonChallenge_C*)new char[total_length];

        //construct packet
        //header
        first_packet->cmd = AUTH_LOGON_CHALLENGE;
        first_packet->error = 0;
        first_packet->size = total_length - AUTH_LOGON_CHALLENGE_INITIAL_SIZE;
        //body
        std::strncpy((char *)first_packet->gamename, "WoW", 3);
        first_packet->version1 = 3;
        first_packet->version2 = 3;
        first_packet->version3 = 5;
        first_packet->build = 12340;
        std::strncpy((char *)first_packet->platform, "68x", 3);
        std::strncpy((char *)first_packet->os, platform.c_str(), 3);
        std::strncpy((char *)first_packet->country, "NChz", 4);
        first_packet->timezone_bias = 0;
        first_packet->ip = this->IpToInt((const char *)ip_addr.toLocal8Bit().data());
        first_packet->I_len = I_len;
        std::strncpy((char *)first_packet->I, username.toLocal8Bit().constData(), username.length());

        // out.setByteOrder(QDataStream::LittleEndian);
        out.writeRawData((const char *)first_packet, total_length);

        delete [] first_packet;
        break;
    case AUTH_LOGON_PROOF:
        {
            second_packet.cmd = AUTH_LOGON_PROOF;
            std::memcpy((char *)second_packet.A, A.as_byte_array(0, true).data(), 32);
            std::memcpy((char *)second_packet.M1, M1.as_byte_array(0, true).data(), 20);

            //calculate crc_hash
            // std::vector<uint8> crc_hash = {
            //     0xCD, 0xCB, 0xBD, 0x51, 0x88, 0x31, 0x5E, 0x6B,
            //     0x4D, 0x19, 0x44, 0x9D, 0x49, 0x2D, 0xBC, 0xFA,
            //     0xF1, 0x56, 0xA3, 0x47
            // };

            qDebug() << "crc_has size:" << CRC_HASH.size() << "crc_hash content:" << CRC_HASH;

            std::memcpy((char *)second_packet.crc_hash, CRC_HASH.data(), 20);

            second_packet.num_of_keys = 0;
            second_packet.security_Falgs = 0;

            out.writeRawData((const char *)&second_packet, sizeof(second_packet));
        }
        break;
    }

    quint64 send_bytes = socket->write(send_data);

    qDebug() << "send_bytes:" << send_bytes;

    return true;
}

uint32 MainWindow::IpToInt(const QString &ipAddress)
{
    QHostAddress address(ipAddress);
    return address.toIPv4Address();
}

void MainWindow::onDisconnected()
{
    qDebug() << "void onDisconnected()";
}

QString MainWindow::getLocalIpAddress() {
    QString localIP;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for (int i = 0; i < list.size(); ++i) {
        QHostAddress address = list.at(i);
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            localIP = address.toString();
            break;
        }
    }
    return localIP;
}

void MainWindow::HexStrToByteArray(std::string_view str, uint8* out, size_t outlen, bool reverse /*= false*/)
{
    if(str.size() != (2 * outlen))
    {
        return ;
    }

    int32 init = 0;
    int32 end = int32(str.length());
    int8 op = 1;

    if (reverse)
    {
        init = int32(str.length() - 2);
        end = -2;
        op = -1;
    }

    uint32 j = 0;
    for (int32 i = init; i != end; i += 2 * op)
    {
        char buffer[3] = { str[i], str[i + 1], '\0' };
        out[j++] = uint8(strtoul(buffer, nullptr, 16));
    }
}

void MainWindow::on_showLog_clicked()
{
    if (showLog == false)
    {
        showLog = true;
        ui->textEdit->setVisible(true);
    }
    else
    {
        showLog = false;
        ui->textEdit->setVisible(false);
    }
}

