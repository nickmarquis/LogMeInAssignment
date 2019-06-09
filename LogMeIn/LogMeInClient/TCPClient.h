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
    TCPClient(QObject *parent = nullptr);
    ~TCPClient() = default;

    bool isConnected() { return m_socket->state() == QAbstractSocket::ConnectedState; }

private slots:
    void onReadyRead();

public slots:
    bool connectToHost();
    void query(QString addressOfRecord);

private:
    QTcpSocket*         m_socket;
};

#endif // TCPCLIENT_H
