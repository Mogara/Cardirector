TEMPLATE = lib
CONFIG += precompile_header staticlib
DESTDIR = $$PWD/../lib

QT += quick multimedia
android:QT += androidextras

CONFIG(staticlib): DEFINES += MCD_STATIC
else: DEFINES += MCD_BUILD

CONFIG += precompile_header
# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = cpch.h

# Google Breakpad
INCLUDEPATH += 3rdparty/breakpad
win32: INCLUDEPATH += 3rdparty/breakpad/client/windows
linux: INCLUDEPATH += 3rdparty/breakpad/client/linux
android: INCLUDEPATH += 3rdparty/breakpad/common/android/include
osx: INCLUDEPATH += \
        3rdparty/breakpad/client/mac \
        3rdparty/breakpad/client/apple/Framework \
        3rdparty/breakpad/common/mac

# Ogg and Vorbis Decoder
INCLUDEPATH += 3rdparty/ogg/include
INCLUDEPATH += 3rdparty/vorbis/include

# Cardirector
INCLUDEPATH += \
    common \
    client \
    gamelogic \
    ui \
    resource \
    server \
    media \
    network \
    3rdparty

SOURCES += \
    ai/cabstractai.cpp \
    client/cclient.cpp \
    client/cclientsettings.cpp \
    client/cclientuser.cpp \
    client/cuserdata.cpp \
    common/cabstractuser.cpp \
    common/cdeviceinfo.cpp \
    common/cexceptionhandler.cpp \
    controller/cabstractaicontroller.cpp \
    controller/cabstractcontroller.cpp \
    controller/cabstractuicontroller.cpp \
    gamelogic/cabstractgamelogic.cpp \
    gamelogic/cabstractplayer.cpp \
    media/coggfile.cpp \
    media/csound.cpp \
    network/cjsonpacketparser.cpp \
    network/cnetwork.cpp \
    network/cpacket.cpp \
    network/cpacketrouter.cpp \
    network/cprotocol.cpp \
    network/ctcpserver.cpp \
    network/ctcpsocket.cpp \
    network/cwifimanager.cpp \
    osc/cosc.cpp \
    resource/cresourcemanager.cpp \
    resource/cimageprovider.cpp \
    server/croom.cpp \
    server/cserver.cpp \
    server/cserveragent.cpp \
    server/cserverrobot.cpp \
    server/cserveruser.cpp \
    ui/cabstractui.cpp \
    ui/cmainwindow.cpp \
    ui/cqmlengine.cpp \
    ui/ctranslator.cpp

HEADERS += \
    ai/cabstractai.h \
    client/cclient.h \
    client/cclientsettings.h \
    client/cclientuser.h \
    client/cuserdata.h \
    common/cabstractuser.h \
    common/cdeviceinfo.h \
    common/cglobal.h \
    common/cexceptionhandler.h \
    controller/cabstractaicontroller.h \
    controller/cabstractcontroller.h \
    controller/cabstractuicontroller.h \
    gamelogic/cabstractgamelogic.h \
    gamelogic/cabstractplayer.h \
    media/coggfile.h \
    media/csound.h \
    network/cabstractpacketparser.h \
    network/cjsonpacketparser.h \
    network/cnetwork.h \
    network/cpacket.h \
    network/cpacketrouter.h \
    network/cprotocol.h \
    network/ctcpserver.h \
    network/ctcpsocket.h \
    network/cwifimanager.h \
    osc/cosc.h \
    resource/cimageprovider.h \
    resource/cresourcemanager.h \
    server/croom.h \
    server/cserver.h \
    server/cserveragent.h \
    server/cserverrobot.h \
    server/cserveruser.h \
    ui/cabstractui.h \
    ui/cmainwindow.h \
    ui/cqmlengine.h \
    ui/ctranslator.h \
    cpch.h

QML_FILES += \
    ../Gui/Dialog.qml \
    ../Gui/FitInView.qml \
    ../Gui/MetroButton.qml \
    ../Gui/Splash.qml \
    ../Gui/TileButton.qml \
    ../Gui/ToolTipArea.qml

include(../Gui/Private/private.pri)

# Create the resource file
GENERATED_RESOURCE_FILE = cgui.qrc

INCLUDED_RESOURCE_FILES = $$QML_FILES

RESOURCE_CONTENT = \
    "<RCC>" \
    "    <qresource prefix=\"/Cardirector/Gui\">"

for(resourcefile, INCLUDED_RESOURCE_FILES) {
    resourcefileabsolutepath = $$absolute_path($$resourcefile)
    relativepath = $$relative_path($$resourcefileabsolutepath, $$PWD)
    RESOURCE_CONTENT += "        <file alias=\"$$basename(resourcefile)\">$$relativepath</file>"
}

RESOURCE_CONTENT += \
    "    </qresource>" \
    "</RCC>"

write_file($$GENERATED_RESOURCE_FILE, RESOURCE_CONTENT)|error("Aborting.")

RESOURCES += \
    $$GENERATED_RESOURCE_FILE \
    ../cimage.qrc \
    ../cfont.qrc

defineTest(copy) {
    file = $$1
    path = $$2
    !exists($$file): return(false)
    system("$$QMAKE_COPY $$system_path($$file) $$system_path($$path)")

    return(true)
}

!build_pass {
    for(file, HEADERS) {
        !equals(file, "cpch.h") {
            copy($$file, ../include/)
        }
    }
}

DISTFILES = ../Gui/qmldir

qmldir.files = ../Gui/qmldir
unix: !android {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
