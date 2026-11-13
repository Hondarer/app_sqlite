#define MOCK_SQLITE3_DISABLE_CALLER_INJECTION
#include <mock_sqlite3.h>
#undef MOCK_SQLITE3_DISABLE_CALLER_INJECTION

#include <cstdio>
#include <type_traits>

extern "C"
{
    const char sqlite3_app_sqlite3_version[] = SQLITE_VERSION;
    char *sqlite3_app_sqlite3_temp_directory = nullptr;
    char *sqlite3_app_sqlite3_data_directory = nullptr;
}

Mock_sqlite3 *_mock_sqlite3 = nullptr;

namespace
{

template <typename T> void trace_result(const char *func, const char *caller_file, const int caller_line,
                                       const char *caller_function, const T value)
{
    if (getTraceLevel() <= TRACE_NONE)
    {
        return;
    }

    std::printf("  > %s (%s:%d %s)", func, caller_file, caller_line, caller_function);
    if (getTraceLevel() >= TRACE_DETAIL)
    {
        if constexpr (std::is_pointer_v<T>)
        {
            std::printf(" -> 0x%p\n", (const void *)value);
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            std::printf(" -> %f\n", (double)value);
        }
        else
        {
            std::printf(" -> %lld\n", (long long)value);
        }
    }
    else
    {
        std::printf("\n");
    }
}

void trace_void(const char *func, const char *caller_file, const int caller_line, const char *caller_function)
{
    if (getTraceLevel() > TRACE_NONE)
    {
        std::printf("  > %s (%s:%d %s)\n", func, caller_file, caller_line, caller_function);
    }
}

} // namespace

#if !defined(_WIN32)
    #define MOCK_SQLITE3_IMPL(return_type, name, ...) MOCK_WEAK_IMPL(return_type, name, __VA_ARGS__)
#else
    #define MOCK_SQLITE3_IMPL(return_type, name, ...) extern "C" return_type SQLITE_APICALL name(__VA_ARGS__)
#endif

#define MOCK_SQLITE3_EXPAND(...) __VA_ARGS__

#define MOCK_SQLITE3_RET(return_type, original_name, api_name, parameters, arguments, mock_parameters, mock_arguments, matchers) \
    return_type delegate_real_##api_name mock_parameters \
    { \
        (void)caller_file; \
        (void)caller_line; \
        (void)caller_function; \
        static auto real_fn = reinterpret_cast<decltype(&api_name)>(resolveSharedSymbolOrExit(kLibSqlite3Name, #api_name)); \
        return real_fn arguments; \
    } \
    MOCK_SQLITE3_IMPL(return_type, api_name, MOCK_SQLITE3_EXPAND parameters) \
    { \
        const char *caller_file = "<unknown>"; \
        const int caller_line = 0; \
        const char *caller_function = "<unknown>"; \
        return mock_##api_name mock_arguments; \
    } \
    extern "C" return_type mock_##api_name mock_parameters \
    { \
        return_type return_value; \
        if (_mock_sqlite3 != nullptr) \
            return_value = _mock_sqlite3->api_name mock_arguments; \
        else \
            return_value = delegate_real_##api_name mock_arguments; \
        trace_result(#api_name, caller_file, caller_line, caller_function, return_value); \
        return return_value; \
    }

#define MOCK_SQLITE3_VOID(return_type, original_name, api_name, parameters, arguments, mock_parameters, mock_arguments, matchers) \
    return_type delegate_real_##api_name mock_parameters \
    { \
        (void)caller_file; \
        (void)caller_line; \
        (void)caller_function; \
        static auto real_fn = reinterpret_cast<decltype(&api_name)>(resolveSharedSymbolOrExit(kLibSqlite3Name, #api_name)); \
        real_fn arguments; \
    } \
    MOCK_SQLITE3_IMPL(return_type, api_name, MOCK_SQLITE3_EXPAND parameters) \
    { \
        const char *caller_file = "<unknown>"; \
        const int caller_line = 0; \
        const char *caller_function = "<unknown>"; \
        mock_##api_name mock_arguments; \
    } \
    extern "C" return_type mock_##api_name mock_parameters \
    { \
        if (_mock_sqlite3 != nullptr) \
            _mock_sqlite3->api_name mock_arguments; \
        else \
            delegate_real_##api_name mock_arguments; \
        trace_void(#api_name, caller_file, caller_line, caller_function); \
    }

#include <mock_sqlite3_api_table.h>

#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET
#undef MOCK_SQLITE3_EXPAND
#undef MOCK_SQLITE3_IMPL

Mock_sqlite3::Mock_sqlite3()
{
#define MOCK_SQLITE3_RET(return_type, original_name, api_name, parameters, arguments, mock_parameters, mock_arguments, matchers) \
    ON_CALL(*this, api_name matchers).WillByDefault(Invoke(delegate_real_##api_name));
#define MOCK_SQLITE3_VOID(return_type, original_name, api_name, parameters, arguments, mock_parameters, mock_arguments, matchers) \
    ON_CALL(*this, api_name matchers).WillByDefault(Invoke(delegate_real_##api_name));
#include <mock_sqlite3_api_table.h>
#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET

    ON_CALL(*this, sqlite3_app_sqlite3_config(_, _, _, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_app_sqlite3_config));
    ON_CALL(*this, sqlite3_app_sqlite3_db_config(_, _, _, _, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_app_sqlite3_db_config));
    ON_CALL(*this, sqlite3_app_sqlite3_mprintf(_, _, _, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_app_sqlite3_mprintf));
    ON_CALL(*this, sqlite3_app_sqlite3_snprintf(_, _, _, _, _, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_app_sqlite3_snprintf));
    ON_CALL(*this, sqlite3_app_sqlite3_str_appendf(_, _, _, _, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_app_sqlite3_str_appendf));
    ON_CALL(*this, sqlite3_app_sqlite3_log(_, _, _, _, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_app_sqlite3_log));
    ON_CALL(*this, sqlite3_app_sqlite3_test_control(_, _, _, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_app_sqlite3_test_control));
    ON_CALL(*this, sqlite3_app_sqlite3_vtab_config(_, _, _, _, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_app_sqlite3_vtab_config));
    TESTFW_REGISTER_MOCK_INSTANCE(_mock_sqlite3);
}

Mock_sqlite3::~Mock_sqlite3()
{
    TESTFW_UNREGISTER_MOCK_INSTANCE(_mock_sqlite3);
}
