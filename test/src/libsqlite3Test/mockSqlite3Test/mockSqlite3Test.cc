#include <testfw.h>

#include <mock_sqlite3.h>

extern "C"
{
const char *namespace_test_caller_file(void);
int namespace_test_caller_line(void);
int namespace_test_direct_call(void);
int namespace_test_pointer_call(void);
}
const char *namespace_test_cpp_caller_file();
int namespace_test_cpp_caller_line();
int namespace_test_cpp_direct_call();

// Mock_sqlite3 を生成しない場合に SQLite の処理へ委譲されることの確認
TEST(mockSqlite3Test, delegates_to_real_without_mock)
{
    // Arrange
    sqlite3 *database = nullptr; // [状態] - データベース ハンドル変数を初期化する。

    // Pre-Assert

    // Act
    int return_value = sqlite3_app_sqlite3_open(
        ":memory:", &database); // [手順] - Mock_sqlite3 を生成せず ":memory:" を指定して sqlite3_app_sqlite3_open() を呼び出す。

    // Assert
    EXPECT_EQ(SQLITE_OK, return_value); // [確認_正常系] - sqlite3_app_sqlite3_open() の戻り値が SQLITE_OK であること。
    ASSERT_NE(nullptr, database);       // [確認_正常系] - database が nullptr 以外であること。

    // Cleanup
    sqlite3_app_sqlite3_close(database);
}

// 注入済み Mock_sqlite3 の未設定呼び出しが SQLite の処理へ委譲されることの確認
TEST(mockSqlite3Test, delegates_to_real_with_default_action)
{
    // Arrange
    NiceMock<Mock_sqlite3> mock_sqlite3; // [状態] - 既定動作の Mock_sqlite3 を生成する。
    sqlite3 *database = nullptr;         // [状態] - データベース ハンドル変数を初期化する。

    // Pre-Assert

    // Act
    int return_value = sqlite3_app_sqlite3_open(
        ":memory:",
        &database); // [手順] - 既定動作の Mock_sqlite3 を介して ":memory:" を指定して sqlite3_app_sqlite3_open() を呼び出す。

    // Assert
    EXPECT_EQ(SQLITE_OK, return_value); // [確認_正常系] - sqlite3_app_sqlite3_open() の戻り値が SQLITE_OK であること。
    ASSERT_NE(nullptr, database);       // [確認_正常系] - database が nullptr 以外であること。

    // Cleanup
    sqlite3_app_sqlite3_close(database);
}

// EXPECT_CALL により SQLite API の戻り値を変更できることの確認
TEST(mockSqlite3Test, overrides_result)
{
    // Arrange
    NiceMock<Mock_sqlite3> mock_sqlite3; // [状態] - 既定動作の Mock_sqlite3 を生成する。
    sqlite3 *database = nullptr;         // [状態] - データベース ハンドル変数を初期化する。

    // Pre-Assert
    EXPECT_CALL(mock_sqlite3, sqlite3_app_sqlite3_open(_, _, _, StrEq(":memory:"), &database))
        .WillOnce(Return(
            SQLITE_CANTOPEN)); // [Pre-Assert確認_異常系] - sqlite3_app_sqlite3_open() が ":memory:" と等しい文字列、&database を指定して 1 回呼び出されること。
    // [Pre-Assert手順] - sqlite3_app_sqlite3_open() から SQLITE_CANTOPEN を返す。

    // Act
    int return_value = sqlite3_app_sqlite3_open(
        ":memory:",
        &database); // [手順] - 戻り値を設定した Mock_sqlite3 を介して ":memory:" を指定して sqlite3_app_sqlite3_open() を呼び出す。

    // Assert
    EXPECT_EQ(SQLITE_CANTOPEN, return_value); // [確認_異常系] - sqlite3_app_sqlite3_open() の戻り値が SQLITE_CANTOPEN であること。
}

// 可変長引数を受け取る sqlite3_app_sqlite3_mprintf の既定動作が SQLite の処理へ委譲されることの確認
TEST(mockSqlite3Test, delegates_variadic_format_to_real)
{
    // Arrange
    NiceMock<Mock_sqlite3> mock_sqlite3; // [状態] - 既定動作の Mock_sqlite3 を生成する。

    // Pre-Assert

    // Act
    char *actual =
        sqlite3_app_sqlite3_mprintf("%s:%d", "sqlite", 3); // [手順] - 可変長引数を指定して sqlite3_app_sqlite3_mprintf() を呼び出す。

    // Assert
    ASSERT_NE(nullptr, actual);       // [確認_正常系] - sqlite3_app_sqlite3_mprintf() の戻り値が nullptr 以外であること。
    EXPECT_STREQ("sqlite:3", actual); // [確認_正常系] - sqlite3_app_sqlite3_mprintf() の戻り値が "sqlite:3" であること。

    // Cleanup
    sqlite3_app_sqlite3_free(actual);
}

