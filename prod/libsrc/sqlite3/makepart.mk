# sqlite amalgamation (sqlite3.c/h, sqlite3ext.h) を取り込む。
#
# 本ディレクトリの .c ファイルおよび ../../include/ 配下のヘッダーは
# app/sqlite/packages 配下の zip から機械的に展開される生成物であり、
# 手動改変しない (再展開で上書きされる)。そのため、本リポジトリの
# コーディング規範 (goto・三項演算子の不使用、新規ファイルへの
# clang-format 適用) は sqlite 本体には適用しない。
# (このディレクトリの makefile/makepart.mk 自体は手書きファイルであり、
#  通常どおり規範・clang-format の対象とする)

# LIB_TYPE は指定しない (デフォルト static -> libsqlite3.a)。
# shared 版は作らない。
