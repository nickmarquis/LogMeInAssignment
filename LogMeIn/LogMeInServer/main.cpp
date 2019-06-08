#include <QCoreApplication>
#include <QDebug>
#include "TCPServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("LogMeInServer");

    qInfo("Initializing server database...");
    auto server = new TCPServer();

    if (!server->LoadSipRegistrations())
        qWarning("Cannot load SIP registrations. The server won't start.");

    qInfo("Starting TCP server...");
    if (!server->Start())
        qWarning("Cannot start server. Please retry.");
    else
        qInfo("Server started. Listening on port 8888");

    return a.exec();
}
