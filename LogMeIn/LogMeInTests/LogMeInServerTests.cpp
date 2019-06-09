//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include "LogMeInServerTests.h"
#include "../LogMeInServer/TCPServer.h"
#include "../LogMeInClient/ClientApp.h"

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
    qRegisterMetaType<QTcpSocket*>();

    QTest::ignoreMessage(QtInfoMsg, "Querying SIP with:  \"015802aa8439a1de3e000100620002\"");
    auto tcpServer = new TCPServer();
    tcpServer->start();

    auto tcpClient = new TCPClient();
    auto success = tcpClient->connectToHost();
    QCOMPARE(success, true);

    // wait for onNewConnection to be called
    QTest::qWait(1000);
    tcpClient->query("015802aa8439a1de3e000100620002");
    QTest::qWait(1000);
}

/// <summary>
/// Test if the server send the right SIP
/// </summary>
void LogMeInServerTests::testCaseClientReceiveSIP()
{
    qRegisterMetaType<QTcpSocket*>();

    QTest::ignoreMessage(QtInfoMsg, QRegularExpression("sip:015802aa8439a1de3e000100620002@26.39.196.144:5060"));
    auto tcpServer = new TCPServer();
    tcpServer->loadSipRegistrations();
    tcpServer->start();

    auto tcpClient = new TCPClient();
    auto success = tcpClient->connectToHost();
    QCOMPARE(success, true);

    // wait for onNewConnection to be called
    QTest::qWait(1000);
    tcpClient->query("015802aa8439a1de3e000100620002");
    QTest::qWait(2000);
}

QTEST_MAIN(LogMeInServerTests)
