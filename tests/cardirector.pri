CONFIG += c++11

CARDIRECTOR = $$PWD/..

INCLUDEPATH += $$CARDIRECTOR/include

LIBS += -L"$$CARDIRECTOR/lib"

LIBS += -l$$qtLibraryTarget(Cardirector)
