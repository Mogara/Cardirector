#-------------------------------------------------
#
# Project created by QtCreator 2015-01-24T00:55:59
#
#-------------------------------------------------
TARGET = Cardirector
TEMPLATE = lib
CONFIG += precompile_header
CONFIG += staticlib

QT += quick multimedia
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

CONFIG += precompile_header
# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = src/cpch.h

TARGET = $$qtLibraryTarget($$TARGET)

include (./Cardirector.pri)

defineTest(copy) {
    file = $$1
    path = $$2
    !exists($$file): return(false)
    system("$$QMAKE_COPY $$system_path($$file) $$system_path($$path)")

    return(true)
}

!build_pass {
    for(file, HEADERS) {
        !equals(file, "src/cpch.h") {
            copy($$file, include/)
        }
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
