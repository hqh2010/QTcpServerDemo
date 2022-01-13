#include "unixsocket.h"
#include <stdio.h>

UnixSocket::UnixSocket(QObject *parent) :
    QObject(parent)
{
    this->socket = new QLocalSocket (this);
}

UnixSocket::~UnixSocket(){}

void UnixSocket::Test ()
{
    connect (this->socket, SIGNAL(connected()), this, SLOT(connected_callback()));
    connect (this->socket, SIGNAL(disconnected()), this, SLOT(disconnected_callback()));
    connect (this->socket, SIGNAL(readyRead()), this, SLOT(readyRead_callback()));
    connect (this->socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten_callback(qint64)));

    qInfo() << "Connecting...";

    socket->connectToServer(ADAPTER_SOCKET);

    if (!socket->waitForConnected(1000))
    {
        qInfo() << "Error: " << this->socket->errorString();
    }
    else
    {
        in.setDevice(socket);
        in.setVersion(QDataStream::Qt_5_8);
        this->blockSize = 0;

    }

}

void UnixSocket::connected_callback()
{
    qInfo() << "Connected to " << socket->fullServerName();

}

void UnixSocket::disconnected_callback()
{
    qInfo() << "Disconnected!";
}

void UnixSocket::bytesWritten_callback(qint64 bytes)
{
    qInfo() << "We wrote: " << bytes;
}

void UnixSocket::readyRead_callback()
{
    qInfo() << "Reading...";
    qInfo() << "Block Size: " << this->blockSize;

    if (this->blockSize == 0) {
        // Relies on the fact that QDataStream serializes a quint32 into
        // sizeof(quint32) bytes
        if (socket->bytesAvailable() < (int)sizeof(quint32))
            return;
        in >> this->blockSize;
    }

    if (socket->bytesAvailable() < this->blockSize || in.atEnd())
        return;

    QString recvd_data;
    in >> recvd_data;
    qInfo() << recvd_data;
}
