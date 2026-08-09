#ifndef MOCK_SQLITE3_H
#define MOCK_SQLITE3_H

#include <sqlite3.h>
#include <testfw.h>

#include <cstdarg>

inline constexpr char kLibSqlite3Name[] = "libsqlite3" TESTFW_SHARED_LIBRARY_EXTENSION;

#define MOCK_SQLITE3_RET(return_type, name, parameters, arguments, matchers) \
    extern return_type delegate_real_##name parameters;
#define MOCK_SQLITE3_VOID(return_type, name, parameters, arguments, matchers) \
    extern return_type delegate_real_##name parameters;
#include <mock_sqlite3_api_table.h>
#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET

extern int delegate_real_sqlite3_config(int operation, va_list args);
extern int delegate_real_sqlite3_db_config(sqlite3 *database, int operation, va_list args);
extern char *delegate_real_sqlite3_mprintf(const char *format, va_list args);
extern char *delegate_real_sqlite3_snprintf(int size, char *buffer, const char *format, va_list args);
extern void delegate_real_sqlite3_str_appendf(sqlite3_str *string, const char *format, va_list args);
extern void delegate_real_sqlite3_log(int error_code, const char *format, va_list args);
extern int delegate_real_sqlite3_test_control(int operation, va_list args);
extern int delegate_real_sqlite3_vtab_config(sqlite3 *database, int operation, va_list args);

class Mock_sqlite3
{
  public:
#define MOCK_SQLITE3_RET(return_type, name, parameters, arguments, matchers) MOCK_METHOD(return_type, name, parameters);
#define MOCK_SQLITE3_VOID(return_type, name, parameters, arguments, matchers) \
    MOCK_METHOD(return_type, name, parameters);
#include <mock_sqlite3_api_table.h>
#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET

    MOCK_METHOD(int, sqlite3_config, (int, va_list));
    MOCK_METHOD(int, sqlite3_db_config, (sqlite3 *, int, va_list));
    MOCK_METHOD(char *, sqlite3_mprintf, (const char *, va_list));
    MOCK_METHOD(char *, sqlite3_snprintf, (int, char *, const char *, va_list));
    MOCK_METHOD(void, sqlite3_str_appendf, (sqlite3_str *, const char *, va_list));
    MOCK_METHOD(void, sqlite3_log, (int, const char *, va_list));
    MOCK_METHOD(int, sqlite3_test_control, (int, va_list));
    MOCK_METHOD(int, sqlite3_vtab_config, (sqlite3 *, int, va_list));

    Mock_sqlite3();
    ~Mock_sqlite3();
};

extern Mock_sqlite3 *_mock_sqlite3;

#endif /* MOCK_SQLITE3_H */
