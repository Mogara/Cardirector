#-------------------------------------------------
#
# Project created by QtCreator 2015-01-24T00:55:59
#
#-------------------------------------------------

QT       -= gui

TARGET = Cardirector
TEMPLATE = lib

#CONFIG += staticlib


CONFIG(staticlib) {
    DEFINES += MCD_STATIC
}
else {
    DEFINES += MCD_BUILD
}

SOURCES += \
    src/ai/abstractai.cpp \
    src/client/abstractclient.cpp \
    src/client/userdata.cpp \
    src/controller/abstractaicontroller.cpp \
    src/controller/abstractcontroller.cpp \
    src/controller/abstractuicontroller.cpp \
    src/gamelogic/abstractgamelogic.cpp \
    src/network/network.cpp \
    src/osc/osc.cpp \
    src/resource/resource.cpp \
    src/server/abstractserver.cpp \
    src/sound/sound.cpp \
    src/ui/virtualui.cpp

HEADERS += \
    src/common/cardirector_global.h \
    src/ai/abstractai.h \
    src/client/abstractclient.h \
    src/client/userdata.h \
    src/controller/abstractaicontroller.h \
    src/controller/abstractcontroller.h \
    src/controller/abstractuicontroller.h \
    src/gamelogic/abstractgamelogic.h \
    src/network/network.h \
    src/osc/osc.h \
    src/resource/resource.h \
    src/server/abstractserver.h \
    src/sound/sound.h \
    src/ui/virtualui.h

