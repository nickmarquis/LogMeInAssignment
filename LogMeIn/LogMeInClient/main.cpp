//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include <QtCore/QCoreApplication>
#include <QHostAddress>
#include <QThread>
#include "ClientApp.h"

/// <summary>
/// Main entry of the program obviously.
/// </summary>
int main(int argc, char *argv[])
{
    qRegisterMetaType<QAbstractSocket::SocketState>();
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("LogMeInClient");

    auto app = new ClientApp();
    app->start();

    qInfo() << "Quitting...";

    return a.exec();
}
