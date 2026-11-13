#include <sqlite3.h>

const char *namespace_test_caller_file(void);
int namespace_test_caller_line(void);
int namespace_test_direct_call(void);
int namespace_test_pointer_call(void);

const char *namespace_test_caller_file(void)
{
    return __FILE__;
}
/* 呼び出し行の照合に使うため、次の 2 定義を隣接させます。 */
// clang-format off
int namespace_test_caller_line(void) { return __LINE__ + 1; }
int namespace_test_direct_call(void) { return sqlite3_app_sqlite3_initialize(); }
// clang-format on
int namespace_test_pointer_call(void)
{
    int (*api)(void) = &sqlite3_app_sqlite3_initialize;
    return api();
}

int namespace_test_variadic_line(void);
int namespace_test_variadic_call(void);
/* 呼び出し行の照合に使うため、次の 2 定義を隣接させます。 */
// clang-format off
int namespace_test_variadic_line(void) { return __LINE__ + 1; }
int namespace_test_variadic_call(void) { return sqlite3_app_sqlite3_config(SQLITE_CONFIG_SERIALIZED); }
// clang-format on
