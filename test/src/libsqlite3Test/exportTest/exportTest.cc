#include <testfw.h>

#include <sqlite3.h>

#include <set>
#include <string>
#include <type_traits>

#define MOCK_SQLITE3_RET(return_type, name, parameters, arguments, matchers) \
    using expected_##name##_fn = return_type(SQLITE_APICALL *) parameters; \
    static_assert(std::is_same<decltype(&name), expected_##name##_fn>::value, #name " のシグネチャが不一致です");
#define MOCK_SQLITE3_VOID(return_type, name, parameters, arguments, matchers) \
    using expected_##name##_fn = return_type(SQLITE_APICALL *) parameters; \
    static_assert(std::is_same<decltype(&name), expected_##name##_fn>::value, #name " のシグネチャが不一致です");
#include <mock_sqlite3_api_table.h>
#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET

using expected_sqlite3_config_fn = int(SQLITE_CDECL *)(int, ...);
using expected_sqlite3_db_config_fn = int(SQLITE_CDECL *)(sqlite3 *, int, ...);
using expected_sqlite3_mprintf_fn = char *(SQLITE_CDECL *)(const char *, ...);
using expected_sqlite3_snprintf_fn = char *(SQLITE_CDECL *)(int, char *, const char *, ...);
using expected_sqlite3_str_appendf_fn = void(SQLITE_CDECL *)(sqlite3_str *, const char *, ...);
using expected_sqlite3_log_fn = void(SQLITE_CDECL *)(int, const char *, ...);
using expected_sqlite3_test_control_fn = int(SQLITE_CDECL *)(int, ...);
using expected_sqlite3_vtab_config_fn = int(SQLITE_CDECL *)(sqlite3 *, int, ...);
static_assert(std::is_same<decltype(&sqlite3_config), expected_sqlite3_config_fn>::value,
              "sqlite3_config のシグネチャが不一致です");
static_assert(std::is_same<decltype(&sqlite3_db_config), expected_sqlite3_db_config_fn>::value,
              "sqlite3_db_config のシグネチャが不一致です");
static_assert(std::is_same<decltype(&sqlite3_mprintf), expected_sqlite3_mprintf_fn>::value,
              "sqlite3_mprintf のシグネチャが不一致です");
static_assert(std::is_same<decltype(&sqlite3_snprintf), expected_sqlite3_snprintf_fn>::value,
              "sqlite3_snprintf のシグネチャが不一致です");
static_assert(std::is_same<decltype(&sqlite3_str_appendf), expected_sqlite3_str_appendf_fn>::value,
              "sqlite3_str_appendf のシグネチャが不一致です");
static_assert(std::is_same<decltype(&sqlite3_log), expected_sqlite3_log_fn>::value,
              "sqlite3_log のシグネチャが不一致です");
static_assert(std::is_same<decltype(&sqlite3_test_control), expected_sqlite3_test_control_fn>::value,
              "sqlite3_test_control のシグネチャが不一致です");
static_assert(std::is_same<decltype(&sqlite3_vtab_config), expected_sqlite3_vtab_config_fn>::value,
              "sqlite3_vtab_config のシグネチャが不一致です");

static const char *const kExpectedExportNames[] = {
#define MOCK_SQLITE3_RET(return_type, name, parameters, arguments, matchers)  #name,
#define MOCK_SQLITE3_VOID(return_type, name, parameters, arguments, matchers) #name,
#include <mock_sqlite3_api_table.h>
#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET
    "sqlite3_config",      "sqlite3_db_config", "sqlite3_mprintf",      "sqlite3_snprintf",
    "sqlite3_str_appendf", "sqlite3_log",       "sqlite3_test_control", "sqlite3_vtab_config",
};

// libsqlite3 の公開関数と mock_sqlite3 の API 表が一致することの確認
TEST(exportTest, sqlite3_symbols_match_api_table)
{
    // Arrange
    std::set<std::string> expected(
        std::begin(kExpectedExportNames),
        std::end(kExpectedExportNames)); // [状態] - mock_sqlite3 の API 表から期待する公開関数名を構築する。
    std::string path = findWorkspaceRoot() + "/app/sqlite/prod/lib/libsqlite3" +
                       TESTFW_SHARED_LIBRARY_EXTENSION; // [状態] - 検査対象を libsqlite3 の動的ライブラリとする。

    // Pre-Assert

    // Act
    std::set<std::string> all_actual =
        testing::getActualExportNames(path); // [手順] - libsqlite3 のエクスポート名を取得する。
    std::set<std::string> actual;
    for (const std::string &name : all_actual)
    {
        if (name.rfind("sqlite3_", 0u) == 0u && name != "sqlite3_version" && name != "sqlite3_temp_directory" &&
            name != "sqlite3_data_directory")
        {
            actual.insert(name);
        }
    }

    // Assert
    EXPECT_EQ(expected, actual); // [確認_正常系] - libsqlite3 の全公開関数名が mock_sqlite3 の API 表と一致すること。
#if defined(_WIN32)
    EXPECT_EQ(277u, actual.size()); // [確認_正常系] - Windows の libsqlite3 の公開関数数が 277 であること。
#else
    EXPECT_EQ(274u, actual.size()); // [確認_正常系] - Linux の libsqlite3 の公開関数数が 274 であること。
#endif
}
