INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/breakpad/src

# Windows
win32:HEADERS += $$PWD/breakpad/src/common/windows/string_utils-inl.h
win32:HEADERS += $$PWD/breakpad/src/common/windows/guid_string.h
win32:HEADERS += $$PWD/breakpad/src/client/windows/handler/exception_handler.h
win32:HEADERS += $$PWD/breakpad/src/client/windows/common/ipc_protocol.h
win32:HEADERS += $$PWD/breakpad/src/google_breakpad/common/minidump_format.h
win32:HEADERS += $$PWD/breakpad/src/google_breakpad/common/breakpad_types.h
win32:HEADERS += $$PWD/breakpad/src/client/windows/crash_generation/crash_generation_client.h
win32:HEADERS += $$PWD/breakpad/src/common/scoped_ptr.h
win32:SOURCES += $$PWD/breakpad/src/client/windows/handler/exception_handler.cc
win32:SOURCES += $$PWD/breakpad/src/common/windows/string_utils.cc
win32:SOURCES += $$PWD/breakpad/src/common/windows/guid_string.cc
win32:SOURCES += $$PWD/breakpad/src/client/windows/crash_generation/crash_generation_client.cc
