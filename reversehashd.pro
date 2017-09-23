TEMPLATE = app
TARGET = reversehashd

QT       += core xml network websockets
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11 c++14

OBJECTS_DIR = tmp/
MOC_DIR = tmp/
RCC_DIR = tmp/

INCLUDEPATH += src

include(src/memory/memory.pri)
include(src/bna/bna.pri)
include(src/server/server.pri)
include(src/tests/tests.pri)

RESOURCES = reversehashd.qrc

SOURCES += src/main.cpp \
    src/helpers.cpp \

HEADERS += \
    src/helpers.h \
   
