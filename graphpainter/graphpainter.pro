QT += core gui opengl widgets

TARGET = graphpainter.bin
CONFIG += console release
TEMPLATE = app
SOURCES += \
	src/widget.cpp \
	src/main.cpp \
	src/graph.cpp

HEADERS += \
	src/widget.h \
	src/graph.h

LIBS += -lglut -lGLU

# INCLUDEPATH += src

OBJECTS_DIR = tmp/
MOC_DIR = tmp/