// 操作コード型可変長 API の戻り値を変更できることの確認
TEST(mockSqlite3Test, overrides_variadic_config_result)
{
    // Arrange
    NiceMock<Mock_sqlite3> mock_sqlite3; // [状態] - 既定動作の Mock_sqlite3 を生成する。

    // Pre-Assert
    EXPECT_CALL(mock_sqlite3, sqlite3_app_sqlite3_config(_, _, _, SQLITE_CONFIG_MEMSTATUS, _))
        .WillOnce(Return(
            SQLITE_BUSY)); // [Pre-Assert確認_異常系] - sqlite3_app_sqlite3_config() が SQLITE_CONFIG_MEMSTATUS を指定して 1 回呼び出されること。
                           // [Pre-Assert手順] - sqlite3_app_sqlite3_config() から SQLITE_BUSY を返す。

    // Act
    int return_value = sqlite3_app_sqlite3_config(
        SQLITE_CONFIG_MEMSTATUS,
        0); // [手順] - 戻り値を設定した Mock_sqlite3 を介して SQLITE_CONFIG_MEMSTATUS を指定して sqlite3_app_sqlite3_config() を呼び出す。

    // Assert
    EXPECT_EQ(SQLITE_BUSY, return_value); // [確認_異常系] - sqlite3_app_sqlite3_config() の戻り値が SQLITE_BUSY であること。
}

// 実データベースを開かずに open / exec / close を単体隔離できることの確認
TEST(mockSqlite3Test, isolates_open_exec_close_without_real_database)
{
    // Arrange
    NiceMock<Mock_sqlite3> mock_sqlite3; // [状態] - 既定動作の Mock_sqlite3 を生成する。
    sqlite3 *fake_database =
        reinterpret_cast<sqlite3 *>(1); // [状態] - モックの出力とするダミーの sqlite3 ポインターを用意する。
    sqlite3 *database = nullptr;        // [状態] - データベース ハンドル変数を初期化する。

    // Pre-Assert
    EXPECT_CALL(mock_sqlite3, sqlite3_app_sqlite3_open(_, _, _, StrEq(":memory:"), _))
        .WillOnce(DoAll(SetArgPointee<4>(fake_database), Return(SQLITE_OK)));
    // [Pre-Assert確認_正常系] - sqlite3_app_sqlite3_open() が ":memory:" と等しい文字列を指定して 1 回呼び出されること。
    // [Pre-Assert手順] - sqlite3_app_sqlite3_open() から、出力引数に fake_database を設定して SQLITE_OK を返す。
    EXPECT_CALL(mock_sqlite3, sqlite3_app_sqlite3_exec(_, _, _, fake_database, StrEq("SELECT 1;"), nullptr, nullptr, nullptr))
        .WillOnce(Return(
            SQLITE_OK)); // [Pre-Assert確認_正常系] - sqlite3_app_sqlite3_exec() が fake_database、"SELECT 1;" と等しい文字列、nullptr、nullptr、nullptr を指定して 1 回呼び出されること。
                         // [Pre-Assert手順] - sqlite3_app_sqlite3_exec() から SQLITE_OK を返す。
    EXPECT_CALL(mock_sqlite3, sqlite3_app_sqlite3_close(_, _, _, fake_database))
        .WillOnce(Return(
            SQLITE_OK)); // [Pre-Assert確認_正常系] - sqlite3_app_sqlite3_close() が fake_database を指定して 1 回呼び出されること。
                         // [Pre-Assert手順] - sqlite3_app_sqlite3_close() から SQLITE_OK を返す。

    // Act
    int open_result = sqlite3_app_sqlite3_open(":memory:", &database); // [手順] - ":memory:" を指定して sqlite3_app_sqlite3_open() を呼び出す。
    int exec_result = sqlite3_app_sqlite3_exec(database, "SELECT 1;", nullptr, nullptr,
                                   nullptr);    // [手順] - "SELECT 1;" を指定して sqlite3_app_sqlite3_exec() を呼び出す。
    int close_result = sqlite3_app_sqlite3_close(database); // [手順] - database を指定して sqlite3_app_sqlite3_close() を呼び出す。

    // Assert
    EXPECT_EQ(SQLITE_OK, open_result);  // [確認_正常系] - sqlite3_app_sqlite3_open() の戻り値が SQLITE_OK であること。
    EXPECT_EQ(fake_database, database); // [確認_正常系] - database が fake_database と一致すること。
    EXPECT_EQ(SQLITE_OK, exec_result);  // [確認_正常系] - sqlite3_app_sqlite3_exec() の戻り値が SQLITE_OK であること。
    EXPECT_EQ(SQLITE_OK, close_result); // [確認_正常系] - sqlite3_app_sqlite3_close() の戻り値が SQLITE_OK であること。
}

