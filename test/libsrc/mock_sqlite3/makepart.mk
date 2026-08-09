ifdef PLATFORM_WINDOWS
    # SQLite の DLL import ではなくモックの実シンボルを定義する。
    DEFINES += SQLITE_API=

    # 多数の MOCK_METHOD / ON_CALL を含むオブジェクトを COFF の通常上限内へ収める必要がないようにする。
    CXXFLAGS += /bigobj
endif
