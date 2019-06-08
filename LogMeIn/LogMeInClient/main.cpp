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

    auto connected = false;
    auto tcpClient = new TCPClient();
    // will need to get that from command line arg
    if (tcpClient->ConnectToHost(QHostAddress("127.0.0.1")))
        connected = true;

    tcpClient->ReadData();
    while(connected)
    {
        //if (tcpClient->isQuerying())
        //{
        //    // sleep 2s
        //    QThread::sleep(2);
        //    continue;
        //}
            
        qInfo() << "Enter the address of record that you're looking for:";
        QTextStream s(stdin);
        auto aor = s.readLine();
        s.flush();

        if(!tcpClient->Query(aor))
        {
            qInfo() << "You have been disconnected. Would you like to reconnect and retry (Y/N): ";
            auto retry = s.readLine();
            if (!retry.compare("Y", Qt::CaseSensitivity::CaseInsensitive))
            {
                qInfo() << "Quitting...";
                break;
            }
            if (!tcpClient->ConnectToHost(QHostAddress("127.0.0.1")) || !tcpClient->Query(aor))
            {
                qWarning() << "Enable to reconnect and send the query. Quitting...";
                break;
            }                          
        }
    }

    return a.exec();
}
