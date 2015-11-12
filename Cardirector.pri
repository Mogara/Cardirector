

INCLUDEPATH += $$PWD/src \
    $$PWD/src/common \
    $$PWD/src/client \
    $$PWD/src/gamelogic \
    $$PWD/src/ui \
    $$PWD/src/resource \
    $$PWD/src/server \
    $$PWD/src/media \
    $$PWD/src/network \
    $$PWD/src/3rdparty

SOURCES += \
    $$PWD/src/ai/cabstractai.cpp \
    $$PWD/src/client/cclient.cpp \
    $$PWD/src/client/cclientsettings.cpp \
    $$PWD/src/client/cclientuser.cpp \
    $$PWD/src/client/cuserdata.cpp \
    $$PWD/src/common/cabstractuser.cpp \
    $$PWD/src/common/cdeviceinfo.cpp \
    $$PWD/src/common/cexceptionhandler.cpp \
    $$PWD/src/controller/cabstractaicontroller.cpp \
    $$PWD/src/controller/cabstractcontroller.cpp \
    $$PWD/src/controller/cabstractuicontroller.cpp \
    $$PWD/src/gamelogic/cabstractgamelogic.cpp \
    $$PWD/src/gamelogic/cabstractplayer.cpp \
    $$PWD/src/media/coggfile.cpp \
    $$PWD/src/media/csound.cpp \
    $$PWD/src/network/cjsonpacketparser.cpp \
    $$PWD/src/network/cnetwork.cpp \
    $$PWD/src/network/cpacket.cpp \
    $$PWD/src/network/cpacketrouter.cpp \
    $$PWD/src/network/cprotocol.cpp \
    $$PWD/src/network/ctcpserver.cpp \
    $$PWD/src/network/ctcpsocket.cpp \
    $$PWD/src/network/cwifimanager.cpp \
    $$PWD/src/osc/cosc.cpp \
    $$PWD/src/resource/cresourcemanager.cpp \
    $$PWD/src/resource/cimageprovider.cpp \
    $$PWD/src/server/croom.cpp \
    $$PWD/src/server/cserver.cpp \
    $$PWD/src/server/cserveragent.cpp \
    $$PWD/src/server/cserverrobot.cpp \
    $$PWD/src/server/cserveruser.cpp \
    $$PWD/src/ui/cabstractui.cpp \
    $$PWD/src/ui/cmainwindow.cpp \
    $$PWD/src/ui/cqmlengine.cpp \
    $$PWD/src/ui/ctranslator.cpp

HEADERS += \
    $$PWD/src/ai/cabstractai.h \
    $$PWD/src/client/cclient.h \
    $$PWD/src/client/cclientsettings.h \
    $$PWD/src/client/cclientuser.h \
    $$PWD/src/client/cuserdata.h \
    $$PWD/src/common/cabstractuser.h \
    $$PWD/src/common/cdeviceinfo.h \
    $$PWD/src/common/cglobal.h \
    $$PWD/src/common/cexceptionhandler.h \
    $$PWD/src/controller/cabstractaicontroller.h \
    $$PWD/src/controller/cabstractcontroller.h \
    $$PWD/src/controller/cabstractuicontroller.h \
    $$PWD/src/gamelogic/cabstractgamelogic.h \
    $$PWD/src/gamelogic/cabstractplayer.h \
    $$PWD/src/media/coggfile.h \
    $$PWD/src/media/csound.h \
    $$PWD/src/network/cabstractpacketparser.h \
    $$PWD/src/network/cjsonpacketparser.h \
    $$PWD/src/network/cnetwork.h \
    $$PWD/src/network/cpacket.h \
    $$PWD/src/network/cpacketrouter.h \
    $$PWD/src/network/cprotocol.h \
    $$PWD/src/network/ctcpserver.h \
    $$PWD/src/network/ctcpsocket.h \
    $$PWD/src/network/cwifimanager.h \
    $$PWD/src/osc/cosc.h \
    $$PWD/src/resource/cimageprovider.h \
    $$PWD/src/resource/cresourcemanager.h \
    $$PWD/src/server/croom.h \
    $$PWD/src/server/cserver.h \
    $$PWD/src/server/cserveragent.h \
    $$PWD/src/server/cserverrobot.h \
    $$PWD/src/server/cserveruser.h \
    $$PWD/src/ui/cabstractui.h \
    $$PWD/src/ui/cmainwindow.h \
    $$PWD/src/ui/cqmlengine.h \
    $$PWD/src/ui/ctranslator.h \
    $$PWD/src/cpch.h

QML_FILES += \
    Gui/Dialog.qml \
    Gui/FitInView.qml \
    Gui/MetroButton.qml \
    Gui/Splash.qml \
    Gui/TileButton.qml \
    Gui/ToolTipArea.qml

include($$PWD/Gui/Private/private.pri)

# Create the resource file
GENERATED_RESOURCE_FILE = $$PWD/cgui.qrc

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
    $$PWD/cimage.qrc \
    $$PWD/cfont.qrc

include($$PWD/src/3rdparty/breakpad/breakpad.pri)
include($$PWD/src/3rdparty/libogg/libogg.pri)
include($$PWD/src/3rdparty/libvorbis/libvorbis.pri)
