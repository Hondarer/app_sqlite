#ifndef MOCK_OVERRIDE_SQLITE3_SQLITE3EXT_H
#define MOCK_OVERRIDE_SQLITE3_SQLITE3EXT_H

#if !defined(MOCK_SQLITE3_ENABLE_CALLER) || defined(MOCK_SQLITE3_IN_OVERRIDE_REAL_HEADERS)
    #if defined(_WIN32)
        /* see: https://learn.microsoft.com/en-us/cpp/preprocessor/warning */
        #pragma warning(push, 0)
        #include "../../prod/include/sqlite3ext.h"
        #pragma warning(pop)
    #else
        #include_next <sqlite3ext.h>
    #endif
#else
    #define MOCK_SQLITE3_IN_OVERRIDE_REAL_HEADERS
    #if defined(_WIN32)
        /* see: https://learn.microsoft.com/en-us/cpp/preprocessor/warning */
        #pragma warning(push, 0)
        #include "../../prod/include/sqlite3.h"
        #include "../../prod/include/sqlite3ext.h"
        #pragma warning(pop)
    #else
        #include_next <sqlite3.h>
        #include_next <sqlite3ext.h>
    #endif
    #undef MOCK_SQLITE3_IN_OVERRIDE_REAL_HEADERS
    #define MOCK_SQLITE3_INJECT_CALLER
    #include <mock_sqlite3_overrides.h>
    #undef MOCK_SQLITE3_INJECT_CALLER
#endif

#endif /* MOCK_OVERRIDE_SQLITE3_SQLITE3EXT_H */
