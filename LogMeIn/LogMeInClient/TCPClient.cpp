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
TCPClient::TCPClient() : 
m_socket(new QTcpSocket()), 
m_isQuerying(false)
{}

/// <summary>
/// Connect the socket to the host on port 8888.
/// return true if it's a success.
/// </summary>
bool TCPClient::ConnectToHost(QHostAddress host)
{
    m_socket->connectToHost(host, 8888);
    //connect(m_socket, &QTcpSocket::readyRead, this, &TCPClient::OnReadyRead);

    if (m_socket->waitForConnected())
        return true;

    qWarning() << "Could not connect to host" << host.toString() << ": " << m_socket->error();
    return false;
}

/// <summary>
/// Query a SIP on the TCP Server with an AOR.
/// </summary>
bool TCPClient::Query(QString addressOfRecord)
{
    qInfo() << "Querying server with AOR: " << addressOfRecord;

    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_isQuerying = true;
        auto byteAOR = QByteArray::fromStdString(addressOfRecord.toStdString());

        // write the size of the payload before the aor itself
        QByteArray temp;
        QDataStream data(&temp, QIODevice::ReadWrite);
        data << byteAOR.size();

        m_socket->write(temp);
        m_socket->write(byteAOR);
        m_socket->waitForBytesWritten();

        ReadData();

        return true;
    }

    return false;
}

/// <summary>
/// Event to read the data reveive on the TCP socket.
/// </summary>
void TCPClient::ReadData()
{
    QByteArray dataBuffer;
    dataBuffer.clear();
    m_socket->waitForReadyRead();
    while (m_socket->bytesAvailable() > 0)
    {
        dataBuffer.append(m_socket->readAll());
    }
    qInfo() << "Server replied with: " << dataBuffer;
    //m_isQuerying = false;
}
