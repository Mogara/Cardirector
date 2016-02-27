TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = breakpad oggvorbis Cardirector

breakpad.subdir = src/3rdparty/breakpad
oggvorbis.subdir = src/3rdparty/oggvorbis
Cardirector.file = src/Cardirector.pro
