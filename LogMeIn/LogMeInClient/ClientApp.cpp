//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include "ClientApp.h"
#include <QThread>

/// <summary>
/// Constructor
/// </summary>
ClientApp::ClientApp(QObject *parent) : QObject(parent),
m_tcpClient(new TCPClient())
{
    auto thread = new QThread(this);
    m_tcpClient->moveToThread(thread);

    connect(this, &ClientApp::query, m_tcpClient, &TCPClient::query);
    connect(this, &ClientApp::connectToHost, m_tcpClient, &TCPClient::connectToHost);
    connect(thread, &QThread::started, m_tcpClient, &TCPClient::connectToHost);

    thread->start();
}

/// <summary>
/// Destructor
/// </summary>
ClientApp::~ClientApp()
{
    m_tcpClient->deleteLater();
}

/// <summary>
/// Start the main loop to read input from user.
/// </summary>
void ClientApp::start()
{
    while(true)
    {
        // Give time for the server to reply
        QThread::sleep(1);

        qInfo() << "\nEnter the address of record that you're looking for:";
        QTextStream s(stdin);
        auto aor = s.readLine();

        if(!m_tcpClient->isConnected())
        {
            qInfo() << "\nYou have been disconnected. Would you like to reconnect (Y/N): ";
            auto retry = s.readLine();
            if (retry.compare("Y", Qt::CaseSensitivity::CaseInsensitive) != 0)
                return;

            emit connectToHost();
        }
        else
            emit query(aor);
    }
}
