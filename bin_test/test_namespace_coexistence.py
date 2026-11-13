#!/usr/bin/env python3
"""Verify app API isolation from an original-name shared library."""
import os
import subprocess
import tempfile
import sys
import unittest
from pathlib import Path

BIN_TEST_PLATFORM = "linux"
APP_DIR = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(APP_DIR.parents[1] / "framework" / "testfw" / "bin_internal"))
from shared_namespace_probe import verify_namespace

@unittest.skipUnless(sys.platform.startswith("linux"), "[Linux] ELF namespace probe requires Linux")
class NamespaceCoexistenceTest(unittest.TestCase):
    def test_both_global_load_orders(self):
        verify_namespace(APP_DIR, 'sqlite3_app', 'sqlite3_exec', '#include <sqlite3.h>\nint namespace_probe(void) { sqlite3 *db=0; if(sqlite3_app_sqlite3_open(":memory:",&db)!=SQLITE_OK) return 10; int rc=sqlite3_app_sqlite3_exec(db,"CREATE TABLE probe(value INTEGER)",0,0,0); int close_rc=sqlite3_app_sqlite3_close(db); return rc!=SQLITE_OK || close_rc!=SQLITE_OK; }\n')


    def test_extension_headers_use_unique_api_names(self):
        with tempfile.TemporaryDirectory(prefix="sqlite-extension-api-") as directory:
            source = Path(directory) / "extension.c"
            for core in (False, True):
                text = "#define SQLITE_CORE\n" if core else ""
                text += "#include <sqlite3.h>\n#include <sqlite3ext.h>\n"
                if not core:
                    text += "SQLITE_EXTENSION_INIT1\n"
                text += "int probe(void) { return sqlite3_app_sqlite3_libversion_number(); }\n"
                source.write_text(text, encoding="utf-8")
                subprocess.run([os.environ.get("CC", "cc"), "-Werror", "-fsyntax-only",
                                "-I" + str(APP_DIR / "prod" / "include"), str(source)],
                               check=True, capture_output=True, encoding="utf-8")

if __name__ == "__main__":
    unittest.main()
