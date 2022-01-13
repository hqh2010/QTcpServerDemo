#ifndef CSERVER_H
#define CSERVER_H

#include <QDebug>
#include <QObject>
#include <QLocalSocket>
#include <QLocalServer>

class CServer : public QObject
{
    Q_OBJECT

public:
    CServer() : m_server(NULL)
    {
        m_server = new QLocalServer;
        connect(m_server, SIGNAL(newConnection()), SLOT(serverNewConnectionHandler()));
        qInfo() << "listen server newConnection";
    }

    ~CServer()
    {
        m_server->close();
        delete m_server;
    }

    void RunServer()
    {
        qInfo() << "Run Server ok";

        QLocalServer::removeServer("/run/user/1000/.dbus-proxy/session-bus-proxy-OARBsq");
        m_server->setSocketOptions(QLocalServer::WorldAccessOption);
        bool ok = m_server->listen("/run/user/1000/.dbus-proxy/session-bus-proxy-OARBsq");
        if (!ok)
        {
               qInfo() << "listen server error";
        }
    }

private slots:
    void serverNewConnectionHandler()
    {
        qInfo() << "New Connection";
        qInfo() << "listen: " << m_server->serverName();

        QLocalSocket* socket = m_server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyReadHandler()));
        connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    }

    void socketReadyReadHandler()
    {
        QLocalSocket* socket = static_cast<QLocalSocket*>(sender());
        if (socket)
        {
            QTextStream stream(socket);
            qInfo() << "Read Data From Client:" << stream.readAll();

            QString response = "Hello Client";
            socket->write(response.toUtf8());
            socket->flush();
        }
        // 返回到客户端的void sendMessage 方法，m_socket->waitForReadyRead()之后的操作
    }

private:
    QLocalServer *m_server;

};

#endif // CSERVER_H

