#-------------------------------------------------
#
# Project created by QtCreator 2015-01-24T00:55:59
#
#-------------------------------------------------
TARGET = Cardirector
TEMPLATE = lib
CONFIG += precompile_header
#CONFIG += staticlib

QT += quick

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = src/cpch.h

DESTDIR = bin

CONFIG(debug, debug|release) {
    mac: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

CONFIG(staticlib) {
    DEFINES += MCD_STATIC
}
else {
    DEFINES += MCD_BUILD
}

INCLUDEPATH += src \
               src/common \
               src/client

SOURCES += \
    src/ai/cabstractai.cpp \
    src/client/cabstractclient.cpp \
    src/client/cuserdata.cpp \
    src/controller/cabstractaicontroller.cpp \
    src/controller/cabstractcontroller.cpp \
    src/controller/cabstractuicontroller.cpp \
    src/gamelogic/cabstractgamelogic.cpp \
    src/network/cnetwork.cpp \
    src/osc/cosc.cpp \
    src/resource/cresource.cpp \
    src/server/cabstractserver.cpp \
    src/sound/csound.cpp \
    src/ui/cabstractui.cpp \
    src/client/cclientsettings.cpp \
    src/ui/cmainwindow.cpp

HEADERS += \
    src/ai/cabstractai.h \
    src/client/cabstractclient.h \
    src/client/cuserdata.h \
    src/controller/cabstractaicontroller.h \
    src/controller/cabstractcontroller.h \
    src/controller/cabstractuicontroller.h \
    src/gamelogic/cabstractgamelogic.h \
    src/network/cnetwork.h \
    src/osc/cosc.h \
    src/resource/cresource.h \
    src/server/cabstractserver.h \
    src/sound/csound.h \
    src/ui/cabstractui.h \
    src/common/cglobal.h \
    src/cpch.h \
    src/client/cclientsettings.h \
    src/ui/cmainwindow.h


for(file, HEADERS) {
    !equals(file, "src/cpch.h") {
        win32 {
            system("copy $$replace(file, /, \\) include\\")
        }
        else {
            system("cp $$file include/")
        }
    }
}

