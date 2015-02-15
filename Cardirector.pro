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

TARGET = $$qtLibraryTarget($$TARGET)

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
    src/network/cabstractpacket.cpp \
    src/network/cjsonpacket.cpp \
    src/network/ctcpserver.cpp \
    src/network/ctcpsocket.cpp \
    src/osc/cosc.cpp \
    src/server/cabstractserver.cpp \
    src/sound/csound.cpp \
    src/ui/cabstractui.cpp \
    src/client/cclientsettings.cpp \
    src/ui/cmainwindow.cpp \
    src/resource/cresourcemanager.cpp

HEADERS += \
    src/ai/cabstractai.h \
    src/client/cabstractclient.h \
    src/client/cuserdata.h \
    src/controller/cabstractaicontroller.h \
    src/controller/cabstractcontroller.h \
    src/controller/cabstractuicontroller.h \
    src/gamelogic/cabstractgamelogic.h \
    src/network/cnetwork.h \
    src/network/cabstractpacket.h \
    src/network/cjsonpacket.h \
    src/network/ctcpserver.h \
    src/network/ctcpsocket.h \
    src/osc/cosc.h \
    src/server/cabstractserver.h \
    src/sound/csound.h \
    src/ui/cabstractui.h \
    src/common/cglobal.h \
    src/cpch.h \
    src/client/cclientsettings.h \
    src/ui/cmainwindow.h \
    src/resource/cresourcemanager.h

#QML_FILES +=

#DISTFILES = Gui/qmldir

#qmldir.files = Gui/qmldir
#unix {
#    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
#    qmldir.path = $$installPath
#    target.path = $$installPath
#    INSTALLS += target qmldir
#}

## Create the resource file
#GENERATED_RESOURCE_FILE = gui.qrc

#INCLUDED_RESOURCE_FILES = $$QML_FILES

#RESOURCE_CONTENT = \
#    "<RCC>" \
#    "<qresource prefix=\"/Cardirector/Gui\">"

#for(resourcefile, INCLUDED_RESOURCE_FILES) {
#    RESOURCE_CONTENT += "<file alias=\"$$basename(resourcefile)\">$$resourcefile</file>"
#}

#RESOURCE_CONTENT += \
#    "</qresource>" \
#    "</RCC>"

#write_file($$GENERATED_RESOURCE_FILE, RESOURCE_CONTENT)|error("Aborting.")

#RESOURCES += $$GENERATED_RESOURCE_FILE

defineTest(copy) {
    file = $$1
    path = $$2
    !exists($$file): return(false)
    equals(QMAKE_HOST.os, Windows) {
        system("copy $$system_path($$file) $$system_path($$path)")
    }
    else {
        system("cp $$file $$path")
    }

    return(true)
}

for(file, HEADERS) {
    !equals(file, "src/cpch.h") {
        copy($$file, include/)
    }
}

win32 {
    QMAKE_POST_LINK = \
        $$QMAKE_COPY $$system_path($$OUT_PWD/$$DESTDIR/$${TARGET}.dll) $$system_path($$PWD/bin) \
     && $$QMAKE_COPY $$system_path($$OUT_PWD/$$DESTDIR/$${TARGET}.exp) $$system_path($$PWD/bin) \
     && $$QMAKE_COPY $$system_path($$OUT_PWD/$$DESTDIR/$${TARGET}.lib) $$system_path($$PWD/lib)

    message($$QMAKE_POST_LINK)
}
android {
    QMAKE_POST_LINK = $$QMAKE_COPY \"$$OUT_PWD\\$$DESTDIR\\lib$${TARGET}.so\" \"$$PWD\\bin\" \
}
