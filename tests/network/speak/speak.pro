TEMPLATE = app

QT += qml quick

SOURCES += main.cpp

RESOURCES +=

CARDIRECTOR = $$PWD/../../..

INCLUDEPATH += $$CARDIRECTOR/include

LIBS += -L"$$CARDIRECTOR/lib"

LIBS += -l$$qtLibraryTarget(Cardirector)
