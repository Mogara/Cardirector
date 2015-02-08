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
    src/osc/cosc.h \
    src/server/cabstractserver.h \
    src/sound/csound.h \
    src/ui/cabstractui.h \
    src/common/cglobal.h \
    src/cpch.h \
    src/client/cclientsettings.h \
    src/ui/cmainwindow.h \
    src/resource/cresourcemanager.h

defineTest(copy) {
    file = $$1
    path = $$2
    !exists(file): return(false)
    win32 {
        system("copy $$replace(file, /, \\) $$replace(path, /, \\)")
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

build_pass {
    win32-msvc2013 {
        copy($$OUT_PWD/$$DESTDIR/$$TARGET".dll", bin/win32-msvc2013/)
        copy($$OUT_PWD/$$DESTDIR/$$TARGET".pdb", bin/win32-msvc2013/)
        copy($$OUT_PWD/$$DESTDIR/$$TARGET".lib", lib/win32-msvc2013/)
    }
    win32-msvc2010 {
        copy($$OUT_PWD/$$DESTDIR/$$TARGET".dll", bin/win32-msvc2010/)
        copy($$OUT_PWD/$$DESTDIR/$$TARGET".pdb", bin/win32-msvc2010/)
        copy($$OUT_PWD/$$DESTDIR/$$TARGET".lib", lib/win32-msvc2010/)
    }
}
