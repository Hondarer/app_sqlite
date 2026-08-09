#!/usr/bin/env python3
"""app/sqlite/bin/extract_package.py

packages/ 配下の sqlite amalgamation アーカイブ (zip) を prod/include,
prod/libsrc/sqlite3, prod/src/cmd/sqlite3 へ展開する。外部ツール (unzip 等) に
依存せず、標準ライブラリ zipfile のみを使用する。
"""

import argparse
import os
import re
import stat
import sys
import tempfile
import zipfile

sys.stdout.reconfigure(encoding="utf-8")
sys.stderr.reconfigure(encoding="utf-8")

PACKAGE_NAME_PATTERN = re.compile(r"^sqlite-amalgamation-.*\.zip$", re.IGNORECASE)
VERSION_PATTERN = re.compile(r"^sqlite-amalgamation-(\d+)\.zip$", re.IGNORECASE)

SQLITE_HEADER_PREFIX = b"""/* Use DLL import by default for Windows consumers. */
#if !defined(SQLITE_API)
#if defined(__WINDOWS__) || defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32)
#define SQLITE_API __declspec(dllimport)
#elif defined(__GNUC__)
#define SQLITE_API __attribute__((visibility(\"default\")))
#endif
#endif

"""
SQLITE_SOURCE_PREFIX = b"""/* Export the public API from the shared library. */
#if !defined(SQLITE_API)
#if defined(__WINDOWS__) || defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32)
#define SQLITE_API __declspec(dllexport)
#elif defined(__GNUC__)
#define SQLITE_API __attribute__((visibility(\"default\")))
#endif
#endif

"""

# 展開対象: zip 内のファイル名 -> 展開先 (プレースホルダーは app_dir からの相対パス)
#
# sqlite3.c は内部で #include "sqlite3.h" のように同一ディレクトリ相対
# (ダブルクォート) インクルードを使うため、cJSON と同様にヘッダーは
# prod/include 直下 (アンブレラ ヘッダー パターン) に配置する。
# shell.c (公式 CLI) は lua フェーズで lua.c を取り込む方針と対称にするため、
# prod/src/cmd/sqlite3/ に取り込み sqlite3 コマンドとしてビルドする。
EXTRACT_TARGETS = {
    "sqlite3.h": ("prod", "include", "sqlite3.h"),
    "sqlite3ext.h": ("prod", "include", "sqlite3ext.h"),
    "sqlite3.c": ("prod", "libsrc", "sqlite3", "sqlite3.c"),
    "shell.c": ("prod", "src", "cmd", "sqlite3", "shell.c"),
}

# 再展開要否の判定に使う代表ファイル
MARKER_SOURCE = "sqlite3.c"
MARKER_TARGET = ("prod", "libsrc", "sqlite3", "sqlite3.c")

# 生成物を除外するための .gitignore を配置するディレクトリと、その内容。
#
# framework/makefw/makefiles/makelibsrc_c_cpp.mk / makesrc_c_cpp.mk は、
# TEST_SRCS/ADD_SRCS を使わないビルド リーフ ディレクトリでは、
# `make clean` のたびに無条件で .gitignore を削除する (app/cjson で判明した
# 挙動と同じ)。prod/libsrc/sqlite3、prod/src/cmd/sqlite3 はいずれも該当するため、
# ここで毎回 (再展開の有無に関わらず) .gitignore を再生成し、`make clean` 後も
# 次回 make で必ず復元されるようにする。
GITIGNORE_TARGETS = {
    ("prod", "libsrc", "sqlite3"): ["sqlite3.c"],
    ("prod", "src", "cmd", "sqlite3"): ["shell.c"],
}


GITIGNORE_HEADER = "# app/sqlite/packages 配下の zip から展開される生成物。手動改変しないため Git 管理対象外とする。\n"


def atomic_replace(path, data):
    """同じディレクトリの一意な一時ファイルを使ってファイルを置換する。"""
    dir_path = os.path.dirname(path)
    prefix = f".{os.path.basename(path)}."
    try:
        file_mode = stat.S_IMODE(os.stat(path).st_mode)
    except FileNotFoundError:
        current_umask = os.umask(0)
        os.umask(current_umask)
        file_mode = 0o666 & ~current_umask
    tmp_path = None
    try:
        if isinstance(data, str):
            with tempfile.NamedTemporaryFile(
                mode="w",
                encoding="utf-8",
                newline="",
                dir=dir_path,
                prefix=prefix,
                suffix=".tmp",
                delete=False,
            ) as f:
                tmp_path = f.name
                f.write(data)
        else:
            with tempfile.NamedTemporaryFile(
                mode="wb",
                dir=dir_path,
                prefix=prefix,
                suffix=".tmp",
                delete=False,
            ) as f:
                tmp_path = f.name
                f.write(data)
        os.chmod(tmp_path, file_mode)
        os.replace(tmp_path, path)
    finally:
        if tmp_path is not None:
            try:
                os.unlink(tmp_path)
            except FileNotFoundError:
                pass


def iter_target_paths(app_dir):
    for rel_parts in EXTRACT_TARGETS.values():
        yield os.path.join(app_dir, *rel_parts)


def ensure_gitignore(app_dir):
    for rel_parts, names in GITIGNORE_TARGETS.items():
        dir_path = os.path.join(app_dir, *rel_parts)
        os.makedirs(dir_path, exist_ok=True)
        gitignore_path = os.path.join(dir_path, ".gitignore")
        content = GITIGNORE_HEADER + "".join(f"/{name}\n" for name in names)
        atomic_replace(gitignore_path, content)


def find_candidates(packages_dir):
    if not os.path.isdir(packages_dir):
        return []
    return sorted(f for f in os.listdir(packages_dir) if PACKAGE_NAME_PATTERN.match(f))


