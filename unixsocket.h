#ifndef UNIXSOCKET_H
#define UNIXSOCKET_H

#include <QObject>
#include <QAbstractSocket>
#include <QLocalSocket>
#include <QtNetwork>
#include <QIODevice>
#include <QDebug>
#include <QTextStream>
#include <QDataStream>

#define ADAPTER_SOCKET "/run/user/1000/.dbus-proxy/session-bus-proxy-HJTGF1"


class UnixSocket: public QObject
{
    Q_OBJECT

public:
    explicit UnixSocket(QObject *parent = 0);
    ~UnixSocket();
    void Test ();

signals:

public slots:

    void connected_callback();
    void disconnected_callback();
    void bytesWritten_callback (qint64 bytes);
    void readyRead_callback();

private:
    QLocalSocket *socket;
    QDataStream in;
    quint32 blockSize;
};

#endif // UNIXSOCKET_H
