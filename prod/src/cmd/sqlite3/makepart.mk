# 公式 CLI シェル (shell.c) から sqlite3 コマンドをビルドする。
#
# shell.c は app/sqlite/packages 配下の zip から機械的に展開される生成物であり、
# 手動改変しない (再展開で上書きされる)。行編集オプション (-DHAVE_READLINE 等) は
# 付与せず、readline/editline への外部依存を避けた最小構成でビルドする。

LIBS += sqlite3

ifdef PLATFORM_LINUX
    # sqlite3.c の拡張ロード機能 (dlopen 等) を利用するため libdl が必要
    LIBS += dl
endif
