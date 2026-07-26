#include <testfw.h>

#include <sqlite3.h>

class sqliteTest : public Test
{
};

TEST_F(sqliteTest, exec_create_insert_select_roundtrip)
{
    // Arrange
    sqlite3 *db = nullptr; // [状態] - インメモリ DB (":memory:") へ接続するためのハンドルを用意する。

    // Pre-Assert

    // Act
    int open_rc = sqlite3_open(":memory:", &db); // [手順] - sqlite3_open(":memory:", &db) でインメモリ DB を開く。

    // Assert
    ASSERT_EQ(SQLITE_OK, open_rc); // [確認_正常系] - sqlite3_open の戻り値が SQLITE_OK であること。
    ASSERT_NE(nullptr, db);        // [確認_正常系] - db が NULL でないこと。

    // Act_2
    int create_rc = sqlite3_exec(db, "CREATE TABLE t (id INTEGER, name TEXT)", nullptr, nullptr,
                                 nullptr); // [手順] - CREATE TABLE 文を sqlite3_exec で実行する。
    int insert_rc = sqlite3_exec(db, "INSERT INTO t (id, name) VALUES (1, 'sqlite')", nullptr, nullptr,
                                 nullptr); // [手順] - INSERT 文を sqlite3_exec で実行する。

    // Assert_2
    ASSERT_EQ(SQLITE_OK,
              create_rc); // [確認_正常系] - CREATE TABLE 実行時の sqlite3_exec の戻り値が SQLITE_OK であること。
    ASSERT_EQ(SQLITE_OK, insert_rc); // [確認_正常系] - INSERT 実行時の sqlite3_exec の戻り値が SQLITE_OK であること。

    // Act_3
    sqlite3_stmt *stmt = nullptr;
    int prepare_rc = sqlite3_prepare_v2(db, "SELECT id, name FROM t", -1, &stmt,
                                        nullptr); // [手順] - SELECT 文を sqlite3_prepare_v2 でコンパイルする。
    int step_rc = sqlite3_step(stmt);             // [手順] - sqlite3_step(stmt) で 1 行目を取得する。

    // Assert_3
    ASSERT_EQ(SQLITE_OK, prepare_rc);          // [確認_正常系] - sqlite3_prepare_v2 の戻り値が SQLITE_OK であること。
    ASSERT_EQ(SQLITE_ROW, step_rc);            // [確認_正常系] - sqlite3_step の戻り値が SQLITE_ROW であること。
    EXPECT_EQ(1, sqlite3_column_int(stmt, 0)); // [確認_正常系] - id 列の値が 1 であること。
    EXPECT_STREQ("sqlite", reinterpret_cast<const char *>(
                               sqlite3_column_text(stmt, 1))); // [確認_正常系] - name 列の値が "sqlite" であること。

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

TEST_F(sqliteTest, exec_invalid_sql_returns_error)
{
    // Arrange
    sqlite3 *db = nullptr; // [状態] - インメモリ DB へ接続するためのハンドルを用意する。
    sqlite3_open(":memory:", &db);
    char *errmsg = nullptr;

    // Pre-Assert
    ASSERT_NE(nullptr, db); // [Pre-Assert確認_正常系] - db が NULL でないこと (以降の Act 前提)。

    // Act
    int rc = sqlite3_exec(db, "SELEKT * FROM nothing", nullptr, nullptr,
                          &errmsg); // [手順] - 構文が誤った SQL 文を sqlite3_exec に渡す。

    // Assert
    EXPECT_NE(SQLITE_OK, rc);   // [確認_異常系] - sqlite3_exec の戻り値が SQLITE_OK でないこと。
    EXPECT_NE(nullptr, errmsg); // [確認_異常系] - errmsg にエラー メッセージが設定されること。

    // Cleanup
    sqlite3_free(errmsg);
    sqlite3_close(db);
}

TEST_F(sqliteTest, prepared_statement_bind_int_and_text)
{
    // Arrange
    sqlite3 *db = nullptr; // [状態] - インメモリ DB へ接続し、バインド対象のテーブルを用意する。
    sqlite3_open(":memory:", &db);
    sqlite3_exec(db, "CREATE TABLE t (id INTEGER, name TEXT)", nullptr, nullptr, nullptr);
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, "INSERT INTO t (id, name) VALUES (?, ?)", -1, &stmt, nullptr);

    // Pre-Assert

    // Act
    sqlite3_bind_int(stmt, 1, 42); // [手順] - 1 番目のプレースホルダーへ整数値 42 をバインドする。
    sqlite3_bind_text(stmt, 2, "bound", -1,
                      SQLITE_STATIC); // [手順] - 2 番目のプレースホルダーへ文字列 "bound" をバインドする。
    int step_rc = sqlite3_step(stmt); // [手順] - バインド済みの sqlite3_stmt を sqlite3_step で実行する。

    // Assert
    EXPECT_EQ(SQLITE_DONE, step_rc); // [確認_正常系] - INSERT 実行時の sqlite3_step の戻り値が SQLITE_DONE であること。

    // Act_2
    sqlite3_stmt *select_stmt = nullptr;
    sqlite3_prepare_v2(db, "SELECT id, name FROM t", -1, &select_stmt, nullptr);
    int select_step_rc = sqlite3_step(select_stmt); // [手順] - SELECT 文の sqlite3_stmt を sqlite3_step で実行する。

    // Assert_2
    ASSERT_EQ(SQLITE_ROW,
              select_step_rc); // [確認_正常系] - SELECT 実行時の sqlite3_step の戻り値が SQLITE_ROW であること。
    EXPECT_EQ(42, sqlite3_column_int(select_stmt, 0)); // [確認_正常系] - id 列の値が、バインドした 42 であること。
    EXPECT_STREQ("bound", reinterpret_cast<const char *>(sqlite3_column_text(
                              select_stmt, 1))); // [確認_正常系] - name 列の値が、バインドした "bound" であること。

    // Cleanup
    sqlite3_finalize(select_stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
