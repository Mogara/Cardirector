TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD/../lib
DEFINES += QT_NO_CAST_FROM_ASCII

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
INCLUDEPATH += 3rdparty/oggvorbis/include

# Cardirector
INCLUDEPATH += \
    ai \
    common \
    client \
    gamelogic \
    media \
    network \
    resource \
    server \
    ui \
    3rdparty

SOURCES += \
    $$files(ai/*.cpp) \
    $$files(common/*.cpp) \
    $$files(client/*.cpp) \
    $$files(gamelogic/*.cpp) \
    $$files(media/*.cpp) \
    $$files(network/*.cpp) \
    $$files(resource/*.cpp) \
    $$files(server/*.cpp) \
    $$files(ui/*.cpp)

HEADERS += \
    $$files(ai/*.h) \
    $$files(common/*.h) \
    $$files(client/*.h) \
    $$files(gamelogic/*.h) \
    $$files(media/*.h) \
    $$files(network/*.h) \
    $$files(resource/*.h) \
    $$files(server/*.h) \
    $$files(ui/*.h)

QML_FILES += \
    ../Gui/Dialog.qml \
    ../Gui/FitInView.qml \
    ../Gui/LogEdit.qml \
    ../Gui/MetroButton.qml \
    ../Gui/Splash.qml \
    ../Gui/TileButton.qml \
    ../Gui/ToolTipArea.qml \
    ../Gui/qmldir

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

#defineTest(generateIncludeDir) {
#    for(file, HEADERS) {
#        !equals(file, "cpch.h") {
#            content = $$cat($$file)
#            prev_word1 =
#            prev_word2 =
#            class_names =
#            for (word, content) {
#                equals(prev_word1, "class"): equals(prev_word2, "MCD_EXPORT") {
#                    class_names += $$word
#                }
#                prev_word1 = $$prev_word2
#                prev_word2 = $$word
#            }
#            copy($$file, ../include/)
#            content = "$${LITERAL_HASH}include \"$$basename(file)\""
#            for (class_name, class_names): write_file(../include/$$class_name, content)
#        }
#    }
#}

##todo: we should make this run in make period, but not qmake period
#!bulid_pass: generateIncludeDir()

for (file, HEADERS) {
    !equals(file, "cpch.h") {
        QMAKE_POST_LINK += $$QMAKE_COPY $$system_path($$_PRO_FILE_PWD_/$$file) $$system_path($$_PRO_FILE_PWD_/../include/) &
    }
}

contains(QMAKE_HOST.os, "Windows") : QMAKE_POST_LINK += cmd /c cscript /nologo $$_PRO_FILE_PWD_/../include/AutoGenerateHeader.vbs
else : QMAKE_POST_LINK += sh $$_PRO_FILE_PWD_/../include/AutoGenerateHeader.sh

LIBS += -L$$PWD/../lib -lbreakpad -loggvorbis

DISTFILES = ../Gui/qmldir

qmldir.files = ../Gui/qmldir
unix: !android {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
