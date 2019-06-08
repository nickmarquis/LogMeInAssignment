#include <QtTest>
#include <QCoreApplication>
#include "../LogMeInServer/tcpserver.h"

// add necessary includes here

class LogMeInServerTests : public QObject
{
    Q_OBJECT

public:
    LogMeInServerTests();
    ~LogMeInServerTests();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCaseReadSIPFile();

};

LogMeInServerTests::LogMeInServerTests()
{

}

LogMeInServerTests::~LogMeInServerTests()
{

}

void LogMeInServerTests::initTestCase()
{

}

void LogMeInServerTests::cleanupTestCase()
{

}

void LogMeInServerTests::testCaseReadSIPFile()
{
    auto tcpServer = new TCPServer();
    auto result = tcpServer->LoadSipRegistrations();
    QCOMPARE(result, true);
}

QTEST_MAIN(LogMeInServerTests)

#include "tst_logmeinservertests.moc"
