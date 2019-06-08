//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QMap>

/// <summary>
/// TCPServer running on port 8888.
/// It reply with a SIP when a address of records is transmitted from a client.
/// </summary>
class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = nullptr);
    ~TCPServer() = default;
    bool LoadSipRegistrations();
    bool Start() const;

private slots:
    void OnNewConnection();
    void OnSocketStateChanged(QAbstractSocket::SocketState socketState);
    void OnReadyRead();

signals:
    void QueryAndSendResult(QTcpSocket* socket, QString aor);

private:
    void OnQueryAndSendResult(QTcpSocket* socket, QString aor);
    int ReadBufferSize(QByteArray source);

    QHash<QString, QString>         m_records;
    QTcpServer*                     m_server;
    QHash<QTcpSocket*, QByteArray*> m_dataBuffers;
    QHash<QTcpSocket*, qint32*>     m_bufferSizes;
};

#endif // TCPSERVER_H
