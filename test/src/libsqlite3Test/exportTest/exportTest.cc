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

#define SQLITE3_EXPORT_VARIABLE_TABLE(EXPORT_ENTRY) \
    EXPORT_ENTRY(sqlite3_version, const char (*)[]) \
    EXPORT_ENTRY(sqlite3_temp_directory, char **) \
    EXPORT_ENTRY(sqlite3_data_directory, char **)

SQLITE3_EXPORT_VARIABLE_TABLE(TESTFW_EXPORT_STATIC_ASSERT_ENTRY)

static const char *const kExpectedExportNames[] = {
#define MOCK_SQLITE3_RET(return_type, name, parameters, arguments, matchers)  #name,
#define MOCK_SQLITE3_VOID(return_type, name, parameters, arguments, matchers) #name,
#include <mock_sqlite3_api_table.h>
#undef MOCK_SQLITE3_VOID
#undef MOCK_SQLITE3_RET
    "sqlite3_config",       "sqlite3_db_config",   "sqlite3_mprintf",
    "sqlite3_snprintf",     "sqlite3_str_appendf", "sqlite3_log",
    "sqlite3_test_control", "sqlite3_vtab_config", SQLITE3_EXPORT_VARIABLE_TABLE(TESTFW_EXPORT_NAME_ENTRY)};

#if defined(PLATFORM_WINDOWS)
// sqlite3.c の os_win.c 由来。SQLITE_API のため DLL に出るが、sqlite3.h には無い。
// SQLITE_WIN32_MALLOC 時の sqlite3_win32_compact_heap / sqlite3_win32_reset_heap も同様で、
// 現行ビルドでは出ない。将来出た場合は同じ理由でこの一覧へ追加する。
static const char *const kUnofficialWin32ExportNames[] = {
    "sqlite3_win32_is_nt",           "sqlite3_win32_mbcs_to_utf8",    "sqlite3_win32_mbcs_to_utf8_v2",
    "sqlite3_win32_sleep",           "sqlite3_win32_unicode_to_utf8", "sqlite3_win32_utf8_to_mbcs",
    "sqlite3_win32_utf8_to_mbcs_v2", "sqlite3_win32_utf8_to_unicode", "sqlite3_win32_write_debug",
};
#endif /* PLATFORM_WINDOWS */

// libsqlite3 の期待シンボルと実ライブラリの全エクスポートが一致することの確認
TEST(exportTest, sqlite3_symbols_match_api_table)
{
    // Arrange
    std::set<std::string> expected(
        std::begin(kExpectedExportNames),
        std::end(kExpectedExportNames)); // [状態] - mock_sqlite3 の API 表から期待する公開関数名を構築する。
#if defined(PLATFORM_WINDOWS)
    expected.insert(
        std::begin(kUnofficialWin32ExportNames),
        std::end(kUnofficialWin32ExportNames)); // [状態] - sqlite3.h に無い Windows 固有シンボルを期待値へ追加する。
    expected.insert(testing::identManifestSymbolName(
        "libsqlite3" TESTFW_SHARED_LIBRARY_EXTENSION)); // [状態] - IDENT manifest シンボル名を期待値へ追加する。
#endif                                                  /* PLATFORM_WINDOWS */
    std::string path = findWorkspaceRoot() + "/app/sqlite/prod/lib/libsqlite3" +
                       TESTFW_SHARED_LIBRARY_EXTENSION; // [状態] - 検査対象を libsqlite3 の動的ライブラリとする。

    // Pre-Assert

    // Act
    std::set<std::string> actual =
        testing::getActualExportNames(path); // [手順] - libsqlite3 のエクスポート名を取得する。

    // Assert
    // [サブ手順参照 名前=testing.expectExportNamesMatch]
    testing::expectExportNamesMatch(expected,
                                    actual); // libsqlite3 のエクスポートに不足や想定外がないこと。
}
