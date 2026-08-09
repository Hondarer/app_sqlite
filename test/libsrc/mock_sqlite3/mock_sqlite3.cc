#include <mock_sqlite3.h>

#include <cstdio>

Mock_sqlite3 *_mock_sqlite3 = nullptr;

namespace
{

void trace_call(const char *function_name)
{
    if (getTraceLevel() > TRACE_NONE)
    {
        std::printf("  > %s\n", function_name);
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
        static auto real_function = \
            reinterpret_cast<decltype(&name)>(resolveSharedSymbolOrExit(kLibSqlite3Name, #name)); \
        return real_function arguments; \
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
        trace_call(__func__); \
        return return_value; \
    }

#define MOCK_SQLITE3_VOID(return_type, name, parameters, arguments, matchers) \
    return_type delegate_real_##name parameters \
    { \
        static auto real_function = \
            reinterpret_cast<decltype(&name)>(resolveSharedSymbolOrExit(kLibSqlite3Name, #name)); \
        real_function arguments; \
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
        trace_call(__func__); \
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
