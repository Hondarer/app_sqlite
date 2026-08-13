# sqlite

このリポジトリ (ディレクトリ) は [c-modernization-kit](https://github.com/Hondarer/c-modernization-kit) の `app/sqlite` として管理される、個別アプリです。  
c-modernization-kit のワークスペース内 (`framework/makefw` 等と組み合わせた make ビルド環境) で利用することを前提としたアプリであり、本ディレクトリ単体ではビルドできません。

## 概要

[SQLite](https://www.sqlite.org/) の amalgamation (単一ファイルにまとめられたソース配布形式。`sqlite3.c` / `sqlite3.h` / `sqlite3ext.h`) および公式 CLI シェル (`shell.c`) を、c-modernization-kit の makefw 規約に沿って取り込んだラッパー ライブラリです。  
SQLite 自体のソースは改変せず、リリース アーカイブをそのまま展開して利用します。

ビルド成果物は動的ライブラリと、`shell.c` から生成する `sqlite3` コマンド (公式 CLI) です。

- Linux: `libsqlite3.so`
- Windows: `libsqlite3.dll` およびリンク用の import library `libsqlite3.lib`

静的ライブラリ (`libsqlite3.a` および静的リンク用の `libsqlite3.lib`) は生成しません。

`sqlite3` コマンドは行編集オプション (`-DHAVE_READLINE` 等) を付与しない最小構成でビルドしており、`readline`/`editline` 等の外部ライブラリには依存しません。

## パッケージの配置手順 (初回セットアップ)

本リポジトリは SQLite の amalgamation アーカイブ (zip) を Git 管理下に手動配置する運用です。  
`app/sqlite/packages/` に、以下の手順で取得した zip を 1 つだけ配置してください。

1. [https://www.sqlite.org/download.html](https://www.sqlite.org/download.html) を開き、  
   「Source Code」節にある amalgamation zip (`sqlite-amalgamation-<version>.zip`) のリンク先 URL を確認する (年別ディレクトリを含む URL のため、バージョンごとに変わります)。
2. 確認した URL から取得する (curl での取得例):

   ```sh
   curl -L -o app/sqlite/packages/sqlite-amalgamation-3460100.zip \
     https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip
   ```

配置後、`make` (または `make test`) を実行すると、`app/sqlite/bin/extract_package.py` が自動的に `prod/include/`、`prod/libsrc/sqlite3/`、`prod/src/cmd/sqlite3/` へ展開します。  
展開先はいずれも生成物であり `.gitignore` 対象です。

`packages/` にアーカイブが存在しない状態で `make` を実行すると、ビルドはエラーで停止し、配置方法の案内が表示されます。

`packages/` に複数のアーカイブが存在する場合はエラーにはせず、ファイル名のバージョン番号が最も新しいものを自動的に採用します (バージョン番号が読み取れないファイルが混在する場合は、更新日時が最も新しいものを採用します)。  
この場合、採用されなかったアーカイブを削除するよう警告が表示されるので、単一ファイル運用に戻してください。

## バージョン更新手順

1. 新しいバージョンの zip を取得し、`app/sqlite/packages/` に追加する。
2. 古いバージョンの zip を削除する (`packages/` には常に 1 個のみを置く運用)。
3. `make` を実行すると、新しい zip のタイムスタンプが展開済み生成物より新しいと判定され、自動的に再展開されます。

## ライセンス

SQLite 本体は Public Domain です。単体の `LICENSE` ファイルはアーカイブ内に同梱されておらず、 `sqlite3.h` 冒頭のコメントに著作権放棄の宣言 (blessing) が記載されています。詳細は [https://www.sqlite.org/copyright.html](https://www.sqlite.org/copyright.html) を参照してください。

`app/sqlite` 直下の `LICENSE` (MIT License) は、本ディレクトリのラッパー コード (`bin/extract_package.py`、`makefile`/`makepart.mk` 等の手書きファイル) に対する著作権表示であり、`sqlite3.c`/`sqlite3.h`/`shell.c` 等の SQLite 本体には適用されません。

## サンプルとテスト

- `prod/src/cmd/sqlite3/` : `shell.c` (生成物) から構築する公式 CLI シェル
- `test/src/cmd/sqliteTest/` : 動作テスト
- `test/src/libsqlite3Test/mockSqlite3Test/` : SQLite API モックの動作テスト
- `test/src/libsqlite3Test/exportTest/` : 公開関数のモック対象漏れを検出するテスト

## SQLite API モック

SQLite を利用するアプリの単体テストでは、`<mock_sqlite3.h>` と `libmock_sqlite3` を使用できます。  
テスト対象の `makepart.mk` では、`sqlite3` の代わりに `mock_sqlite3` をリンクします。  
`sqlite3` と `mock_sqlite3` を同時にリンクしないでください。Linux では実ライブラリの強シンボルが弱定義のモックを上書きし、`EXPECT_CALL` が効かなくなります。

```makefile
ifdef PLATFORM_WINDOWS
    DEFINES += SQLITE_API=
endif

LIBS += mock_sqlite3
```

Windows では、`SQLITE_API=` により SQLite の DLL import 宣言を無効にし、`mock_sqlite3` が提供する実シンボルを参照します。  
`Mock_sqlite3` を生成しない場合と、生成後に `EXPECT_CALL` や `ON_CALL` を設定しない場合は、動的ライブラリの実関数を呼び出します。  
実関数への委譲では、Linux の `LD_LIBRARY_PATH` または Windows の `PATH` から `libsqlite3` を読み込みます。

振る舞いを変更するテストでは、`Mock_sqlite3` を生成して `EXPECT_CALL` または `ON_CALL` を指定します。

```cpp
NiceMock<Mock_sqlite3> mock_sqlite3;
EXPECT_CALL(mock_sqlite3, sqlite3_open(StrEq(":memory:"), _)).WillOnce(Return(SQLITE_CANTOPEN));
```

実のデータベースを開かずに単体隔離する場合は、SUT が呼び出す関数をすべてスタブしてください。  
`sqlite3_open` を成功させるときは、`SetArgPointee` で `sqlite3 **` に偽ハンドルを設定します。  
スタブしていない呼び出しは実関数へ委譲されるため、偽ハンドルを渡すと実関数が失敗します。

```cpp
sqlite3 *fake_database = reinterpret_cast<sqlite3 *>(1);
EXPECT_CALL(mock_sqlite3, sqlite3_open(StrEq(":memory:"), _))
    .WillOnce(DoAll(SetArgPointee<1>(fake_database), Return(SQLITE_OK)));
```

`sqlite3_config`、`sqlite3_db_config`、`sqlite3_mprintf` などの可変長引数 API は、可変長部分を `va_list` としてモックへ渡します。  
引数の内容を照合しない場合は、該当する `va_list` 引数に `_` を指定してください。

公開変数 `sqlite3_version`、`sqlite3_temp_directory`、`sqlite3_data_directory` は関数モックの対象外です。  
`mock_sqlite3` はこれらの変数を定義するので、実ライブラリなしでもリンクできます。  
`sqlite3_version` の初期値は `SQLITE_VERSION` です。`sqlite3_temp_directory` と `sqlite3_data_directory` の初期値は `NULL` です。

`exportTest` は公開ヘッダー `sqlite3.h` に載る関数だけを検査します。  
Windows の DLL には `sqlite3.h` に無い `sqlite3_win32_*` が追加で出ますが、これらは検査対象外です。

`sqlite3ext.h` の拡張 API 表はモック対象外です。
