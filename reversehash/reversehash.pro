QT       += core
QT       -= gui
QT       += xml

TARGET = reversehash.bin
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/graph.cpp \
    src/memory.cpp \
    src/line.cpp \
    src/helpers.cpp \
    src/memoryitem.cpp \
    src/operations/xor.cpp \
    src/operations/or.cpp \
    src/operations/and.cpp \
    src/vertexes/vertex_in.cpp \
    src/vertexes/vertex.cpp \
    src/vertex_graph.cpp \


HEADERS += \
    src/graph.h \
    src/memory.h \
    src/line.h \
    src/helpers.h \
    src/memoryitem.h \
    src/interfaces/ioperation.h \
    src/interfaces/ivertex_operation.h \
    src/interfaces/ivertex_out.h \
    src/interfaces/ivertex_in.h \
    src/operations/xor.h \
    src/operations/or.h \
    src/operations/and.h \
    src/vertexes/vertex_in.h \
    src/vertexes/vertex.h \
    src/vertex_graph.h \



OBJECTS_DIR = tmp/
