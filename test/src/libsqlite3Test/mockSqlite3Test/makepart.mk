ifdef PLATFORM_WINDOWS
    # SQLite の DLL import ではなく mock_sqlite3 が提供する実シンボルを参照する。
    DEFINES += SQLITE_API=
endif

LIBS += mock_sqlite3
