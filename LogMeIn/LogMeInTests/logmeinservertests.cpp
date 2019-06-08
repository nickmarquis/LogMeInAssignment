//:>--------------------------------------------------------------------------------------+
//:> Nick Marquis                                                                   06/2019
//:>+--------------------------------------------------------------------------------------

#include "../LogMeInServer/tcpserver.h"
#include "logmeinservertests.h"

/// <summary>
/// Init the object we need in the test file.
/// </summary>
void LogMeInServerTests::initTestCase()
{

}

/// <summary>
/// Cleanup run after all the test are done.
/// </summary>
void LogMeInServerTests::cleanupTestCase()
{

}

/// <summary>
/// Test if when can correctly read a SIP file.
/// </summary>
void LogMeInServerTests::testCaseReadSIPFile()
{
    auto tcpServer = new TCPServer();
    auto result = tcpServer->LoadSipRegistrations();
    QCOMPARE(result, true);
}

QTEST_MAIN(LogMeInServerTests)