def parse_version(filename):
    m = VERSION_PATTERN.match(filename)
    if m is None:
        return None
    return int(m.group(1))


def select_package(packages_dir, candidates):
    """複数候補がある場合、ファイル名のバージョン番号が最も新しいものを採用する。
    バージョン番号が抽出できないファイルが混在する場合は、mtime が最も新しい
    ものにフォールバックする。"""
    if len(candidates) == 1:
        return candidates[0], []

    versions = {name: parse_version(name) for name in candidates}
    if all(v is not None for v in versions.values()):
        selected = max(candidates, key=lambda name: versions[name])
    else:
        selected = max(
            candidates,
            key=lambda name: os.path.getmtime(os.path.join(packages_dir, name)),
        )
    rejected = [name for name in candidates if name != selected]
    return selected, rejected


def print_missing_package_guide(packages_dir):
    lines = [
        "",
        "ERROR: sqlite amalgamation のアーカイブ (zip) が app/sqlite/packages に見つかりません。",
        "",
        f"  配置先: {packages_dir}",
        "  ファイル名の例: sqlite-amalgamation-3460100.zip (SQLITE_VERSION_NUMBER 形式の 7 桁を含む)",
        "",
        "  取得方法:",
        "    1. https://www.sqlite.org/download.html を開く",
        "    2. 'Source Code' 節にある amalgamation zip (sqlite-amalgamation-<version>.zip) の",
        "       リンク先 URL を確認する (年別ディレクトリを含む URL のため、バージョンごとに変わる)",
        "    3. 確認した URL から curl で取得する (例):",
        "         curl -L -o app/sqlite/packages/sqlite-amalgamation-3460100.zip \\",
        "           https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip",
        "",
        "  取得後、このディレクトリには常に 1 個の zip のみを配置してください。",
        "  バージョン更新時は、古いアーカイブを新しいものに置き換えてください。",
        "",
    ]
    print("\n".join(lines), file=sys.stderr)


def print_multiple_package_warning(selected, rejected):
    lines = [
        "",
        "WARNING: app/sqlite/packages に複数の sqlite amalgamation アーカイブが見つかりました。",
        f"  採用: {selected} (バージョンが最も新しいと判断)",
    ]
    lines += [f"  未採用: {name}" for name in rejected]
    lines += [
        "  単一ファイル運用のため、未採用のアーカイブは削除してください。",
        "",
    ]
    print("\n".join(lines), file=sys.stderr)


def find_member(names, filename):
    """zip 内から <トップディレクトリ>/filename に一致するメンバー名を返す。"""
    matches = [n for n in names if n.split("/", 1)[-1] == filename and n.count("/") == 1]
    return matches[0] if matches else None


def needs_extraction(zip_path, app_dir):
    if any(not os.path.isfile(path) for path in iter_target_paths(app_dir)):
        return True

    marker = os.path.join(app_dir, *MARKER_TARGET)
    if os.path.getmtime(zip_path) > os.path.getmtime(marker):
        return True

    header = os.path.join(app_dir, "prod", "include", "sqlite3.h")
    source = os.path.join(app_dir, "prod", "libsrc", "sqlite3", "sqlite3.c")
    with open(header, "rb") as f:
        header_data = f.read()
    with open(source, "rb") as f:
        source_data = f.read()
    return not (
        header_data.startswith(SQLITE_HEADER_PREFIX)
        and source_data.startswith(SQLITE_SOURCE_PREFIX)
    )


def prepare_extracted_data(src_name, data):
    if src_name == "sqlite3.h":
        return SQLITE_HEADER_PREFIX + data
    if src_name == "sqlite3.c":
        return SQLITE_SOURCE_PREFIX + data
    return data


def extract(zip_path, app_dir):
    dest_paths = {}
    for src_name, rel_parts in EXTRACT_TARGETS.items():
        dest_path = os.path.join(app_dir, *rel_parts)
        os.makedirs(os.path.dirname(dest_path), exist_ok=True)
        dest_paths[src_name] = dest_path

    with zipfile.ZipFile(zip_path) as zf:
        names = zf.namelist()
        # 代表ファイルを最後に置換し、全出力の準備前に別プロセスが
        # 展開完了と判定しないようにする。
        ordered_sources = [name for name in dest_paths if name != MARKER_SOURCE]
        ordered_sources.append(MARKER_SOURCE)
        for src_name in ordered_sources:
            dest_path = dest_paths[src_name]
            member = find_member(names, src_name)
            if member is None:
                print(f"ERROR: zip 内に {src_name} が見つかりません: {zip_path}", file=sys.stderr)
                return False
            data = prepare_extracted_data(src_name, zf.read(member))
            atomic_replace(dest_path, data)

    zip_mtime = os.path.getmtime(zip_path)
    for dest_path in dest_paths.values():
        os.utime(dest_path, (zip_mtime, zip_mtime))
    return True


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--app-dir", required=True)
    args = parser.parse_args()

    packages_dir = os.path.join(args.app_dir, "packages")
    candidates = find_candidates(packages_dir)

    if not candidates:
        print_missing_package_guide(packages_dir)
        return 1

    selected, rejected = select_package(packages_dir, candidates)
    if rejected:
        print_multiple_package_warning(selected, rejected)

    zip_path = os.path.join(packages_dir, selected)

    ensure_gitignore(args.app_dir)

    if not needs_extraction(zip_path, args.app_dir):
        return 0

    print(f"INFO: sqlite amalgamation パッケージを展開しています: {selected}", file=sys.stderr)
    ok = extract(zip_path, args.app_dir)
    return 0 if ok else 2


if __name__ == "__main__":
    sys.exit(main())
