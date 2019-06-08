//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include <QCoreApplication>
#include <QDebug>
#include "TCPServer.h"

/// <summary>
/// Main entry of the program obviously.
/// </summary>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("LogMeInServer");

    auto server = new TCPServer();

    if (!server->LoadSipRegistrations())
        qWarning("Cannot load SIP registrations. The server won't start.");
    else
    {
        if (!server->Start())
            qWarning("Cannot start server. Please close the program and retry.");
        else
            qInfo("Server started. Listening on port 8888");
    }

    return a.exec();
}
