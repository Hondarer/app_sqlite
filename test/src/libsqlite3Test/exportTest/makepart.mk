# ライブラリ全体の公開シンボルを検査する黒箱テストのため、TEST_SRCS は指定しない。
LIBS += sqlite3

ifdef PLATFORM_LINUX
    DEFINES += PLATFORM_LINUX
    LIBS += dl
else ifdef PLATFORM_WINDOWS
    DEFINES += PLATFORM_WINDOWS
endif
