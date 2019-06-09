#ifndef CLIENTAPP_H
#define CLIENTAPP_H

//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include <QObject>
#include "TCPClient.h"

/// <summary>
/// Client App to take user input and move the socket to another thread.
/// </summary>
class ClientApp : public QObject
{
    Q_OBJECT
public:
    ClientApp(QObject *parent = nullptr);
    ~ClientApp();
    void start();

signals:
    void query(QString addressOfRecord);
    void connectToHost();

private:
    TCPClient*      m_tcpClient;
};

#endif // CLIENTAPP_H
