#include <mock_sqlite3.h>

#include <cstdio>
#include <type_traits>

extern "C"
{
    const char sqlite3_version[] = SQLITE_VERSION;
    char *sqlite3_temp_directory = nullptr;
    char *sqlite3_data_directory = nullptr;
}

Mock_sqlite3 *_mock_sqlite3 = nullptr;

namespace
{

template <typename T> void trace_result(const char *func, const T value)
{
    if (getTraceLevel() <= TRACE_NONE)
    {
        return;
    }

    std::printf("  > %s", func);
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

void trace_void(const char *func)
{
    if (getTraceLevel() > TRACE_NONE)
    {
        std::printf("  > %s\n", func);
    }
}

} // namespace

#if !defined(_WIN32)
    #define MOCK_SQLITE3_IMPL(return_type, name, ...) MOCK_WEAK_IMPL(return_type, name, __VA_ARGS__)
#else
    #define MOCK_SQLITE3_IMPL(return_type, name, ...) extern "C" return_type SQLITE_APICALL name(__VA_ARGS__)
#endif

#define MOCK_SQLITE3_EXPAND(...) __VA_ARGS__

#define MOCK_SQLITE3_RET(return_type, name, parameters, arguments, matchers) \
    return_type delegate_real_##name parameters \
    { \
        static auto real_fn = reinterpret_cast<decltype(&name)>(resolveSharedSymbolOrExit(kLibSqlite3Name, #name)); \
        return real_fn arguments; \
    } \
    MOCK_SQLITE3_IMPL(return_type, name, MOCK_SQLITE3_EXPAND parameters) \
    { \
        return_type return_value; \
        if (_mock_sqlite3 != nullptr) \
        { \
            return_value = _mock_sqlite3->name arguments; \
        } \
        else \
        { \
            return_value = delegate_real_##name arguments; \
        } \
        trace_result(__func__, return_value); \
        return return_value; \
    }

#define MOCK_SQLITE3_VOID(return_type, name, parameters, arguments, matchers) \
    return_type delegate_real_##name parameters \
    { \
        static auto real_fn = reinterpret_cast<decltype(&name)>(resolveSharedSymbolOrExit(kLibSqlite3Name, #name)); \
        real_fn arguments; \
    } \
    MOCK_SQLITE3_IMPL(return_type, name, MOCK_SQLITE3_EXPAND parameters) \
    { \
        if (_mock_sqlite3 != nullptr) \
        { \
            _mock_sqlite3->name arguments; \
        } \
        else \
        { \
            delegate_real_##name arguments; \
        } \
        trace_void(__func__); \
    }

#include <mock_sqlite3_api_table.h>

#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET
#undef MOCK_SQLITE3_EXPAND
#undef MOCK_SQLITE3_IMPL

Mock_sqlite3::Mock_sqlite3()
{
#define MOCK_SQLITE3_RET(return_type, name, parameters, arguments, matchers) \
    ON_CALL(*this, name matchers).WillByDefault(Invoke(delegate_real_##name));
#define MOCK_SQLITE3_VOID(return_type, name, parameters, arguments, matchers) \
    ON_CALL(*this, name matchers).WillByDefault(Invoke(delegate_real_##name));
#include <mock_sqlite3_api_table.h>
#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET

    ON_CALL(*this, sqlite3_config(_, _)).WillByDefault(Invoke(delegate_real_sqlite3_config));
    ON_CALL(*this, sqlite3_db_config(_, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_db_config));
    ON_CALL(*this, sqlite3_mprintf(_, _)).WillByDefault(Invoke(delegate_real_sqlite3_mprintf));
    ON_CALL(*this, sqlite3_snprintf(_, _, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_snprintf));
    ON_CALL(*this, sqlite3_str_appendf(_, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_str_appendf));
    ON_CALL(*this, sqlite3_log(_, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_log));
    ON_CALL(*this, sqlite3_test_control(_, _)).WillByDefault(Invoke(delegate_real_sqlite3_test_control));
    ON_CALL(*this, sqlite3_vtab_config(_, _, _)).WillByDefault(Invoke(delegate_real_sqlite3_vtab_config));
    _mock_sqlite3 = this;
}

Mock_sqlite3::~Mock_sqlite3()
{
    _mock_sqlite3 = nullptr;
}
