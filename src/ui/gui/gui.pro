TEMPLATE = lib
TARGET = gui
QT += qml quick
CONFIG += qt plugin

DESTDIR = bin

TARGET = $$qtLibraryTarget($$TARGET)

uri = Cardirector.Gui

INCLUDEPATH += \
    ../../common

# Input
SOURCES += \
    cguiplugin.cpp

HEADERS += \
    cguiplugin.h

QML_FILES += \
    MainWindow.qml

DISTFILES = qmldir $$QML_FILES

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

# Create the resource file
GENERATED_RESOURCE_FILE = $$OUT_PWD/gui.qrc

INCLUDED_RESOURCE_FILES = $$QML_FILES

RESOURCE_CONTENT = \
    "<RCC>" \
    "<qresource prefix=\"/Cardirector/Gui\">"

for(resourcefile, INCLUDED_RESOURCE_FILES) {
    resourcefileabsolutepath = $$absolute_path($$resourcefile)
    relativepath_in = $$relative_path($$resourcefileabsolutepath, $$_PRO_FILE_PWD_)
    relativepath_out = $$relative_path($$resourcefileabsolutepath, $$OUT_PWD)
    RESOURCE_CONTENT += "<file alias=\"$$relativepath_in\">$$relativepath_out</file>"
}

RESOURCE_CONTENT += \
    "</qresource>" \
    "</RCC>"

write_file($$GENERATED_RESOURCE_FILE, RESOURCE_CONTENT)|error("Aborting.")

RESOURCES += $$GENERATED_RESOURCE_FILE

defineTest(copy) {
    file = $$1
    path = $$2
    win32 {
        system("copy $$replace(file, /, \\) $$replace(path, /, \\)")
    }
    else {
        system("cp $$file $$path")
    }

    return(true)
}

build_pass {
    win32-msvc* {
        dest = ../../../Gui/
        copy($$OUT_PWD/$$DESTDIR/$$TARGET".dll", $$dest)
        copy($$OUT_PWD/$$DESTDIR/$$TARGET".pdb", $$dest)
        copy(qmldir, $$dest)
        for(file, QML_FILES) {
            copy($$file, $$dest)
        }
    }
}
