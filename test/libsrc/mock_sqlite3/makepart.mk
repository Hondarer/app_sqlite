ifdef PLATFORM_WINDOWS
    # SQLite の DLL import ではなくモックの実シンボルを定義する。
    DEFINES += SQLITE_API=
endif
