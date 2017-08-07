QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = killLocalSocket
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mianclass.cpp

HEADERS += \
    mianclass.h
