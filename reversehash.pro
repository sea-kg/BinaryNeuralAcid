QT       += core
QT       -= gui
QT       += xml

TARGET = reversehash
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
    src/tests/tests.cpp \
    src/tests/callout_test.cpp \
    src/tests/readwrite_test.cpp \
    src/tests/reverse_test.cpp \


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
    src/tests/tests.h \
    src/tests/ireversehashtest.h \
    src/tests/callout_test.h \
    src/tests/readwrite_test.h \
    src/tests/reverse_test.h \
   


OBJECTS_DIR = tmp/
