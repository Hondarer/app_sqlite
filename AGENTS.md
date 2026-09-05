# AGENTS.md

## 対象

このリポジトリは、SQLite amalgamation をワークスペースへ取り込むラッパーであり、SQLite を利用するプログラムの単体テスト向け API モックも含みます。

## 参照先

作業に関係する文書の該当節を参照してください。

- [README.md](README.md)
- パッケージの展開と更新では、README.md の手順を確認してください。

## 注意点

- 展開された SQLite 本体を直接編集しないでください。
- Windows の mock 利用側では `SQLITE_API=`、リンク先では `mock_sqlite3` を使用し、実ライブラリを同時リンクしないでください。
- `sqlite3ext.h` と公開変数は関数 API 表と別に扱ってください。
