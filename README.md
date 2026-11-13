# sqlite

作業前に [作業規則](AGENTS.md) を確認してください。

このリポジトリ (ディレクトリ) は [c-modernization-kit](https://github.com/Hondarer/c-modernization-kit) の `app/sqlite` として管理される、個別アプリです。  
c-modernization-kit のワークスペース内 (`framework/makefw` 等と組み合わせた make ビルド環境) で利用することを前提としたアプリであり、本ディレクトリ単体ではビルドできません。

## 固有の公開 API 名で OS のライブラリと分離します

OS の同名ライブラリと同じプロセスで関数解決が混ざることを防ぐため、公開関数・変数と関数呼び出しマクロに `sqlite3_app_` を付けます。  
たとえば `sqlite3_open` の利用側は `sqlite3_app_sqlite3_open` を呼び出します。  
型名、定数、構造体フィールドは配布ライブラリの定義を使います。  
旧名の alias と mock ラッパーは提供しないため、既存の利用側はソースを更新して再ビルド・再リンクしてください。  
`dlsym()` と `GetProcAddress()` に渡す文字列も固有名へ変更します。

## 概要

[SQLite](https://www.sqlite.org/) の amalgamation (単一ファイルにまとめられたソース配布形式。`sqlite3.c` / `sqlite3.h` / `sqlite3ext.h`) および公式 CLI シェル (`shell.c`) を、c-modernization-kit の makefw 規約に沿って取り込んだラッパー ライブラリです。  
SQLite のリリース アーカイブを展開し、本体への変更は [パッチ](patches/README.md) 経由で適用します。

SQLite を利用するプログラムの単体テスト向けに、Google Mock 対応の SQLite API モックも含みます。

ビルド成果物は動的ライブラリと、`shell.c` から生成する `sqlite3` コマンド (公式 CLI) です。

- Linux: `libsqlite3_app.so`
- Windows: `libsqlite3_app.dll` およびリンク用の import library `libsqlite3_app.lib`

静的ライブラリは生成しません。

`sqlite3` コマンドは行編集オプション (`-DHAVE_READLINE` 等) を付与しない最小構成でビルドしており、`readline`/`editline` 等の外部ライブラリには依存しません。

`sqlite3ext.h` を使う拡張でも、呼び出しには `sqlite3_app_` 付きの API 名を使います。  
`sqlite3_api_routines` のフィールドとレイアウト、外部拡張の入口名は維持します。  
既存の拡張は、新しいヘッダーで再ビルドしてください。

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

配置後、`make` (または `make test`) を実行すると、`app/sqlite/bin_internal/extract_package.py` が自動的に `prod/include/`、`prod/libsrc/sqlite3/`、`prod/src/cmd/sqlite3/` へ展開します。  
展開先はいずれも生成物であり `.gitignore` 対象です。

展開に続けて、`app/sqlite/patches/` の unified diff を自動的に適用します。  
適用器は `framework/makefw/bin_internal/apply_patches.py` (app 間で共有する適用器) です。  
sqlite3 本体への変更はすべてこのパッチ経由で行い、展開直後のファイルを直接編集することはありません。  
パッチの一覧と個々の目的は [patches/README.md](patches/README.md) を参照してください。

`packages/` にアーカイブが存在しない状態で `make` を実行すると、ビルドはエラーで停止し、配置方法の案内が表示されます。

`packages/` に複数のアーカイブが存在する場合はエラーにはせず、ファイル名のバージョン番号が最も新しいものを自動的に採用します (バージョン番号が読み取れないファイルが混在する場合は、更新日時が最も新しいものを採用します)。  
この場合、採用されなかったアーカイブを削除するよう警告が表示されるので、単一ファイル運用に戻してください。

## バージョン更新手順

1. 新しいバージョンの zip を取得し、`app/sqlite/packages/` に追加します。
2. 古いバージョンの zip を削除します (`packages/` には常に 1 個のみを配置する運用)。
3. `make` を実行すると、新しい zip のタイムスタンプが展開済み生成物より新しいと判定され、自動的に再展開されます。

## ライセンス

SQLite 本体は Public Domain です。単体の `LICENSE` ファイルはアーカイブ内に同梱されておらず、 `sqlite3.h` 冒頭のコメントに著作権放棄の宣言 (blessing) が記載されています。詳細は [https://www.sqlite.org/copyright.html](https://www.sqlite.org/copyright.html) を参照してください。

`app/sqlite` 直下の `LICENSE` (MIT License) は、本ディレクトリのラッパー コード (`bin_internal/extract_package.py`、`makefile`/`makepart.mk` 等の手書きファイル) に対する著作権表示であり、`sqlite3.c`/`sqlite3.h`/`shell.c` 等の SQLite 本体には適用されません。

## サンプルとテスト

- `prod/src/cmd/sqlite3/` : `shell.c` (生成物) から構築する公式 CLI シェル
- `test/src/cmd/sqliteTest/` : 動作テスト
- `test/src/libsqlite3Test/mockSqlite3Test/` : SQLite API モックの動作テスト
- `test/src/libsqlite3Test/exportTest/` : API 表、公開変数、Windows 固有シンボル、IDENT manifest、実ライブラリの全エクスポートが一致することを検出するテスト

## SQLite API モック

テスト対象ソースを `TEST_SRCS` に指定し、mock をリンクするテストの `makepart.mk` に `DEFINES += MOCK_SQLITE3_ENABLE_CALLER` を追加します。  
これにより呼び出し元情報の注入を有効にし、実ライブラリを使うテストには注入しません。

mock の宣言と注入マクロは API 表から生成し、mock ライブラリのビルド前に自動更新します。  
生成済みの `test/include/mock_sqlite3_overrides.h` は直接編集せず、通常 API 表と、可変長 API がある場合は専用の可変長 API 表を変更してください。

通常の API 呼び出しは差し替えヘッダーを通し、mock の先頭 3 引数へ呼び出し元のファイル名、行番号、関数名を渡します。  
`EXPECT_CALL` と `ON_CALL` でも、この 3 引数を実 API の引数より先に指定します。  
新しい固有名の関数アドレスを取得して関数ポインター経由で呼び出す場合も mock に到達しますが、呼び出し元は取得できないため、ファイル名と関数名は `"<unknown>"`、行番号は `0` になります。  
実委譲は app の固有シンボルだけを解決し、OS 側の旧名へ戻す探索は行いません。

SQLite を利用するアプリの単体テストでは、`<mock_sqlite3.h>` と `libmock_sqlite3` を使用できます。  
テスト対象の `makepart.mk` では、`sqlite3_app` の代わりに `mock_sqlite3` をリンクします。  
`sqlite3_app` と `mock_sqlite3` を同時にリンクしないでください。Linux では実ライブラリの強シンボルが弱定義のモックを上書きし、`EXPECT_CALL` が効かなくなります。

```makefile
ifdef PLATFORM_WINDOWS
    DEFINES += SQLITE_API=
endif

LIBS += mock_sqlite3
```

Windows では、`SQLITE_API=` により SQLite の DLL import 宣言を無効にし、`mock_sqlite3` が提供する実シンボルを参照します。  
`Mock_sqlite3` を生成しない場合と、生成後に `EXPECT_CALL` や `ON_CALL` を設定しない場合は、動的ライブラリの実関数を呼び出します。  
実関数への委譲では、Linux の `LD_LIBRARY_PATH` または Windows の `PATH` から `libsqlite3_app` を読み込みます。

振る舞いを変更するテストでは、`Mock_sqlite3` を生成して `EXPECT_CALL` または `ON_CALL` を指定します。

```cpp
NiceMock<Mock_sqlite3> mock_sqlite3;
EXPECT_CALL(mock_sqlite3, sqlite3_app_sqlite3_open(_, _, _, StrEq(":memory:"), _)).WillOnce(Return(SQLITE_CANTOPEN));
```

実のデータベースを開かずに単体隔離する場合は、SUT が呼び出す関数をすべてスタブしてください。  
`sqlite3_app_sqlite3_open` を成功させるときは、`SetArgPointee` で `sqlite3 **` に偽ハンドルを設定します。  
スタブしていない呼び出しは実関数へ委譲されるため、偽ハンドルを渡すと実関数が失敗します。

```cpp
sqlite3 *fake_database = reinterpret_cast<sqlite3 *>(1);
EXPECT_CALL(mock_sqlite3, sqlite3_app_sqlite3_open(_, _, _, StrEq(":memory:"), _))
    .WillOnce(DoAll(SetArgPointee<4>(fake_database), Return(SQLITE_OK)));
```

`sqlite3_app_sqlite3_config`、`sqlite3_app_sqlite3_db_config`、`sqlite3_app_sqlite3_mprintf` などの可変長引数 API は、可変長部分を `va_list` としてモックへ渡します。  
引数の内容を照合しない場合は、該当する `va_list` 引数に `_` を指定してください。

公開変数 `sqlite3_app_sqlite3_version`、`sqlite3_app_sqlite3_temp_directory`、`sqlite3_app_sqlite3_data_directory` は関数モックの対象外です。  
`mock_sqlite3` はこれらの変数を定義するので、実ライブラリなしでもリンクできます。  
`sqlite3_app_sqlite3_version` の初期値は `SQLITE_VERSION` です。`sqlite3_app_sqlite3_temp_directory` と `sqlite3_app_sqlite3_data_directory` の初期値は `NULL` です。

`exportTest` は、公開ヘッダー `sqlite3.h` に載る関数と公開変数を検査します。  
Windows の DLL に追加される `sqlite3.h` に無い `sqlite3_app_sqlite3_win32_*` と IDENT manifest も期待値へ明示し、実ライブラリの全エクスポートと比較します。

`sqlite3ext.h` の拡張 API 表はモック対象外です。
