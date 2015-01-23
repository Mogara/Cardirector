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
    src/ai/AbstractAi.cpp \
    src/client/AbstractClient.cpp \
    src/client/UserData.cpp \
    src/controller/AbstractAiController.cpp \
    src/controller/AbstractController.cpp \
    src/controller/AbstractUiController.cpp \
    src/gamelogic/AbstractGameLogic.cpp \
    src/network/Network.cpp \
    src/osc/OSC.cpp \
    src/resource/Resource.cpp \
    src/server/AbstractServer.cpp \
    src/sound/Sound.cpp \
    src/ui/VirtualUi.cpp

HEADERS += \
    src/common/cardirector_global.h \
    src/ai/AbstractAi.h \
    src/client/AbstractClient.h \
    src/client/UserData.h \
    src/controller/AbstractAiController.h \
    src/controller/AbstractController.h \
    src/controller/AbstractUiController.h \
    src/gamelogic/AbstractGameLogic.h \
    src/network/Network.h \
    src/osc/OSC.h \
    src/resource/Resource.h \
    src/server/AbstractServer.h \
    src/sound/Sound.h \
    src/ui/VirtualUi.h

