ifdef PLATFORM_WINDOWS
    # SQLite の DLL import ではなく mock_sqlite3 が提供する実シンボルを参照する。
    DEFINES += SQLITE_API=
endif

LIBS += mock_sqlite3
DEFINES += MOCK_SQLITE3_ENABLE_CALLER
TEST_SRCS += $(MYAPP_DIR)/test/src/libsqlite3Test/mockSqlite3Test/caller_info.c \
    $(MYAPP_DIR)/test/src/libsqlite3Test/mockSqlite3Test/caller_info_cpp.cc
