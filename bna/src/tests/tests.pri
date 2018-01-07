INCLUDEPATH += src/tests

HEADERS += \
    src/tests/tests.h \
    src/tests/ireversehashtest.h \
    src/tests/callout_test.h \
    src/tests/readwrite_test.h \
    src/tests/reverse_test.h \
    src/tests/random_mutations_test.h \
    $$PWD/learning_test.h \
    $$PWD/bnaoper_test.h

SOURCES += \
    src/tests/tests.cpp \
    src/tests/callout_test.cpp \
    src/tests/readwrite_test.cpp \
    src/tests/reverse_test.cpp \
    src/tests/random_mutations_test.cpp \
    $$PWD/learning_test.cpp \
    $$PWD/bnaoper_test.cpp
