win32-msvc*{
    CONFIG(release, debug|release) {
        QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
        DEFINES += USE_BREAKPAD

        SOURCES += \
            $$PWD/breakpad/client/windows/crash_generation/client_info.cc \
            $$PWD/breakpad/client/windows/crash_generation/crash_generation_client.cc \
            $$PWD/breakpad/client/windows/crash_generation/crash_generation_server.cc \
            $$PWD/breakpad/client/windows/crash_generation/minidump_generator.cc \
            $$PWD/breakpad/client/windows/handler/exception_handler.cc \
            $$PWD/breakpad/common/windows/guid_string.cc

        HEADERS += \
            $$PWD/breakpad/client/windows/crash_generation/client_info.h \
            $$PWD/breakpad/client/windows/crash_generation/crash_generation_client.h \
            $$PWD/breakpad/client/windows/crash_generation/crash_generation_server.h \
            $$PWD/breakpad/client/windows/crash_generation/minidump_generator.h \
            $$PWD/breakpad/client/windows/handler/exception_handler.h \
            $$PWD/breakpad/common/windows/guid_string.h

        INCLUDEPATH += \
            $$PWD/breakpad \
            $$PWD/breakpad/client/windows
    }
}

linux{
    CONFIG(release, debug|release) {
        SOURCES += \
            $$PWD/breakpad/client/linux/crash_generation/crash_generation_client.cc \
            $$PWD/breakpad/client/linux/crash_generation/crash_generation_server.cc \
            $$PWD/breakpad/client/linux/dump_writer_common/seccomp_unwinder.cc \
            $$PWD/breakpad/client/linux/dump_writer_common/thread_info.cc \
            $$PWD/breakpad/client/linux/dump_writer_common/ucontext_reader.cc \
            $$PWD/breakpad/client/linux/handler/exception_handler.cc \
            $$PWD/breakpad/client/linux/handler/minidump_descriptor.cc \
            $$PWD/breakpad/client/linux/log/log.cc \
            $$PWD/breakpad/client/linux/microdump_writer/microdump_writer.cc \
            $$PWD/breakpad/client/linux/minidump_writer/linux_dumper.cc \
            $$PWD/breakpad/client/linux/minidump_writer/linux_ptrace_dumper.cc \
            $$PWD/breakpad/client/linux/minidump_writer/minidump_writer.cc \
            $$PWD/breakpad/client/minidump_file_writer.cc \
            $$PWD/breakpad/common/convert_UTF.c \
            $$PWD/breakpad/common/md5.cc \
            $$PWD/breakpad/common/string_conversion.cc \
            $$PWD/breakpad/common/linux/elfutils.cc \
            $$PWD/breakpad/common/linux/file_id.cc \
            $$PWD/breakpad/common/linux/guid_creator.cc \
            $$PWD/breakpad/common/linux/linux_libc_support.cc \
            $$PWD/breakpad/common/linux/memory_mapped_file.cc \
            $$PWD/breakpad/common/linux/safe_readlink.cc

        HEADERS += \
            $$PWD/breakpad/client/linux/handler/exception_handler.h

        QMAKE_CXXFLAGS += -g
        DEFINES += USE_BREAKPAD
        INCLUDEPATH += \
            $$PWD/breakpad \
            $$PWD/breakpad/client/linux
    }

    android{
        CONFIG(release, debug|release) {
            SOURCES += $$PWD/breakpad/common/android/breakpad_getcontext.S
            INCLUDEPATH += $$PWD/breakpad/common/android/include
        }
    }
}
