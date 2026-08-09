#include <mock_sqlite3.h>

#include <cstdarg>

namespace
{

using sqlite3_config_fn = int(SQLITE_CDECL *)(int, ...);
using sqlite3_db_config_fn = int(SQLITE_CDECL *)(sqlite3 *, int, ...);
using sqlite3_log_fn = void(SQLITE_CDECL *)(int, const char *, ...);
using sqlite3_test_control_fn = int(SQLITE_CDECL *)(int, ...);
using sqlite3_vtab_config_fn = int(SQLITE_CDECL *)(sqlite3 *, int, ...);
using log_callback_fn = void (*)(void *, int, const char *);
using sql_log_callback_fn = void (*)(void *, sqlite3 *, const char *, int);
using fault_callback_fn = int (*)(int);
using void_callback_fn = void (*)(void);
using localtime_callback_fn = int (*)(const void *, void *);
using vdbe_coverage_callback_fn = void (*)(void *, unsigned int, unsigned char, unsigned char);

template <typename FunctionType> FunctionType resolve_variadic(const char *name)
{
    return reinterpret_cast<FunctionType>(resolveSharedSymbolOrExit(kLibSqlite3Name, name));
}

bool is_db_flag_operation(int operation)
{
    switch (operation)
    {
    case SQLITE_DBCONFIG_ENABLE_FKEY:
    case SQLITE_DBCONFIG_ENABLE_TRIGGER:
    case SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER:
    case SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION:
    case SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE:
    case SQLITE_DBCONFIG_ENABLE_QPSG:
    case SQLITE_DBCONFIG_TRIGGER_EQP:
    case SQLITE_DBCONFIG_RESET_DATABASE:
    case SQLITE_DBCONFIG_DEFENSIVE:
    case SQLITE_DBCONFIG_WRITABLE_SCHEMA:
    case SQLITE_DBCONFIG_LEGACY_ALTER_TABLE:
    case SQLITE_DBCONFIG_DQS_DML:
    case SQLITE_DBCONFIG_DQS_DDL:
    case SQLITE_DBCONFIG_ENABLE_VIEW:
    case SQLITE_DBCONFIG_LEGACY_FILE_FORMAT:
    case SQLITE_DBCONFIG_TRUSTED_SCHEMA:
    case SQLITE_DBCONFIG_STMT_SCANSTATUS:
    case SQLITE_DBCONFIG_REVERSE_SCANORDER:
    case SQLITE_DBCONFIG_ENABLE_ATTACH_CREATE:
    case SQLITE_DBCONFIG_ENABLE_ATTACH_WRITE:
    case SQLITE_DBCONFIG_ENABLE_COMMENTS:
        return true;
    default:
        return false;
    }
}

} // namespace

int delegate_real_sqlite3_config(int operation, va_list args)
{
    static sqlite3_config_fn real_function = resolve_variadic<sqlite3_config_fn>("sqlite3_config");

    switch (operation)
    {
    case SQLITE_CONFIG_SINGLETHREAD:
    case SQLITE_CONFIG_MULTITHREAD:
    case SQLITE_CONFIG_SERIALIZED:
    case SQLITE_CONFIG_SCRATCH:
    case SQLITE_CONFIG_PCACHE:
    case SQLITE_CONFIG_GETPCACHE:
        return real_function(operation);
    case SQLITE_CONFIG_MALLOC:
    case SQLITE_CONFIG_GETMALLOC:
        return real_function(operation, va_arg(args, sqlite3_mem_methods *));
    case SQLITE_CONFIG_MUTEX:
    case SQLITE_CONFIG_GETMUTEX:
        return real_function(operation, va_arg(args, sqlite3_mutex_methods *));
    case SQLITE_CONFIG_PAGECACHE:
    case SQLITE_CONFIG_HEAP:
    {
        void *memory = va_arg(args, void *);
        int size = va_arg(args, int);
        int count = va_arg(args, int);
        return real_function(operation, memory, size, count);
    }
    case SQLITE_CONFIG_MEMSTATUS:
    case SQLITE_CONFIG_SMALL_MALLOC:
    case SQLITE_CONFIG_URI:
    case SQLITE_CONFIG_COVERING_INDEX_SCAN:
    case SQLITE_CONFIG_WIN32_HEAPSIZE:
    case SQLITE_CONFIG_STMTJRNL_SPILL:
    case SQLITE_CONFIG_SORTERREF_SIZE:
        return real_function(operation, va_arg(args, int));
    case SQLITE_CONFIG_PCACHE_HDRSZ:
    case SQLITE_CONFIG_ROWID_IN_VIEW:
        return real_function(operation, va_arg(args, int *));
    case SQLITE_CONFIG_LOOKASIDE:
    {
        int size = va_arg(args, int);
        int count = va_arg(args, int);
        return real_function(operation, size, count);
    }
    case SQLITE_CONFIG_PCACHE2:
    case SQLITE_CONFIG_GETPCACHE2:
        return real_function(operation, va_arg(args, sqlite3_pcache_methods2 *));
    case SQLITE_CONFIG_LOG:
    {
        log_callback_fn callback = va_arg(args, log_callback_fn);
        void *context = va_arg(args, void *);
        return real_function(operation, callback, context);
    }
    case SQLITE_CONFIG_SQLLOG:
    {
        sql_log_callback_fn callback = va_arg(args, sql_log_callback_fn);
        void *context = va_arg(args, void *);
        return real_function(operation, callback, context);
    }
    case SQLITE_CONFIG_MMAP_SIZE:
    {
        sqlite3_int64 default_size = va_arg(args, sqlite3_int64);
        sqlite3_int64 maximum_size = va_arg(args, sqlite3_int64);
        return real_function(operation, default_size, maximum_size);
    }
    case SQLITE_CONFIG_PMASZ:
        return real_function(operation, va_arg(args, unsigned int));
    case SQLITE_CONFIG_MEMDB_MAXSIZE:
        return real_function(operation, va_arg(args, sqlite3_int64));
    default:
        return real_function(operation);
    }
}

