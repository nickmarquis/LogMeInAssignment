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
    ~TCPClient() = default;

    bool ConnectToHost(QHostAddress host);
    bool Query(QString addressOfRecord);
    bool isQuerying() const { return m_isQuerying; }
    void ReadData();

private:
    QTcpSocket*         m_socket;
    bool                m_isQuerying;
};

#endif // TCPCLIENT_H
