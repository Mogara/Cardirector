TEMPLATE = lib
CONFIG += staticlib warn_off
DESTDIR = $$PWD/../../../lib

CONFIG(release, debug|release) {
    win32-msvc* {
        QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
    } else {
        QMAKE_CXXFLAGS += -g
    }

    SOURCES += \
        common/convert_UTF.c \
        common/md5.cc \
        common/string_conversion.cc \

    win32 {
        DEFINES += USE_BREAKPAD
        SOURCES += \
            client/windows/crash_generation/client_info.cc \
            client/windows/crash_generation/crash_generation_client.cc \
            client/windows/crash_generation/crash_generation_server.cc \
            client/windows/crash_generation/minidump_generator.cc \
            client/windows/handler/exception_handler.cc \
            common/windows/guid_string.cc

        INCLUDEPATH += \
            client/windows
    }

    osx {
        DEFINES += USE_BREAKPAD
        QMAKE_LFLAGS += -framework CoreFoundation -framework Foundation

        SOURCES += \
            client/mac/crash_generation/crash_generation_client.cc \
            client/mac/crash_generation/crash_generation_server.cc \
            client/mac/handler/exception_handler.cc \
            client/mac/handler/minidump_generator.cc \
            client/mac/handler/breakpad_nlist_64.cc \
            client/mac/handler/dynamic_images.cc \
            client/mac/handler/protected_memory_allocator.cc \
            common/mac/arch_utilities.cc \
            common/mac/bootstrap_compat.cc \
            common/mac/file_id.cc \
            common/mac/launch_reporter.cc \
            common/mac/macho_id.cc \
            common/mac/macho_reader.cc \
            common/mac/macho_utilities.cc \
            common/mac/macho_walker.cc \
            common/mac/string_utilities.cc

        SOURCES += \
            $$PWD/client/minidump_file_writer.cc \

        OBJECTIVE_SOURCES += \
            client/mac/crash_generation/ConfigFile.mm \
            client/mac/crash_generation/Inspector.mm \
#           client/mac/crash_generation/InspectorMain.mm \
            client/mac/Framework/Breakpad.mm \
            client/mac/Framework/OnDemandServer.mm \
#           common/mac/dump_syms.mm \
            common/mac/HTTPMultipartUpload.m \
            common/mac/MachIPC.mm

        INCLUDEPATH += \
            client/mac \
            client/apple/Framework \
            common/mac
    }

    linux{
        DEFINES += USE_BREAKPAD
        SOURCES += \
            client/linux/crash_generation/crash_generation_client.cc \
            client/linux/crash_generation/crash_generation_server.cc \
            client/linux/dump_writer_common/thread_info.cc \
            client/linux/dump_writer_common/ucontext_reader.cc \
            client/linux/handler/exception_handler.cc \
            client/linux/handler/minidump_descriptor.cc \
            client/linux/log/log.cc \
            client/linux/microdump_writer/microdump_writer.cc \
            client/linux/minidump_writer/linux_dumper.cc \
            client/linux/minidump_writer/linux_ptrace_dumper.cc \
            client/linux/minidump_writer/minidump_writer.cc \
            common/linux/elfutils.cc \
            common/linux/file_id.cc \
            common/linux/guid_creator.cc \
            common/linux/linux_libc_support.cc \
            common/linux/memory_mapped_file.cc \
            common/linux/safe_readlink.cc

        SOURCES += \
            client/minidump_file_writer.cc \

        INCLUDEPATH += \
            client/linux

        android{
            DEFINES += __STDINT_LIMITS
            CONFIG(release, debug|release) {
                SOURCES += common/android/breakpad_getcontext.S
                INCLUDEPATH += common/android/include
            }
        }
    }
}