int delegate_real_sqlite3_db_config(sqlite3 *database, int operation, va_list args)
{
    static sqlite3_db_config_fn real_function = resolve_variadic<sqlite3_db_config_fn>("sqlite3_db_config");

    if (operation == SQLITE_DBCONFIG_MAINDBNAME)
    {
        return real_function(database, operation, va_arg(args, const char *));
    }
    if (operation == SQLITE_DBCONFIG_LOOKASIDE)
    {
        void *buffer = va_arg(args, void *);
        int size = va_arg(args, int);
        int count = va_arg(args, int);
        return real_function(database, operation, buffer, size, count);
    }
    if (operation == SQLITE_DBCONFIG_FP_DIGITS || is_db_flag_operation(operation))
    {
        int value = va_arg(args, int);
        int *result = va_arg(args, int *);
        return real_function(database, operation, value, result);
    }
    return real_function(database, operation);
}

char *delegate_real_sqlite3_mprintf(const char *format, va_list args)
{
    return delegate_real_sqlite3_vmprintf(format, args);
}

char *delegate_real_sqlite3_snprintf(int size, char *buffer, const char *format, va_list args)
{
    return delegate_real_sqlite3_vsnprintf(size, buffer, format, args);
}

void delegate_real_sqlite3_str_appendf(sqlite3_str *string, const char *format, va_list args)
{
    delegate_real_sqlite3_str_vappendf(string, format, args);
}

void delegate_real_sqlite3_log(int error_code, const char *format, va_list args)
{
    static sqlite3_log_fn real_function = resolve_variadic<sqlite3_log_fn>("sqlite3_log");
    char *message = delegate_real_sqlite3_vmprintf(format, args);

    if (message != nullptr)
    {
        real_function(error_code, "%s", message);
        delegate_real_sqlite3_free(message);
    }
    else
    {
        real_function(error_code, "%s", "");
    }
}

