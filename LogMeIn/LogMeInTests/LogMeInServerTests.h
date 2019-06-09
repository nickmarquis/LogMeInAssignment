//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#ifndef LOGMEINSERVERTESTS_H
#define LOGMEINSERVERTESTS_H

#include <QtTest>
#include <QCoreApplication>

/// <summary>
/// Unit and integration tests for the TCPServer
/// </summary>
class LogMeInServerTests : public QObject
{
    Q_OBJECT

public:
    LogMeInServerTests() = default;
    ~LogMeInServerTests() = default;

private slots:

    //:>---------------------------------------------------------------------------------------
    //:> Unit Tests
    //:>---------------------------------------------------------------------------------------
    void testCaseReadSIPFile();
    void testCaseStartServer();

    //:>---------------------------------------------------------------------------------------
    //:> Integration Tests
    //:>---------------------------------------------------------------------------------------
    void testCaseServerConnect();
    void testCaseServerDisconnect();
    void testCaseServerReceiveAOR();
    void testCaseClientReceiveSIP();
};

#endif // LOGMEINSERVERTESTS_H
