//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include "TCPClient.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

/// <summary>
/// Constructor
/// </summary>
TCPClient::TCPClient(QObject *parent) : QObject(parent),
m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
}

/// <summary>
/// Connect the socket to the host on port 8888.
/// return true if it's a success.
/// </summary>
bool TCPClient::connectToHost()
{
    auto host = QHostAddress("127.0.0.1");
    m_socket->connectToHost(host, 8888);
    if (m_socket->waitForConnected())
        return true;

    qWarning() << "Could not connect to host " << host.toString() << ": " << m_socket->error();
    return false;
}

/// <summary>
/// Query a SIP on the TCP Server with an AOR.
/// </summary>
void TCPClient::query(QString addressOfRecord)
{
    qInfo() << "\nQuerying server with AOR: " << addressOfRecord;
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        auto byteAOR = QByteArray::fromStdString(addressOfRecord.toStdString());

        // write the size of the payload before the aor itself
        QByteArray temp;
        QDataStream data(&temp, QIODevice::ReadWrite);
        data << byteAOR.size();

        m_socket->write(temp);
        m_socket->write(byteAOR);
        m_socket->waitForBytesWritten();
        return;
    }
    qWarning() << "The socket is close, unable to query...";
}

/// <summary>
/// Read the data reveive on the TCP socket.
/// </summary>
void TCPClient::onReadyRead()
{
    QByteArray dataBuffer;
    while (m_socket->bytesAvailable() > 0)
    {
        dataBuffer.append(m_socket->readAll());
    }
    qInfo() << "\nServer replied with: " << dataBuffer;
}
