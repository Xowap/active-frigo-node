#-------------------------------------------------
#
# Project created by QtCreator 2014-12-20T16:18:52
#
#-------------------------------------------------

QT       += core network multimedia

QT       -= gui

TARGET = active-frigo-node
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    maintask.cpp \
    frigoserver.cpp \
    config.cpp

HEADERS += \
    maintask.h \
    frigoserver.h \
    config.h
