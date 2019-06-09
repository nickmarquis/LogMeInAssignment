//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTimer>
#include <QMap>

/// <summary>
/// TCPServer running on port 8888.
/// It reply with a SIP when a address of records is transmitted from a client.
/// </summary>
class TCPServer : public QObject
{
    Q_OBJECT
public:
    TCPServer(QObject *parent = nullptr);
    ~TCPServer();
    bool loadSipRegistrations();
    bool start() const;

public slots:
    void onNewConnection();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();
    void onCloseConnection();
    void onQueryAndSendResult(QTcpSocket* socket, QString aor);

signals:
    void queryAndSendResult(QTcpSocket* socket, QString aor);

private:
    int readBufferSize(QByteArray source);

    QHash<QString, QString>         m_records;
    QTcpServer*                     m_server;
    QHash<QTcpSocket*, QByteArray*> m_dataBuffers;
    QHash<QTcpSocket*, qint32*>     m_bufferSizes;
    QHash<QTimer*, QTcpSocket*>     m_socketTimers;
};

#endif // TCPSERVER_H
