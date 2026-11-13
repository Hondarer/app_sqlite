/* 可変長 API は通常の関数 API 表と分けて管理する。 */
#ifndef MOCK_VARIADIC
    #error MOCK_VARIADIC must be defined
#endif

MOCK_VARIADIC(int, sqlite3_config, sqlite3_app_sqlite3_config, (int operation), (operation), (const char *caller_file, int caller_line, const char *caller_function, int operation, va_list args), (caller_file, caller_line, caller_function, operation, args))
MOCK_VARIADIC(int, sqlite3_db_config, sqlite3_app_sqlite3_db_config, (sqlite3 *database, int operation), (database, operation), (const char *caller_file, int caller_line, const char *caller_function, sqlite3 *database, int operation, va_list args), (caller_file, caller_line, caller_function, database, operation, args))
MOCK_VARIADIC(char *, sqlite3_mprintf, sqlite3_app_sqlite3_mprintf, (const char *format), (format), (const char *caller_file, int caller_line, const char *caller_function, const char *format, va_list args), (caller_file, caller_line, caller_function, format, args))
MOCK_VARIADIC(char *, sqlite3_snprintf, sqlite3_app_sqlite3_snprintf, (int size, char *buffer, const char *format), (size, buffer, format), (const char *caller_file, int caller_line, const char *caller_function, int size, char *buffer, const char *format, va_list args), (caller_file, caller_line, caller_function, size, buffer, format, args))
MOCK_VARIADIC(void, sqlite3_str_appendf, sqlite3_app_sqlite3_str_appendf, (sqlite3_str *string, const char *format), (string, format), (const char *caller_file, int caller_line, const char *caller_function, sqlite3_str *string, const char *format, va_list args), (caller_file, caller_line, caller_function, string, format, args))
MOCK_VARIADIC(void, sqlite3_log, sqlite3_app_sqlite3_log, (int error_code, const char *format), (error_code, format), (const char *caller_file, int caller_line, const char *caller_function, int error_code, const char *format, va_list args), (caller_file, caller_line, caller_function, error_code, format, args))
MOCK_VARIADIC(int, sqlite3_test_control, sqlite3_app_sqlite3_test_control, (int operation), (operation), (const char *caller_file, int caller_line, const char *caller_function, int operation, va_list args), (caller_file, caller_line, caller_function, operation, args))
MOCK_VARIADIC(int, sqlite3_vtab_config, sqlite3_app_sqlite3_vtab_config, (sqlite3 *database, int operation), (database, operation), (const char *caller_file, int caller_line, const char *caller_function, sqlite3 *database, int operation, va_list args), (caller_file, caller_line, caller_function, database, operation, args))
