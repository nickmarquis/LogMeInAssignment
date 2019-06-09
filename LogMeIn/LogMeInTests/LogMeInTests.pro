QT += testlib core network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SERVER = ../LogMeInServer
CLIENT = ../LogMeInClient
INCLUDEPATH += $$SERVER $$CLIENT

HEADERS += $$SERVER/TCPServer.h \
    $$CLIENT/TCPClient.h \
    LogMeInServerTests.h
SOURCES +=  $$SERVER/TCPServer.cpp \
    $$CLIENT/TCPClient.cpp \
    LogMeInServerTests.cpp

RESOURCES += \
    $$SERVER/Resources/resources.qrc
