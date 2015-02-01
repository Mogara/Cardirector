#-------------------------------------------------
#
# Project created by QtCreator 2015-01-24T00:55:59
#
#-------------------------------------------------
TARGET = Cardirector
TEMPLATE = lib
CONFIG += precompile_header
#CONFIG += staticlib

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = src/cpch.h

CONFIG(staticlib) {
    DEFINES += MCD_STATIC
}
else {
    DEFINES += MCD_BUILD
}

INCLUDEPATH += src \
               src/common \
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

HEADERS += \
    src/common/cardirector_global.h \
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