int delegate_real_sqlite3_test_control(int operation, va_list args)
{
    static sqlite3_test_control_fn real_function = resolve_variadic<sqlite3_test_control_fn>("sqlite3_test_control");

    switch (operation)
    {
    case SQLITE_TESTCTRL_PRNG_SAVE:
    case SQLITE_TESTCTRL_PRNG_RESTORE:
    case SQLITE_TESTCTRL_BYTEORDER:
    case SQLITE_TESTCTRL_ISINIT:
        return real_function(operation);
    case SQLITE_TESTCTRL_PRNG_SEED:
    {
        int seed = va_arg(args, int);
        sqlite3 *database = va_arg(args, sqlite3 *);
        return real_function(operation, seed, database);
    }
    case SQLITE_TESTCTRL_FK_NO_ACTION:
    {
        sqlite3 *database = va_arg(args, sqlite3 *);
        int enabled = va_arg(args, int);
        return real_function(operation, database, enabled);
    }
    case SQLITE_TESTCTRL_BITVEC_TEST:
    {
        int size = va_arg(args, int);
        int *program = va_arg(args, int *);
        return real_function(operation, size, program);
    }
    case SQLITE_TESTCTRL_FAULT_INSTALL:
        return real_function(operation, va_arg(args, fault_callback_fn));
    case SQLITE_TESTCTRL_BENIGN_MALLOC_HOOKS:
    {
        void_callback_fn begin = va_arg(args, void_callback_fn);
        void_callback_fn end = va_arg(args, void_callback_fn);
        return real_function(operation, begin, end);
    }
    case SQLITE_TESTCTRL_PENDING_BYTE:
        return real_function(operation, va_arg(args, unsigned int));
    case SQLITE_TESTCTRL_ASSERT:
    case SQLITE_TESTCTRL_ALWAYS:
    case SQLITE_TESTCTRL_NEVER_CORRUPT:
    case SQLITE_TESTCTRL_EXTRA_SCHEMA_CHECKS:
    case SQLITE_TESTCTRL_ONCE_RESET_THRESHOLD:
        return real_function(operation, va_arg(args, int));
    case SQLITE_TESTCTRL_JSON_SELFCHECK:
        return real_function(operation, va_arg(args, int *));
    case SQLITE_TESTCTRL_OPTIMIZATIONS:
    {
        sqlite3 *database = va_arg(args, sqlite3 *);
        unsigned int flags = va_arg(args, unsigned int);
        return real_function(operation, database, flags);
    }
    case SQLITE_TESTCTRL_GETOPT:
    {
        sqlite3 *database = va_arg(args, sqlite3 *);
        int *flags = va_arg(args, int *);
        return real_function(operation, database, flags);
    }
    case SQLITE_TESTCTRL_INTERNAL_FUNCTIONS:
        return real_function(operation, va_arg(args, sqlite3 *));
    case SQLITE_TESTCTRL_LOCALTIME_FAULT:
    {
        int enabled = va_arg(args, int);
        if (enabled == 2)
        {
            localtime_callback_fn callback = va_arg(args, localtime_callback_fn);
            return real_function(operation, enabled, callback);
        }
        return real_function(operation, enabled);
    }
    case SQLITE_TESTCTRL_VDBE_COVERAGE:
    {
        vdbe_coverage_callback_fn callback = va_arg(args, vdbe_coverage_callback_fn);
        void *context = va_arg(args, void *);
        return real_function(operation, callback, context);
    }
    case SQLITE_TESTCTRL_SORTER_MMAP:
    {
        sqlite3 *database = va_arg(args, sqlite3 *);
        int maximum = va_arg(args, int);
        return real_function(operation, database, maximum);
    }
    case SQLITE_TESTCTRL_IMPOSTER:
    {
        sqlite3 *database = va_arg(args, sqlite3 *);
        const char *database_name = va_arg(args, const char *);
        int mode = va_arg(args, int);
        int root_page = va_arg(args, int);
        return real_function(operation, database, database_name, mode, root_page);
    }
    case SQLITE_TESTCTRL_PARSER_COVERAGE:
        return real_function(operation, va_arg(args, FILE *));
    case SQLITE_TESTCTRL_RESULT_INTREAL:
        return real_function(operation, va_arg(args, sqlite3_context *));
    case SQLITE_TESTCTRL_SEEK_COUNT:
    {
        sqlite3 *database = va_arg(args, sqlite3 *);
        sqlite3_uint64 *count = va_arg(args, sqlite3_uint64 *);
        return real_function(operation, database, count);
    }
    case SQLITE_TESTCTRL_TRACEFLAGS:
    {
        int trace_operation = va_arg(args, int);
        unsigned int *flags = va_arg(args, unsigned int *);
        return real_function(operation, trace_operation, flags);
    }
    case SQLITE_TESTCTRL_LOGEST:
    {
        double input = va_arg(args, double);
        int *log_estimate = va_arg(args, int *);
        sqlite3_uint64 *integer = va_arg(args, sqlite3_uint64 *);
        int *log_estimate_2 = va_arg(args, int *);
        return real_function(operation, input, log_estimate, integer, log_estimate_2);
    }
    case SQLITE_TESTCTRL_ATOF:
    {
        const char *input = va_arg(args, const char *);
        double *result = va_arg(args, double *);
        return real_function(operation, input, result);
    }
    case SQLITE_TESTCTRL_TUNE:
    {
        int identifier = va_arg(args, int);
        int *value = va_arg(args, int *);
        return real_function(operation, identifier, value);
    }
    default:
        return real_function(operation);
    }
}

