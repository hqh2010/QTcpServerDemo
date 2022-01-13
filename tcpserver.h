#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QAbstractSocket>
#include <QDebug>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>

class TcpServer : public QObject
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer()
    {
        if (mTcpServer != nullptr)
        {
            delete mTcpServer;
        }
    }

public slots:
    void newConnection();
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError);
    void onStateChanged(QAbstractSocket::SocketState state);
    void onSocketConnected();

private:
    QTcpServer *mTcpServer;
};

#endif // TCPSERVER_H
