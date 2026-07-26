# packages

このディレクトリには、[sqlite.org の Download ページ](https://www.sqlite.org/download.html) から取得した sqlite の amalgamation アーカイブ (zip) を配置します。

- ファイル名の例: `sqlite-amalgamation-3460100.zip` (`SQLITE_VERSION_NUMBER` 形式の 7 桁をファイル名に含めること)
- 常に 1 個のみを配置する運用とします。バージョン更新時は、古いアーカイブを削除し、新しいアーカイブに置き換えてください。
- `make` 実行時、`app/sqlite/bin/extract_package.py` がここに配置された zip を自動検出し、`prod/include/`, `prod/libsrc/sqlite3/`, `prod/src/cmd/sqlite3/` へ展開します。
- 万一複数のアーカイブが存在する場合はエラーにはせず、ファイル名のバージョン番号が最も新しいものを自動的に採用し、未採用のアーカイブを削除するよう警告を表示します。
- ダウンロード URL は年別ディレクトリを含むため (例: `https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip`)、バージョンごとに `download.html` から実際の URL を確認してください。

取得方法や配置手順の詳細は [../README.md](../README.md) を参照してください。