int delegate_real_sqlite3_vtab_config(sqlite3 *database, int operation, va_list args)
{
    static sqlite3_vtab_config_fn real_function = resolve_variadic<sqlite3_vtab_config_fn>("sqlite3_vtab_config");

    if (operation == SQLITE_VTAB_CONSTRAINT_SUPPORT)
    {
        return real_function(database, operation, va_arg(args, int));
    }
    return real_function(database, operation);
}

#if !defined(_WIN32)
    #define MOCK_SQLITE3_VARIADIC_IMPL(return_type, name, ...) MOCK_WEAK_IMPL(return_type, name, __VA_ARGS__)
#else
    #define MOCK_SQLITE3_VARIADIC_IMPL(return_type, name, ...) extern "C" return_type SQLITE_CDECL name(__VA_ARGS__)
#endif

MOCK_SQLITE3_VARIADIC_IMPL(int, sqlite3_config, int operation, ...)
{
    va_list args;
    int return_value;
    va_start(args, operation);
    if (_mock_sqlite3 != nullptr)
    {
        return_value = _mock_sqlite3->sqlite3_config(operation, args);
    }
    else
    {
        return_value = delegate_real_sqlite3_config(operation, args);
    }
    va_end(args);
    return return_value;
}

MOCK_SQLITE3_VARIADIC_IMPL(int, sqlite3_db_config, sqlite3 *database, int operation, ...)
{
    va_list args;
    int return_value;
    va_start(args, operation);
    if (_mock_sqlite3 != nullptr)
    {
        return_value = _mock_sqlite3->sqlite3_db_config(database, operation, args);
    }
    else
    {
        return_value = delegate_real_sqlite3_db_config(database, operation, args);
    }
    va_end(args);
    return return_value;
}

MOCK_SQLITE3_VARIADIC_IMPL(char *, sqlite3_mprintf, const char *format, ...)
{
    va_list args;
    char *return_value;
    va_start(args, format);
    if (_mock_sqlite3 != nullptr)
    {
        return_value = _mock_sqlite3->sqlite3_mprintf(format, args);
    }
    else
    {
        return_value = delegate_real_sqlite3_mprintf(format, args);
    }
    va_end(args);
    return return_value;
}

MOCK_SQLITE3_VARIADIC_IMPL(char *, sqlite3_snprintf, int size, char *buffer, const char *format, ...)
{
    va_list args;
    char *return_value;
    va_start(args, format);
    if (_mock_sqlite3 != nullptr)
    {
        return_value = _mock_sqlite3->sqlite3_snprintf(size, buffer, format, args);
    }
    else
    {
        return_value = delegate_real_sqlite3_snprintf(size, buffer, format, args);
    }
    va_end(args);
    return return_value;
}

MOCK_SQLITE3_VARIADIC_IMPL(void, sqlite3_str_appendf, sqlite3_str *string, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    if (_mock_sqlite3 != nullptr)
    {
        _mock_sqlite3->sqlite3_str_appendf(string, format, args);
    }
    else
    {
        delegate_real_sqlite3_str_appendf(string, format, args);
    }
    va_end(args);
}

MOCK_SQLITE3_VARIADIC_IMPL(void, sqlite3_log, int error_code, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    if (_mock_sqlite3 != nullptr)
    {
        _mock_sqlite3->sqlite3_log(error_code, format, args);
    }
    else
    {
        delegate_real_sqlite3_log(error_code, format, args);
    }
    va_end(args);
}

MOCK_SQLITE3_VARIADIC_IMPL(int, sqlite3_test_control, int operation, ...)
{
    va_list args;
    int return_value;
    va_start(args, operation);
    if (_mock_sqlite3 != nullptr)
    {
        return_value = _mock_sqlite3->sqlite3_test_control(operation, args);
    }
    else
    {
        return_value = delegate_real_sqlite3_test_control(operation, args);
    }
    va_end(args);
    return return_value;
}

MOCK_SQLITE3_VARIADIC_IMPL(int, sqlite3_vtab_config, sqlite3 *database, int operation, ...)
{
    va_list args;
    int return_value;
    va_start(args, operation);
    if (_mock_sqlite3 != nullptr)
    {
        return_value = _mock_sqlite3->sqlite3_vtab_config(database, operation, args);
    }
    else
    {
        return_value = delegate_real_sqlite3_vtab_config(database, operation, args);
    }
    va_end(args);
    return return_value;
}

#undef MOCK_SQLITE3_VARIADIC_IMPL
