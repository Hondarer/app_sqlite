# ビルド済みライブラリの検索パス
LIBSDIR += \
	$(MYAPP_DIR)/prod/lib

# ライブラリの指定
#
# 本テストは sqlite3.c 自体のカバレッジ計測ではなく、
# app/sqlite が提供する API (libsqlite3.a) の独自動作確認を目的とする。
# そのため TEST_SRCS は使用せず、ビルド済みライブラリへの通常のリンクで検証する。
LIBS += sqlite3

ifdef PLATFORM_LINUX
    # sqlite3.c の拡張ロード機能 (dlopen 等) を利用するため libdl が必要
    LIBS += dl
endif
