#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    QHostAddress hostAddr = QHostAddress("117.110.47.117");
    if (mTcpServer->isListening())
    {
        mTcpServer->close();
    }
    if (!mTcpServer->listen(QHostAddress::Any, 9000))
    {
        qCritical() << "Server start failed";
    }
    else
    {
        qInfo() << "Server started";
    }
}

void TcpServer::newConnection()
{
    QTcpSocket *clientSocket = mTcpServer->nextPendingConnection();
    if (!clientSocket)
    {
        qCritical() << "no clientSocket connect";
        return;
    }

    connect(clientSocket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    // 客户端断开时触发
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    // 注意信号与slot签名要一致
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
    connect(clientSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(onErrorOccurred(QAbstractSocket::SocketError)));

    clientSocket->write("Hello client\r\n");
    clientSocket->flush();
    clientSocket->waitForBytesWritten(3000);
    // 关闭之后无法接收数据
    // clientSocket->close();
}

void TcpServer::onSocketConnected() {

    QTcpSocket *sender = static_cast<QTcpSocket *>(QObject::sender());

    QString clientInfo = QString("[%1:%2] client connect")
                          .arg(sender->peerAddress().toString())
                          .arg(sender->peerPort());
    qInfo() << clientInfo;

}

void TcpServer::onReadyRead()
{
    QTcpSocket *sender = static_cast<QTcpSocket *>(QObject::sender());
    QByteArray data = sender->readAll();
    qInfo() << "receive client data:" << data;
}

void TcpServer::onDisconnected()
{
    QTcpSocket *sender = static_cast<QTcpSocket *>(QObject::sender());
    QString clientInfo = QString("[%1:%2] onDisconnected").arg(sender->peerAddress().toString()).arg(sender->peerPort());
    qInfo() << clientInfo;

    sender->deleteLater();
}

// Constant	Value	Description
// QAbstractSocket::UnconnectedState	0	The socket is not connected.
// QAbstractSocket::HostLookupState	1	The socket is performing a host name lookup.
// QAbstractSocket::ConnectingState	2	The socket has started establishing a connection.
// QAbstractSocket::ConnectedState	3	A connection is established.
// QAbstractSocket::BoundState	4	The socket is bound to an address and port.
// QAbstractSocket::ClosingState	6	The socket is about to close (data may still be waiting to be written).
// QAbstractSocket::ListeningState	5	For internal use only.
void TcpServer::onStateChanged(QAbstractSocket::SocketState state)
{
    qInfo() << "TcpSocketState------" << state;
    switch (state)
    {
    case QAbstractSocket::ConnectedState:
    case QAbstractSocket::ConnectingState:
    case QAbstractSocket::ClosingState:
        break;
    case QAbstractSocket::UnconnectedState:
        break;
    default:
        break;
    }
}

void TcpServer::onErrorOccurred(QAbstractSocket::SocketError err)
{
    QTcpSocket *sender = static_cast<QTcpSocket *>(QObject::sender());

    QString errInfo = QString("[%1:%2] client Error:%3")
                          .arg(sender->peerAddress().toString())
                          .arg(sender->peerPort())
                          .arg(err);
    qCritical() << errInfo;
}
