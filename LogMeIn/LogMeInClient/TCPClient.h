//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------
#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

/// <summary>
/// Client use to connect to a server by TCPSocket and Query SIP.
/// </summary>
class TCPClient : public QObject
{
    Q_OBJECT
public:
    TCPClient();
    ~TCPClient() { m_socket->deleteLater(); }

    bool isConnected() { return m_socket->waitForConnected(500); }
    bool query(QString addressOfRecord);
    void readData();

public slots:
    bool connectToHost();

private:
    QTcpSocket*         m_socket;
};

#endif // TCPCLIENT_H
