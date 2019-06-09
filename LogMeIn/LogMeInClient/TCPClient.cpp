//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include "TCPClient.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QThread>
#include <QCoreApplication>

/// <summary>
/// Constructor
/// </summary>
TCPClient::TCPClient() :
m_socket(new QTcpSocket())
{
    connect(m_socket, &QTcpSocket::disconnected, []()->void{ qDebug("Disconnected from host"); });
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
    {
        readData();
        return true;
    }

    qWarning() << "Could not connect to host " << host.toString() << ": " << m_socket->error();
    return false;
}

/// <summary>
/// Query a SIP on the TCP Server with an AOR.
/// </summary>
bool TCPClient::query(QString addressOfRecord)
{
    QCoreApplication::processEvents();
    qInfo() << "Querying server with AOR: " << addressOfRecord;
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
        readData();
        return true;
    }
    return false;
}

/// <summary>
/// Read the data reveive on the TCP socket.
/// </summary>
void TCPClient::readData()
{
    QByteArray dataBuffer;
    dataBuffer.clear();

    // timeout read after 2 sec
    m_socket->waitForReadyRead(2000);
    while (m_socket->bytesAvailable() > 0)
    {
        dataBuffer.append(m_socket->readAll());
    }
    qInfo() << "\nServer replied with: " << dataBuffer;
}
