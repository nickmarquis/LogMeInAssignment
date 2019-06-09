//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include "TCPServer.h"
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QJsonObject>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDataStream>
#include <QTimer>
#include <cassert>

Q_DECLARE_METATYPE(QHostAddress);

/// <summary>
/// Constructor
/// </summary>
TCPServer::TCPServer(QObject *parent) : QObject(parent),
    m_server(new QTcpServer()) {}

/// <summary>
/// Destructor
/// </summary>
TCPServer::~TCPServer()
{
    m_server->deleteLater();
    qDeleteAll(m_bufferSizes);
    qDeleteAll(m_dataBuffers);
    qDeleteAll(m_socketTimers);
}

/// <summary>
/// Load SIP registrations from a file and store it in a map with the AOR as the key.
/// </summary>
bool TCPServer::loadSipRegistrations()
{
    qDebug() << "Reading and parsing SIPs file...";

    // file is in the qrc
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

/// <summary>
/// Start the server to listen on any IPV4 and IPV6 address on port 8888
/// </summary>
bool TCPServer::start() const
{
    qInfo("Starting TCP server...");
    connect(m_server, &QTcpServer::newConnection, this, &TCPServer::onNewConnection);
    return m_server->listen(QHostAddress::Any, 8888);
}

/// <summary>
/// Event when a new connection arrive. It can handle multiple connections.
/// Store the socket and a buffer for it in a HashMap.
/// </summary>
void TCPServer::onNewConnection()
{
    while (m_server->hasPendingConnections())
    {
        auto clientSocket = m_server->nextPendingConnection();
        connect(clientSocket, &QTcpSocket::readyRead, this, &TCPServer::onReadyRead);
        connect(clientSocket, &QTcpSocket::stateChanged, this, &TCPServer::onSocketStateChanged);

        qInfo() << "New client connected: " << clientSocket->peerAddress().toString();
        clientSocket->write(QByteArray::fromStdString("Connected to host!"));
        clientSocket->waitForBytesWritten();

        auto buffer = new QByteArray();
        auto s = new qint32(0);
        m_dataBuffers.insert(clientSocket, buffer);
        m_bufferSizes.insert(clientSocket, s);

       //Start timer to disconnect after 10sec
        auto timer = new QTimer();
        connect(timer, &QTimer::timeout, this, &TCPServer::onCloseConnection);
        m_socketTimers.insert(timer, clientSocket);
        timer->start(10000); // 10s
    }
}

/// <summary>
/// Event to handle the disconnnection of the socket.
/// </summary>
void TCPServer::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        auto sender = static_cast<QTcpSocket*>(QObject::sender());
        qInfo() << "Client " <<  sender->peerAddress().toString() << "has just been disconnected";

        // delete everything (socket, sizePointer and bufferPointer, timer)
        auto buffer = m_dataBuffers.value(sender);
        auto size = m_bufferSizes.value(sender);
        auto timer = m_socketTimers.key(sender);
        m_socketTimers.remove(timer);
        sender->deleteLater();
        timer->deleteLater();
        delete buffer;
        delete size;
    }
}

/// <summary>
/// Event called when new data steam arrive by a socket.
/// </summary>
void TCPServer::onReadyRead()
{
    auto socket = static_cast<QTcpSocket*>(QObject::sender());
    auto timer = m_socketTimers.key(socket);
    timer->stop();

    auto buffer = m_dataBuffers.value(socket);
    auto s = m_bufferSizes.value(socket);
    auto size = *s;

    // read until there is no more byte in the socket
    while (socket->bytesAvailable() > 0)
    {
        buffer->append(socket->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size))
        {
            // read the size and remove those data
            if (size == 0 && buffer->size() >= 4)
            {
                size = readBufferSize(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            // read the AOR
            if (size > 0 && buffer->size() >= size)
            {
                auto data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                // emit a signal to query the hashMap.
                emit onQueryAndSendResult(socket, QString(data));
            }
        }
    }
    timer->start(10000); // 10s
}

/// <summary>
/// Event to query the hashMap of SIP with the requested AOR.
/// Answer to the socket with the response (SIP).
/// </summary>
void TCPServer::onQueryAndSendResult(QTcpSocket* socket, QString aor)
{
    auto sip = m_records.value(aor);
    socket->write(QByteArray::fromStdString(sip.toStdString()));
    socket->waitForBytesWritten();
}

/// <summary>
/// Read the byte array and transform it as a number.
/// </summary>
int TCPServer::readBufferSize(QByteArray source)
{
    int temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

/// <summary>
/// Event sent by a timer to close a socket.
/// </summary>
void TCPServer::onCloseConnection()
{
    auto timer = static_cast<QTimer*>(QObject::sender());
    auto socket = m_socketTimers.value(timer);
    socket->disconnectFromHost();
}
