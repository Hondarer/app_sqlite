#!/usr/bin/env python3
"""公式パッケージ展開と公開 API namespace 生成を検証する。"""
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

APP = Path(__file__).resolve().parents[1]
MAKEFW = APP.parents[1] / "framework" / "makefw"


class ExtractPackageTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="sqlite-extract-test-")
        self.addCleanup(self.temp.cleanup)
        self.app = Path(self.temp.name)
        shutil.copytree(APP / "packages", self.app / "packages")
        shutil.copytree(APP / "patches", self.app / "patches")
        (self.app / "prod").mkdir()
        shutil.copyfile(APP / "prod/public_symbols.txt", self.app / "prod/public_symbols.txt")
        shutil.copyfile(APP / "prod/public_api_macros.txt", self.app / "prod/public_api_macros.txt")

    def run_extract(self):
        return subprocess.run(
            [sys.executable, str(APP / "bin_internal/extract_package.py"),
             "--app-dir", str(self.app), "--makefw-home", str(MAKEFW)],
            capture_output=True, text=True, encoding="utf-8", check=False)

    def test_namespace_generation_and_noop(self):
        result = self.run_extract()
        self.assertEqual(0, result.returncode, result.stderr)
        header = self.app / "prod/include/sqlite3.h"
        self.assertIn("sqlite3_app_sqlite3_open", header.read_text(encoding="utf-8"))
        extension_header = (self.app / "prod/include/sqlite3ext.h").read_text(encoding="utf-8")
        self.assertIn("(*open)(", extension_header)
        self.assertIn("#define sqlite3_app_sqlite3_open", extension_header)
        self.assertIn("sqlite3_api->open", extension_header)
        namespace = self.app / "prod/include_internal/sqlite3_app_namespace.h"
        self.assertTrue(namespace.is_file())
        # 列メタデータ API は sqlite3.c が 0 に #define するため、名前空間マクロを生成しない。
        self.assertNotIn("#define sqlite3_column_database_name ", namespace.read_text(encoding="utf-8"))
        before = {p: p.stat().st_mtime_ns for p in self.app.rglob("*") if p.is_file()}
        result = self.run_extract()
        self.assertEqual(0, result.returncode, result.stderr)
        self.assertEqual(before, {p: p.stat().st_mtime_ns for p in before})


    def test_symbol_list_change_triggers_regeneration(self):
        result = self.run_extract()
        self.assertEqual(0, result.returncode, result.stderr)
        symbols = self.app / "prod/public_symbols.txt"
        with symbols.open("a", encoding="utf-8", newline="\n") as stream:
            stream.write("_NamespaceProbe\n")
        result = self.run_extract()
        self.assertEqual(0, result.returncode, result.stderr)
        namespace = self.app / "prod/include_internal/sqlite3_app_namespace.h"
        self.assertIn("#define _NamespaceProbe sqlite3_app__NamespaceProbe", namespace.read_text(encoding="utf-8"))

    def test_missing_namespace_header_is_regenerated(self):
        result = self.run_extract()
        self.assertEqual(0, result.returncode, result.stderr)
        namespace = self.app / "prod/include_internal/sqlite3_app_namespace.h"
        namespace.unlink()
        result = self.run_extract()
        self.assertEqual(0, result.returncode, result.stderr)
        self.assertTrue(namespace.is_file())


if __name__ == "__main__":
    unittest.main()
