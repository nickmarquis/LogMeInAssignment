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
    void initTestCase();
    void cleanupTestCase();
    void testCaseReadSIPFile();

};

#endif // LOGMEINSERVERTESTS_H
