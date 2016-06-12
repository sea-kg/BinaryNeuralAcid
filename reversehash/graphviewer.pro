QT += core gui opengl widgets

TARGET = graphviewer
TEMPLATE = app
CONFIG += console release

SOURCES += \
	src/ui/widget.cpp \
	src/graphviewer.cpp \
	src/vertexes/vertex_in.cpp \
	src/vertexes/vertex.cpp \
    src/operations/xor.cpp \
    src/operations/or.cpp \
    src/operations/and.cpp \


HEADERS += \
	src/ui/widget.h \
	src/vertexes/vertex_in.h \
	src/vertexes/vertex.h \
	src/interfaces/ioperation.h \
	src/interfaces/ivertex_out.h \
	src/interfaces/ivertex_in.h \
	src/interfaces/ivertex_operation.h \
    src/operations/xor.h \
    src/operations/or.h \
    src/operations/and.h \
    
    
LIBS += -lglut -lGLU

OBJECTS_DIR = tmp/
MOC_DIR = tmp/
