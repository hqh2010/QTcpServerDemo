#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>

#include "cserver.h"
#include "unixsocket.h"

#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // // then ask the bus to send us a copy of each Notify call message
    // // https://dbus.freedesktop.org/doc/dbus-specification.html#message-bus-routing-match-rules
    // QString matchString = "interface='com.deepin.linglong.PackageManager',member='test',type='method_call',eavesdrop='true'";
    // QDBusInterface busInterface("org.freedesktop.DBus", "/org/freedesktop/DBus","org.freedesktop.DBus", QDBusConnection::sessionBus());
    // busInterface.call("AddMatch", matchString);
    // qInfo() << "Monitor proxy main start1";

    //CServer server;
    //server.RunServer();
    // UnixSocket mUnixSocket;
    // mUnixSocket.Test();

    TcpServer tcpServer;
    return a.exec();
}
