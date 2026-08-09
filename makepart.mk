# app/sqlite/makepart.mk
# sqlite amalgamation パッケージの展開を、app/sqlite 配下のどのディレクトリで
# make を起動しても最初に保証するためのフック。
#
# 注意: TEST_SRCS / SRCS_C の $(wildcard) 判定は Makefile の読み込み時に
# 即時評価されるため、対象ディレクトリの pre-build (ターゲット実行フェーズ)
# では展開が間に合わない。そのため、より上位の (全階層に継承される)
# makepart.mk で $(shell) を用いて「読み込み時」に展開を完了させる。
# see: framework/makefw/docs/makeparts.md
# (app/cjson/makepart.mk と同じ構造。詳細な経緯コメントはそちらを参照)

ifndef MAKEFW_SYNC_EVAL
    _SQLITE_EXTRACT_STATUS := $(shell python3 "$(MYAPP_DIR)/bin/extract_package.py" --app-dir "$(MYAPP_DIR)" >&2; echo $$?)
    ifneq ($(_SQLITE_EXTRACT_STATUS),0)
        $(error sqlite amalgamation パッケージの準備に失敗しました。上記のメッセージに従って app/sqlite/packages にアーカイブを配置してください)
    endif
endif

# 警告抑制 (app/makepart.mk の GCC_WARN_BASE) について:
#
# sqlite3.c/sqlite3.h/shell.c は upstream (外部) 側の設計であり、本リポジトリ側
# ではソースを改変しない方針のためレイアウトや実装スタイルを変更できない。
# 本リポジトリは警告抑制フラグ (-Wno-*) を原則使わない方針だが
# (see: framework/makefw/docs/gcc-warning-guide/README.md)、cJSON と同様に
# 今回はソース非改変を優先し、sqlite 本体に起因する以下の警告に限り
# app/sqlite 配下のコンパイル オプションで例外的に抑制する
# (ユーザー承認済み。cJSON は -Wpadded のみだったが、sqlite は amalgamation の
#  規模が大きく、以下 6 種の警告が sqlite3.c/shell.c 全体に及ぶため範囲を広げた)。
#
# - -Wpadded: 構造体パディング (sqlite3.h の公開 API 構造体。ヘッダー経由で
#   本リポジトリ側のコードにも波及するため、app/sqlite 全体で抑制する)
# - -Wcast-qual: const 修飾子を外す明示キャスト (sqlite3.c/shell.c 内部実装)
# - -Wundef: 未定義マクロの評価 (HAVE_READLINE 等、ビルド設定用の任意定義マクロ)
# - -Wswitch-default: switch 文の default 分岐省略
# - -Wmissing-prototypes / -Wmissing-declarations: 内部関数のプロトタイプ/事前
#   宣言省略 (shell.c。両者は GCC 上で排他的に検出されるため両方抑制する)
# - -Wunused-parameter: 未使用引数 (コールバック シグネチャ統一のための引数)
# - -Wconversion / -Wsign-conversion: 暗黙の型変換と符号変換 (amalgamation 全体に
#   及び、上流を改変せずに解消できない)
#
# app/makepart.mk (親階層) で CFLAGS/CXXFLAGS に GCC_WARN_BASE が設定された後、
# makepart.mk は親から子の順に評価されるため、ここで追記する -Wno-* は
# 常に元の -W* より後方に置かれ、GCC 上で有効に上書きできる。
# -Wmissing-prototypes は C/ObjC 専用 (GCC が C++ では警告を出す) のため
# CFLAGS のみに追記する。それ以外は C/C++ 共通のため両方に追記する。
ifdef PLATFORM_LINUX
    CFLAGS   += -Wno-padded -Wno-cast-qual -Wno-undef -Wno-switch-default -Wno-missing-prototypes -Wno-missing-declarations -Wno-unused-parameter -Wno-conversion -Wno-sign-conversion
    CXXFLAGS += -Wno-padded -Wno-cast-qual -Wno-undef -Wno-switch-default -Wno-missing-declarations -Wno-unused-parameter -Wno-conversion -Wno-sign-conversion
endif

# C4200: 非標準の拡張機能 (構造体/共用体中のサイズ 0 の配列)。sqlite3.c 内部の
# フレキシブル配列メンバー (upstream 側の設計) に起因する。
ifdef PLATFORM_WINDOWS
    CFLAGS   += /wd4200
    CXXFLAGS += /wd4200
endif
