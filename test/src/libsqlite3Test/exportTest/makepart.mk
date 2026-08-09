LIBS += sqlite3

ifdef PLATFORM_LINUX
    LIBS += dl
endif
