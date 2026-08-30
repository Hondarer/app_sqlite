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
#
# --makefw-home は、展開済み sqlite3.h/sqlite3.c へ unified diff (patches/)
# を適用する framework/makefw/bin/apply_patches.py を extract_package.py が
# import するために必要。

ifndef MAKEFW_SYNC_EVAL
    _SQLITE_EXTRACT_STATUS := $(shell python3 "$(MYAPP_DIR)/bin/extract_package.py" --app-dir "$(MYAPP_DIR)" --makefw-home "$(MAKEFW_HOME)" >&2; echo $$?)
    ifneq ($(_SQLITE_EXTRACT_STATUS),0)
        $(error sqlite amalgamation パッケージの準備に失敗しました。上記のメッセージに従って app/sqlite/packages にアーカイブを配置してください)
    endif
endif
