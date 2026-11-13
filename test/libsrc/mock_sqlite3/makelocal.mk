.PHONY: pre-build
pre-build:
	python3 "$(TESTFW_HOME)/bin_internal/generate_shared_lib_mock_overrides.py" "$(MYAPP_DIR)/test/include/mock_sqlite3_api_table.h" "$(MYAPP_DIR)/test/include/mock_sqlite3_overrides.h" --macro-prefix MOCK_SQLITE3 --variadic-api-table "$(MYAPP_DIR)/test/include/mock_sqlite3_variadic_api_table.h"
