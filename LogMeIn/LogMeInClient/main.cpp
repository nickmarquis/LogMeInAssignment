//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include <QtCore/QCoreApplication>
#include <QHostAddress>
#include <QThread>
#include "TCPClient.h"

/// <summary>
/// Main entry of the program obviously.
/// </summary>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("LogMeInClient");

    auto tcpClient = new TCPClient();

    bool connected = false;
    if (tcpClient->connectToHost())
        connected = true;

    while(connected)
    {
        // Give time for the server to reply
        QThread::sleep(1);

        qInfo() << "\nEnter the address of record that you're looking for:";
        QTextStream s(stdin);
        auto aor = s.readLine();

        QCoreApplication::instance()->processEvents(QEventLoop::AllEvents);
        if(!tcpClient->isConnected())
        {
            qInfo() << "\nYou have been disconnected. Would you like to reconnect and retry (Y/N): ";
            auto retry = s.readLine();
            if (retry.compare("Y", Qt::CaseSensitivity::CaseInsensitive) != 0)
            {
                qInfo() << "Quitting...";
                QThread::sleep(2);
                return 0;
            }
            else if (!tcpClient->connectToHost())
            {
                qWarning() << "Enable to reconnect. Quitting...";
                QThread::sleep(2);
                return 0;
            }
        }
        if (!tcpClient->query(aor))
            qWarning() << "Enable to query AOR:" << aor;
    }

    return a.exec();
}
