# patches

このディレクトリには、`packages/` の配布アーカイブ (sqlite amalgamation) から展開した sqlite3 本体へ適用するパッチを配置します。

展開された `prod/include/sqlite3.h`、`prod/include/sqlite3ext.h`、`prod/libsrc/sqlite3/sqlite3.c`、`prod/src/cmd/sqlite3/shell.c` は生成物です。  
これらを直接編集しないでください。再展開で上書きされます。  
sqlite3 本体へ手を入れる唯一の方法は、このディレクトリへパッチを追加することです。

## 適用の仕組み

`make` 実行時に `bin_internal/extract_package.py` がアーカイブを展開し、続けてこのディレクトリのパッチを適用します。  
適用器は `framework/makefw/bin_internal/apply_patches.py` です。標準ライブラリだけで動作し、`patch` や `git apply` に依存しません。

適用は **厳密** です。文脈行が 1 バイトでも一致しない場合、探索や fuzz による救済を行わずビルドを停止します。  
アーカイブのバージョンを更新した際にパッチが当たらなくなったら、それは上流の変更を確認すべき合図です。

## ファイルの規約

- ファイル名は `NNNN-<要約>.patch` とします。適用順はファイル名の昇順です。
- 形式は unified diff です。`--- a/<path>` と `+++ b/<path>` の見出しを持ちます。
- パスは先頭 1 階層を除去したうえで、`app/sqlite` からの相対パスとして解決します (`git apply -p1` 相当)。
- 差分だけを書き、説明文は含めないでください。パッチの意図はこの README に記載します。

## 収録しているパッチ

| ファイル | 対象 | 目的 |
|---|---|---|
| `0001-windows-dll-api-macro.patch` | `prod/include/sqlite3.h`、`prod/libsrc/sqlite3/sqlite3.c` | `SQLITE_API` が未定義のとき、Windows では利用側 (ヘッダー) を DLL import、ビルド側 (ソース) を DLL export の既定にする。GCC 系では可視性を `default` にする。 |

`0001` はヘッダーとソースの双方に、同一目的 (Windows での DLL import/export 既定値の付与) を適用する変更のため、1 本のパッチにまとめています。  
両ファイルとも変更はファイル先頭への挿入のみで、対象ファイルとインクルード方向 (import/export) が対になっているため、分割しても関連性を追いにくくなるだけで独立した更新単位にはなりません。

## パッチの追加と再生成

1. 展開済みのファイルを別の場所へ複製します。
2. 複製を編集します。
3. `diff -u` で差分を取り、見出しの行を `--- a/<相対パス>` と `+++ b/<相対パス>` に書き換えます (タイムスタンプは残さないでください)。
4. このディレクトリへ配置し、`make clean` の後に `make` を実行して適用されることを確認します。

パッチを追加または変更すると `make_extract.stamp` のダイジェストが変わり、次回の `make` で自動的に再展開と再適用が行われます。
