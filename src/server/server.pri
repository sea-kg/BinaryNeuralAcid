INCLUDEPATH += src/server
INCLUDEPATH += src/server/interfaces
INCLUDEPATH += src/server/cmd_handlers/include
INCLUDEPATH += src/server/cmd_handlers

HEADERS += \
    src/server/interfaces/icmdhandler.h \
    src/server/interfaces/ireversehashdserver.h \
    src/server/reversehashd_server.h \
	src/server/error.h \
	src/server/training_thread.h \
	src/server/training_thread_item.h \
	src/server/training_thread_message.h \
    src/server/cmd_handlers/create_cmd_handlers.h \
	src/server/cmd_handlers/include/cmd_hello_handler.h \
	src/server/cmd_handlers/include/cmd_reverse_handler.h \
	src/server/cmd_handlers/include/cmd_statistics_handler.h \
	src/server/cmd_handlers/include/cmd_graph_handler.h \

SOURCES += \
	src/server/reversehashd_server.cpp \
    src/server/error.cpp \
    src/server/training_thread.cpp \
    src/server/training_thread_item.cpp \
    src/server/training_thread_message.cpp \
    src/server/cmd_handlers/create_cmd_handlers.cpp \
    src/server/cmd_handlers/cmd_hello_handler.cpp \
    src/server/cmd_handlers/cmd_reverse_handler.cpp \
    src/server/cmd_handlers/cmd_statistics_handler.cpp \
    src/server/cmd_handlers/cmd_graph_handler.cpp \
