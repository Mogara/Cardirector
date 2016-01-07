TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = breakpad ogg vorbis Cardirector

breakpad.subdir = src/3rdparty/breakpad
ogg.subdir = src/3rdparty/ogg
vorbis.subdir = src/3rdparty/vorbis
Cardirector.file = src/Cardirector.pro
