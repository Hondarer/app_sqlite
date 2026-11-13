#ifndef MOCK_SQLITE3_H
#define MOCK_SQLITE3_H

#include <sqlite3.h>
#include <testfw.h>
#include <mock_sqlite3_overrides.h>

#include <cstdarg>

inline constexpr char kLibSqlite3Name[] = "libsqlite3_app" TESTFW_SHARED_LIBRARY_EXTENSION;

#define MOCK_SQLITE3_RET(return_type, original_name, api_name, parameters, arguments, mock_parameters, mock_arguments, matchers) \
    extern return_type delegate_real_##api_name mock_parameters;
#define MOCK_SQLITE3_VOID(return_type, original_name, api_name, parameters, arguments, mock_parameters, mock_arguments, matchers) \
    extern return_type delegate_real_##api_name mock_parameters;
#include <mock_sqlite3_api_table.h>
#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET

#define MOCK_VARIADIC(return_type, original_name, api_name, parameters, arguments, mock_parameters, mock_arguments) \
    extern return_type delegate_real_##api_name mock_parameters;
#include <mock_sqlite3_variadic_api_table.h>
#undef MOCK_VARIADIC

class Mock_sqlite3
{
  public:
#define MOCK_SQLITE3_RET(return_type, original_name, api_name, parameters, arguments, mock_parameters, mock_arguments, matchers) MOCK_METHOD(return_type, api_name, mock_parameters);
#define MOCK_SQLITE3_VOID(return_type, original_name, api_name, parameters, arguments, mock_parameters, mock_arguments, matchers) \
    MOCK_METHOD(return_type, api_name, mock_parameters);
#include <mock_sqlite3_api_table.h>
#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET

#define MOCK_VARIADIC(return_type, original_name, api_name, parameters, arguments, mock_parameters, mock_arguments) \
    MOCK_METHOD(return_type, api_name, mock_parameters);
#include <mock_sqlite3_variadic_api_table.h>
#undef MOCK_VARIADIC

    Mock_sqlite3();
    virtual ~Mock_sqlite3();
};

extern Mock_sqlite3 *_mock_sqlite3;

#endif /* MOCK_SQLITE3_H */