TEST(mockSqlite3Test, injects_c_callsite_and_marks_function_pointer_as_unknown)
{
    // Arrange
    NiceMock<Mock_sqlite3> mock_sqlite3;

    // Pre-Assert
    EXPECT_CALL(mock_sqlite3, sqlite3_app_sqlite3_initialize(StrEq(namespace_test_caller_file()),
                                                            namespace_test_caller_line(),
                                                            StrEq("namespace_test_direct_call")))
        .WillOnce(Return(SQLITE_OK)); // [Pre-Assert確認_正常系] - C 呼び出し元の情報を伴って sqlite3_app_sqlite3_initialize を呼び出す。
    EXPECT_CALL(mock_sqlite3, sqlite3_app_sqlite3_initialize(StrEq(namespace_test_cpp_caller_file()),
                                                            namespace_test_cpp_caller_line(),
                                                            StrEq("namespace_test_cpp_direct_call")))
        .WillOnce(Return(SQLITE_OK)); // [Pre-Assert確認_正常系] - C++ 呼び出し元の情報を伴って sqlite3_app_sqlite3_initialize を呼び出す。
    EXPECT_CALL(mock_sqlite3, sqlite3_app_sqlite3_initialize(StrEq("<unknown>"), 0, StrEq("<unknown>")))
        .WillOnce(Return(SQLITE_ERROR)); // [Pre-Assert確認_正常系] - 関数ポインター経由の取得不能値を受け取る。

    // Act
    const int direct_result = namespace_test_direct_call();
    const int cpp_direct_result = namespace_test_cpp_direct_call();
    const int pointer_result = namespace_test_pointer_call();

    // Assert
    EXPECT_EQ(SQLITE_OK, direct_result); // [確認_正常系] - C 直接呼び出しの mock 戻り値を受け取る。
    EXPECT_EQ(SQLITE_OK, cpp_direct_result); // [確認_正常系] - C++ 直接呼び出しの mock 戻り値を受け取る。
    EXPECT_EQ(SQLITE_ERROR, pointer_result); // [確認_正常系] - 関数ポインター呼び出しの mock 戻り値を受け取る。
}

// mock_sqlite3 だけで公開変数を参照できることの確認
TEST(mockSqlite3Test, provides_public_variables_without_real_library)
{
    // Arrange

    // Pre-Assert

    // Act
    const char *version = sqlite3_app_sqlite3_version; // [手順] - sqlite3_app_sqlite3_version を参照する。

    // Assert
    EXPECT_STREQ(SQLITE_VERSION, version);      // [確認_正常系] - sqlite3_app_sqlite3_version が SQLITE_VERSION と一致すること。
    EXPECT_EQ(nullptr, sqlite3_app_sqlite3_temp_directory); // [確認_正常系] - sqlite3_app_sqlite3_temp_directory が nullptr であること。
    EXPECT_EQ(nullptr, sqlite3_app_sqlite3_data_directory); // [確認_正常系] - sqlite3_app_sqlite3_data_directory が nullptr であること。
}

extern "C" int namespace_test_variadic_line(void);
extern "C" int namespace_test_variadic_call(void);

TEST(mockSqlite3Test, preserves_variadic_caller_information)
{
    // Arrange
    ::testing::NiceMock<Mock_sqlite3> mock;

    // Pre-Assert
    EXPECT_CALL(mock, sqlite3_app_sqlite3_config(::testing::StrEq(namespace_test_caller_file()),
        namespace_test_variadic_line(), ::testing::StrEq("namespace_test_variadic_call"), SQLITE_CONFIG_SERIALIZED, ::testing::_))
        .WillOnce(::testing::Return(17)); // [Pre-Assert確認_正常系] - 可変長引数の入口が呼び出し元を維持すること。

    // Act / Assert
    EXPECT_EQ(17, namespace_test_variadic_call()); // [確認_正常系] - 設定した戻り値を返すこと。
}
