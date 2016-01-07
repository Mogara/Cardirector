TEMPLATE = lib
CONFIG += staticlib warn_off
DESTDIR = $$PWD/../../../lib

SOURCES += \
    bitwise.c \
    framing.c

INCLUDEPATH += include
