#include "tcpserver.h"
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QJsonObject>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDataStream>
#include <cassert>

TCPServer::TCPServer(QObject *parent) : QObject(parent),
    m_server(new QTcpServer()) {}

bool TCPServer::LoadSipRegistrations()
{
    qDebug() << "Reading and parsing SIPs file...";

    QFile sipFile(":/regs.txt");
    if (!sipFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't find SIP file: " << sipFile.errorString();
        return false;
    }

    QTextStream in(&sipFile);
    while (!in.atEnd())
    {
        auto sip = in.readLine();
        auto sipJson = QJsonDocument::fromJson(sip.toUtf8()).object();

        // assert if an entry doesn't contains an address of record.
        assert(sipJson.keys().contains("addressOfRecord"));
        m_records.insert(sipJson.value("addressOfRecord").toString(), sip);
    }
    sipFile.close();

    // assert if the sip file was empty.
    assert(!m_records.isEmpty());

    return true;
}

bool TCPServer::Start() const
{
    connect(m_server, &QTcpServer::newConnection, this, &TCPServer::OnNewConnection);
    return m_server->listen(QHostAddress::Any, 8888);
}

void TCPServer::OnNewConnection()
{
    while (m_server->hasPendingConnections())
    {
        auto clientSocket = m_server->nextPendingConnection();
        connect(clientSocket, &QTcpSocket::readyRead, this, &TCPServer::OnReadyRead);
        connect(clientSocket, &QTcpSocket::stateChanged, this, &TCPServer::OnSocketStateChanged);

        qInfo() << "New client connected with: " << clientSocket->peerAddress().toString();
        clientSocket->write(QByteArray::fromStdString("Connected to host!"));
        clientSocket->waitForBytesWritten();

        auto buffer = new QByteArray();
        auto s = new qint32(0);
        m_dataBuffers.insert(clientSocket, buffer);
        m_bufferSizes.insert(clientSocket, s);
    }
}

void TCPServer::OnSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        auto sender = static_cast<QTcpSocket*>(QObject::sender());
        qInfo() << "Client " <<  sender->peerAddress().toString() << "has just been disconnected";

        auto buffer = m_dataBuffers.value(sender);
        auto size = m_bufferSizes.value(sender);
        sender->deleteLater();
        delete buffer;
        delete size;
    }
}

void TCPServer::OnReadyRead()
{
    auto sender = static_cast<QTcpSocket*>(QObject::sender());
    auto buffer = m_dataBuffers.value(sender);
    auto s = m_bufferSizes.value(sender);
    auto size = *s;
    while (sender->bytesAvailable() > 0)
    {
        buffer->append(sender->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size))
        {
            if (size == 0 && buffer->size() >= 4)
            {
                size = ReadBufferSize(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size)
            {
                auto data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                QueryAndSendResult(sender, QString(data));
            }
        }
    }
}

void TCPServer::QueryAndSendResult(QTcpSocket* socket, QString aor)
{
    auto sip = m_records.value(aor);
    socket->write(QByteArray::fromStdString(sip.toStdString()));
    socket->waitForBytesWritten();
}

int TCPServer::ReadBufferSize(QByteArray source)
{
    int temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}
