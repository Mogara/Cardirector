TEMPLATE = subdirs

SUBDIRS = breakpad oggvorbis Cardirector

breakpad.subdir = src/3rdparty/breakpad
oggvorbis.subdir = src/3rdparty/oggvorbis
Cardirector.depends = breakpad oggvorbis
Cardirector.file = src/Cardirector.pro
