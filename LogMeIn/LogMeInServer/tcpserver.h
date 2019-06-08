#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QMap>

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

private:
    void QueryAndSendResult(QTcpSocket* socket, QString aor);
    int ReadBufferSize(QByteArray source);

    QMap<QString, QString>          m_records;
    QTcpServer*                     m_server;
    QHash<QTcpSocket*, QByteArray*> m_dataBuffers;
    QHash<QTcpSocket*, qint32*>     m_bufferSizes;
};

#endif // TCPSERVER_H
