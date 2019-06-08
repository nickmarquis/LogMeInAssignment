QT += testlib core network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCDIR = ../LogMeInServer
INCLUDEPATH += $$INCDIR

HEADERS += $$INCDIR/tcpServer.h
SOURCES +=  tst_logmeinservertests.cpp $$INCDIR/tcpServer.cpp

RESOURCES += \
    $$INCDIR/Resources/resources.qrc
