INCLUDEPATH += src/bna
INCLUDEPATH += src/bna/operations
INCLUDEPATH += src/bna/vertexes
INCLUDEPATH += src/bna/interfaces

HEADERS += \
	src/bna/bna.h \
	src/bna/vertex_graph.h \
    src/bna/operations/xor.h \
    src/bna/operations/or.h \
    src/bna/operations/and.h \
    src/bna/vertexes/vertex_in.h \
    src/bna/vertexes/vertex.h \

SOURCES += \
	src/bna/bna.cpp \
	src/bna/vertex_graph.cpp \
    src/bna/operations/xor.cpp \
    src/bna/operations/or.cpp \
    src/bna/operations/and.cpp \
    src/bna/vertexes/vertex_in.cpp \
    src/bna/vertexes/vertex.cpp \
