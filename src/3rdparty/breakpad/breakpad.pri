#win32-msvc*{
win32 {
    CONFIG(release, debug|release) {
        win32-msvc* {
            QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
        }
        win32-g++ {
            QMAKE_CXXFLAGS += -g
        }
        DEFINES += USE_BREAKPAD

        SOURCES += \
            $$PWD/client/windows/crash_generation/client_info.cc \
            $$PWD/client/windows/crash_generation/crash_generation_client.cc \
            $$PWD/client/windows/crash_generation/crash_generation_server.cc \
            $$PWD/client/windows/crash_generation/minidump_generator.cc \
            $$PWD/client/windows/handler/exception_handler.cc \
            $$PWD/common/windows/guid_string.cc

        HEADERS += \
            $$PWD/client/windows/crash_generation/client_info.h \
            $$PWD/client/windows/crash_generation/crash_generation_client.h \
            $$PWD/client/windows/crash_generation/crash_generation_server.h \
            $$PWD/client/windows/crash_generation/minidump_generator.h \
            $$PWD/client/windows/handler/exception_handler.h \
            $$PWD/common/windows/guid_string.h

        INCLUDEPATH += \
            $$PWD \
            $$PWD/client/windows
    }
}

osx {
    CONFIG(release, debug|release) {
        QMAKE_LFLAGS += -framework CoreFoundation

        SOURCES += \
            $$PWD/client/mac/crash_generation/crash_generation_client.cc \
            $$PWD/client/mac/crash_generation/crash_generation_server.cc \
            $$PWD/client/mac/handler/exception_handler.cc \
            $$PWD/client/mac/handler/minidump_generator.cc \
            $$PWD/client/mac/handler/breakpad_nlist_64.cc \
            $$PWD/client/mac/handler/dynamic_images.cc \
            $$PWD/client/mac/handler/protected_memory_allocator.cc \
            $$PWD/client/minidump_file_writer.cc \
            $$PWD/common/convert_UTF.c \
            $$PWD/common/md5.cc \
            $$PWD/common/string_conversion.cc \
            $$PWD/common/mac/arch_utilities.cc \
            $$PWD/common/mac/bootstrap_compat.cc \
            $$PWD/common/mac/file_id.cc \
            $$PWD/common/mac/launch_reporter.cc \
            $$PWD/common/mac/macho_id.cc \
            $$PWD/common/mac/macho_reader.cc \
            $$PWD/common/mac/macho_utilities.cc \
            $$PWD/common/mac/macho_walker.cc \
            $$PWD/common/mac/string_utilities.cc

        OBJECTIVE_SOURCES += \
            $$PWD/common/mac/dump_syms.mm \
            $$PWD/common/mac/HTTPMultipartUpload.m \
            $$PWD/common/mac/MachIPC.mm

        HEADERS += \
            $$PWD/client/mac/handler/exception_handler.h


        QMAKE_CXXFLAGS += -g
        DEFINES += USE_BREAKPAD
        INCLUDEPATH += \
            $$PWD \
            $$PWD/client/mac
            $$PWD/client/apple
    }
}

linux{
    CONFIG(release, debug|release) {
        SOURCES += \
            $$PWD/client/linux/crash_generation/crash_generation_client.cc \
            $$PWD/client/linux/crash_generation/crash_generation_server.cc \
            $$PWD/client/linux/dump_writer_common/thread_info.cc \
            $$PWD/client/linux/dump_writer_common/ucontext_reader.cc \
            $$PWD/client/linux/handler/exception_handler.cc \
            $$PWD/client/linux/handler/minidump_descriptor.cc \
            $$PWD/client/linux/log/log.cc \
            $$PWD/client/linux/microdump_writer/microdump_writer.cc \
            $$PWD/client/linux/minidump_writer/linux_dumper.cc \
            $$PWD/client/linux/minidump_writer/linux_ptrace_dumper.cc \
            $$PWD/client/linux/minidump_writer/minidump_writer.cc \
            $$PWD/client/minidump_file_writer.cc \
            $$PWD/common/convert_UTF.c \
            $$PWD/common/md5.cc \
            $$PWD/common/string_conversion.cc \
            $$PWD/common/linux/elfutils.cc \
            $$PWD/common/linux/file_id.cc \
            $$PWD/common/linux/guid_creator.cc \
            $$PWD/common/linux/linux_libc_support.cc \
            $$PWD/common/linux/memory_mapped_file.cc \
            $$PWD/common/linux/safe_readlink.cc

        HEADERS += \
            $$PWD/client/linux/handler/exception_handler.h

        QMAKE_CXXFLAGS += -g
        DEFINES += USE_BREAKPAD
        INCLUDEPATH += \
            $$PWD \
            $$PWD/client/linux

        android{
            DEFINES += __STDINT_LIMITS
            CONFIG(release, debug|release) {
                SOURCES += $$PWD/common/android/breakpad_getcontext.S
                INCLUDEPATH += $$PWD/common/android/include
            }
        }
    }
}
