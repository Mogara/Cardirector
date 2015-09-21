#-------------------------------------------------
#
# Project created by QtCreator 2015-01-24T00:55:59
#
#-------------------------------------------------
TARGET = Cardirector
TEMPLATE = lib
CONFIG += precompile_header
CONFIG += staticlib

QT += quick
android:QT += androidextras

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = src/cpch.h

TARGET = $$qtLibraryTarget($$TARGET)

CONFIG(staticlib) {
    DEFINES += MCD_STATIC
}
else {
    DEFINES += MCD_BUILD
}

INCLUDEPATH += src \
    src/common \
    src/client \
    src/gamelogic \
    src/ui \
    src/resource \
    src/server \
    src/network

SOURCES += \
    src/ai/cabstractai.cpp \
    src/client/cclient.cpp \
    src/client/cclientsettings.cpp \
    src/client/cclientuser.cpp \
    src/client/cuserdata.cpp \
    src/common/cabstractuser.cpp \
    src/common/cdeviceinfo.cpp \
    src/common/cexceptionhandler.cpp \
    src/controller/cabstractaicontroller.cpp \
    src/controller/cabstractcontroller.cpp \
    src/controller/cabstractuicontroller.cpp \
    src/gamelogic/cabstractgamelogic.cpp \
    src/gamelogic/cabstractplayer.cpp \
    src/network/cjsonpacketparser.cpp \
    src/network/cnetwork.cpp \
    src/network/cpacket.cpp \
    src/network/cpacketrouter.cpp \
    src/network/cprotocol.cpp \
    src/network/ctcpserver.cpp \
    src/network/ctcpsocket.cpp \
    src/network/cwifimanager.cpp \
    src/osc/cosc.cpp \
    src/resource/cresourcemanager.cpp \
    src/resource/cimageprovider.cpp \
    src/server/croom.cpp \
    src/server/cserver.cpp \
    src/server/cserveragent.cpp \
    src/server/cserverrobot.cpp \
    src/server/cserveruser.cpp \
    src/sound/csound.cpp \
    src/ui/cabstractui.cpp \
    src/ui/cmainwindow.cpp \
    src/ui/cqmlengine.cpp \
    src/ui/ctranslator.cpp

HEADERS += \
    src/ai/cabstractai.h \
    src/client/cclient.h \
    src/client/cclientsettings.h \
    src/client/cclientuser.h \
    src/client/cuserdata.h \
    src/common/cabstractuser.h \
    src/common/cdeviceinfo.h \
    src/common/cglobal.h \
    src/common/cexceptionhandler.h \
    src/controller/cabstractaicontroller.h \
    src/controller/cabstractcontroller.h \
    src/controller/cabstractuicontroller.h \
    src/gamelogic/cabstractgamelogic.h \
    src/gamelogic/cabstractplayer.h \
    src/network/cabstractpacketparser.h \
    src/network/cjsonpacketparser.h \
    src/network/cnetwork.h \
    src/network/cpacket.h \
    src/network/cpacketrouter.h \
    src/network/cprotocol.h \
    src/network/ctcpserver.h \
    src/network/ctcpsocket.h \
    src/network/cwifimanager.h \
    src/osc/cosc.h \
    src/resource/cimageprovider.h \
    src/resource/cresourcemanager.h \
    src/server/croom.h \
    src/server/cserver.h \
    src/server/cserveragent.h \
    src/server/cserverrobot.h \
    src/server/cserveruser.h \
    src/sound/csound.h \
    src/ui/cabstractui.h \
    src/ui/cmainwindow.h \
    src/ui/cqmlengine.h \
    src/ui/ctranslator.h \
    src/cpch.h

QML_FILES += Gui/Dialog.qml \
             Gui/MetroButton.qml \
             Gui/Splash.qml \
             Gui/TileButton.qml \
             Gui/ToolTipArea.qml

include(Gui/Private/private.pri)

DISTFILES = Gui/qmldir

qmldir.files = Gui/qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

# Create the resource file
GENERATED_RESOURCE_FILE = gui.qrc

INCLUDED_RESOURCE_FILES = $$QML_FILES

RESOURCE_CONTENT = \
    "<RCC>" \
    "    <qresource prefix=\"/Cardirector/Gui\">"

for(resourcefile, INCLUDED_RESOURCE_FILES) {
    resourcefileabsolutepath = $$absolute_path($$resourcefile)
    relativepath = $$relative_path($$resourcefileabsolutepath, $$_PRO_FILE_PWD_)
    RESOURCE_CONTENT += "        <file alias=\"$$basename(resourcefile)\">$$relativepath</file>"
}

RESOURCE_CONTENT += \
    "    </qresource>" \
    "</RCC>"

write_file($$GENERATED_RESOURCE_FILE, RESOURCE_CONTENT)|error("Aborting.")

RESOURCES += \
    $$GENERATED_RESOURCE_FILE \
    image.qrc \
    font.qrc

defineTest(copy) {
    file = $$1
    path = $$2
    !exists($$file): return(false)
    system("$$QMAKE_COPY $$system_path($$file) $$system_path($$path)")

    return(true)
}

for(file, HEADERS) {
    !equals(file, "src/cpch.h") {
        copy($$file, include/)
    }
}

win32 {
    DESTDIR = bin
    win32-msvc* {
        QMAKE_POST_LINK = $$QMAKE_COPY $$system_path($$OUT_PWD/$$DESTDIR/$${TARGET}.lib) $$system_path($$PWD/lib)
    }
    win32-g++ {
        QMAKE_POST_LINK = $$QMAKE_COPY $$system_path($$OUT_PWD/$$DESTDIR/lib$${TARGET}.a) $$system_path($$PWD/lib)
    }
    !CONFIG(staticlib) {
        QMAKE_POST_LINK += && $$QMAKE_COPY $$system_path($$OUT_PWD/$$DESTDIR/$${TARGET}.dll) $$system_path($$PWD/bin)
        win32-msvc {
            QMAKE_POST_LINK += && $$QMAKE_COPY $$system_path($$OUT_PWD/$$DESTDIR/$${TARGET}.exp) $$system_path($$PWD/bin)
        }
    }
}
linux {
    CONFIG(staticlib) {
        QMAKE_POST_LINK = $$QMAKE_COPY $$system_path($$OUT_PWD/lib$${TARGET}.a) $$system_path($$PWD/lib)
    } else {
        QMAKE_POST_LINK = $$QMAKE_COPY $$system_path($$OUT_PWD/lib$${TARGET}.so) $$system_path($$PWD/lib)
    }
}
macx {
    CONFIG(staticlib) {
        QMAKE_POST_LINK = $$QMAKE_COPY $$system_path($$OUT_PWD/lib$${TARGET}.a) $$system_path($$PWD/lib)
    } else {
        QMAKE_POST_LINK = $$QMAKE_COPY $$system_path($$OUT_PWD/lib$${TARGET}.dylib) $$system_path($$PWD/lib)
    }
}

include(src/3rdparty/breakpad.pri)
