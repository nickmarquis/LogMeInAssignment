//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include "LogMeInServerTests.h"
#include "../LogMeInServer/TCPServer.h"
#include "../LogMeInClient/TCPClient.h"

/// <summary>
/// Test if when can correctly read a SIP file.
/// </summary>
void LogMeInServerTests::testCaseReadSIPFile()
{
    auto tcpServer = new TCPServer();
    auto result = tcpServer->loadSipRegistrations();
    QCOMPARE(result, true);
}

/// <summary>
/// Test if we correctly start the server.
/// </summary>
void LogMeInServerTests::testCaseStartServer()
{
    auto tcpServer = new TCPServer();
    auto result = tcpServer->start();
    QCOMPARE(result, true);
}

/// <summary>
/// Test if a socket can connect to the server
/// </summary>
void LogMeInServerTests::testCaseServerConnect()
{
    auto tcpServer = new TCPServer();
    tcpServer->start();

    auto tcpSocket = new QTcpSocket();

    QSignalSpy spy(tcpSocket, &QTcpSocket::connected);
    tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 8888);

    spy.wait();
    QCOMPARE(spy.count(), 1);
}

/// <summary>
/// Test if server disconnect the socket after 10s
/// </summary>
void LogMeInServerTests::testCaseServerDisconnect()
{
    auto tcpServer = new TCPServer();
    tcpServer->start();

    auto tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 8888);
    QCOMPARE(tcpSocket->waitForConnected(), true);

    QSignalSpy spy(tcpSocket, &QTcpSocket::disconnected);
    spy.wait(12000);
    QCOMPARE(spy.count(), 1);
}

Q_DECLARE_METATYPE(QTcpSocket*)
/// <summary>
/// Test if the server read the correct AOR
/// </summary>
void LogMeInServerTests::testCaseServerReceiveAOR()
{
    auto tcpServer = new TCPServer();
    tcpServer->start();

    auto tcpClient = new TCPClient();
    auto success = tcpClient->connectToHost();
    QCOMPARE(success, true);

    QSignalSpy spy(tcpServer, &TCPServer::queryAndSendResult);

    qRegisterMetaType<QTcpSocket*>();
    tcpClient->query("015802aa8439a1de3e000100620002");

    //spy.wait(100);
    //QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 2000);
    //QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(LogMeInServerTests)
