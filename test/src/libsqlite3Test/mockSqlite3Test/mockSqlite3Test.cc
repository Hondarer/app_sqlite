#include <testfw.h>

#include <mock_sqlite3.h>

// Mock_sqlite3 を生成しない場合に SQLite の実関数へ委譲されることの確認
TEST(mockSqlite3Test, delegates_to_real_without_mock)
{
    // Arrange
    sqlite3 *database = nullptr;

    // Pre-Assert

    // Act
    int return_value =
        sqlite3_open(":memory:", &database); // [手順] - Mock_sqlite3 を生成せず sqlite3_open を呼び出す。

    // Assert
    EXPECT_EQ(SQLITE_OK, return_value); // [確認_正常系] - sqlite3_open の戻り値が SQLITE_OK であること。
    ASSERT_NE(nullptr, database);       // [確認_正常系] - sqlite3_open が database を設定すること。

    // Cleanup
    sqlite3_close(database);
}

// 注入済み Mock_sqlite3 の未設定呼び出しが SQLite の実関数へ委譲されることの確認
TEST(mockSqlite3Test, delegates_to_real_with_default_action)
{
    // Arrange
    NiceMock<Mock_sqlite3> mock_sqlite3;
    sqlite3 *database = nullptr;

    // Pre-Assert

    // Act
    int return_value =
        sqlite3_open(":memory:", &database); // [手順] - 既定動作の Mock_sqlite3 を介して sqlite3_open を呼び出す。

    // Assert
    EXPECT_EQ(SQLITE_OK, return_value); // [確認_正常系] - sqlite3_open の戻り値が SQLITE_OK であること。
    ASSERT_NE(nullptr, database);       // [確認_正常系] - sqlite3_open が database を設定すること。

    // Cleanup
    sqlite3_close(database);
}

// EXPECT_CALL により SQLite API の戻り値を変更できることの確認
TEST(mockSqlite3Test, overrides_result)
{
    // Arrange
    NiceMock<Mock_sqlite3> mock_sqlite3;
    sqlite3 *database = nullptr;

    // Pre-Assert
    EXPECT_CALL(mock_sqlite3, sqlite3_open(StrEq(":memory:"), &database))
        .WillOnce(Return(
            SQLITE_CANTOPEN)); // [Pre-Assert確認_異常系] - sqlite3_open がインメモリ DB を指定して 1 回呼び出されること。
                               // [Pre-Assert手順] - sqlite3_open から SQLITE_CANTOPEN を返却する。

    // Act
    int return_value = sqlite3_open(
        ":memory:", &database); // [手順] - 戻り値を設定した Mock_sqlite3 を介して sqlite3_open を呼び出す。

    // Assert
    EXPECT_EQ(SQLITE_CANTOPEN, return_value); // [確認_異常系] - sqlite3_open の戻り値が SQLITE_CANTOPEN であること。
}

// 可変長引数を受け取る sqlite3_mprintf の既定動作が実関数へ委譲されることの確認
TEST(mockSqlite3Test, delegates_variadic_format_to_real)
{
    // Arrange
    NiceMock<Mock_sqlite3> mock_sqlite3;

    // Pre-Assert

    // Act
    char *actual = sqlite3_mprintf("%s:%d", "sqlite", 3); // [手順] - 可変長引数を指定して sqlite3_mprintf を呼び出す。

    // Assert
    ASSERT_NE(nullptr, actual);       // [確認_正常系] - sqlite3_mprintf の戻り値が NULL でないこと。
    EXPECT_STREQ("sqlite:3", actual); // [確認_正常系] - sqlite3_mprintf の戻り値が "sqlite:3" であること。

    // Cleanup
    sqlite3_free(actual);
}

// 操作コード型可変長 API の戻り値を変更できることの確認
TEST(mockSqlite3Test, overrides_variadic_config_result)
{
    // Arrange
    NiceMock<Mock_sqlite3> mock_sqlite3;

    // Pre-Assert
    EXPECT_CALL(mock_sqlite3, sqlite3_config(SQLITE_CONFIG_MEMSTATUS, _))
        .WillOnce(Return(
            SQLITE_BUSY)); // [Pre-Assert確認_異常系] - sqlite3_config が SQLITE_CONFIG_MEMSTATUS を指定して 1 回呼び出されること。
                           // [Pre-Assert手順] - sqlite3_config から SQLITE_BUSY を返却する。

    // Act
    int return_value = sqlite3_config(SQLITE_CONFIG_MEMSTATUS,
                                      0); // [手順] - 戻り値を設定した Mock_sqlite3 を介して sqlite3_config を呼び出す。

    // Assert
    EXPECT_EQ(SQLITE_BUSY, return_value); // [確認_異常系] - sqlite3_config の戻り値が SQLITE_BUSY であること。
}
