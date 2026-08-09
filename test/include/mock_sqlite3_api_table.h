/* API 表として複数回インクルードするため、インクルード ガードを定義しない。 */
#if defined(_WIN32)
MOCK_SQLITE3_RET(int, sqlite3_win32_set_directory, (unsigned long type, void *value), (type, value), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_win32_set_directory8, (unsigned long type, const char *value), (type, value), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_win32_set_directory16, (unsigned long type, const void *value), (type, value), (_, _))
#endif /* _WIN32 */
MOCK_SQLITE3_RET(void *, sqlite3_aggregate_context, (sqlite3_context * arg1, int nBytes), (arg1, nBytes), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_aggregate_count, (sqlite3_context * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_auto_extension, (void (*xEntryPoint)(void)), (xEntryPoint), (_))
MOCK_SQLITE3_RET(int, sqlite3_autovacuum_pages,
                 (sqlite3 * db, unsigned int (*arg2)(void *, const char *, unsigned int, unsigned int, unsigned int),
                  void *arg3, void (*arg4)(void *)),
                 (db, arg2, arg3, arg4), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_backup_finish, (sqlite3_backup * p), (p), (_))
MOCK_SQLITE3_RET(sqlite3_backup *, sqlite3_backup_init,
                 (sqlite3 * pDest, const char *zDestName, sqlite3 *pSource, const char *zSourceName),
                 (pDest, zDestName, pSource, zSourceName), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_backup_pagecount, (sqlite3_backup * p), (p), (_))
MOCK_SQLITE3_RET(int, sqlite3_backup_remaining, (sqlite3_backup * p), (p), (_))
MOCK_SQLITE3_RET(int, sqlite3_backup_step, (sqlite3_backup * p, int nPage), (p, nPage), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_blob, (sqlite3_stmt * arg1, int arg2, const void *arg3, int n, void (*arg5)(void *)),
                 (arg1, arg2, arg3, n, arg5), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_blob64,
                 (sqlite3_stmt * arg1, int arg2, const void *arg3, sqlite3_uint64 arg4, void (*arg5)(void *)),
                 (arg1, arg2, arg3, arg4, arg5), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_double, (sqlite3_stmt * arg1, int arg2, double arg3), (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_int, (sqlite3_stmt * arg1, int arg2, int arg3), (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_int64, (sqlite3_stmt * arg1, int arg2, sqlite3_int64 arg3), (arg1, arg2, arg3),
                 (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_null, (sqlite3_stmt * arg1, int arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_parameter_count, (sqlite3_stmt * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_bind_parameter_index, (sqlite3_stmt * arg1, const char *zName), (arg1, zName), (_, _))
MOCK_SQLITE3_RET(const char *, sqlite3_bind_parameter_name, (sqlite3_stmt * arg1, int arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_pointer,
                 (sqlite3_stmt * arg1, int arg2, void *arg3, const char *arg4, void (*arg5)(void *)),
                 (arg1, arg2, arg3, arg4, arg5), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_text,
                 (sqlite3_stmt * arg1, int arg2, const char *arg3, int arg4, void (*arg5)(void *)),
                 (arg1, arg2, arg3, arg4, arg5), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_text16,
                 (sqlite3_stmt * arg1, int arg2, const void *arg3, int arg4, void (*arg5)(void *)),
                 (arg1, arg2, arg3, arg4, arg5), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_text64,
                 (sqlite3_stmt * arg1, int arg2, const char *arg3, sqlite3_uint64 arg4, void (*arg5)(void *),
                  unsigned char encoding),
                 (arg1, arg2, arg3, arg4, arg5, encoding), (_, _, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_value, (sqlite3_stmt * arg1, int arg2, const sqlite3_value *arg3),
                 (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_zeroblob, (sqlite3_stmt * arg1, int arg2, int n), (arg1, arg2, n), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_bind_zeroblob64, (sqlite3_stmt * arg1, int arg2, sqlite3_uint64 arg3), (arg1, arg2, arg3),
                 (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_blob_bytes, (sqlite3_blob * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_blob_close, (sqlite3_blob * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_blob_open,
                 (sqlite3 * arg1, const char *zDb, const char *zTable, const char *zColumn, sqlite3_int64 iRow,
                  int flags, sqlite3_blob **ppBlob),
                 (arg1, zDb, zTable, zColumn, iRow, flags, ppBlob), (_, _, _, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_blob_read, (sqlite3_blob * arg1, void *Z, int N, int iOffset), (arg1, Z, N, iOffset),
                 (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_blob_reopen, (sqlite3_blob * arg1, sqlite3_int64 arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_blob_write, (sqlite3_blob * arg1, const void *z, int n, int iOffset),
                 (arg1, z, n, iOffset), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_busy_handler, (sqlite3 * arg1, int (*arg2)(void *, int), void *arg3), (arg1, arg2, arg3),
                 (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_busy_timeout, (sqlite3 * arg1, int ms), (arg1, ms), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_cancel_auto_extension, (void (*xEntryPoint)(void)), (xEntryPoint), (_))
MOCK_SQLITE3_RET(int, sqlite3_changes, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(sqlite3_int64, sqlite3_changes64, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_clear_bindings, (sqlite3_stmt * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_close, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_close_v2, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_collation_needed,
                 (sqlite3 * arg1, void *arg2, void (*arg3)(void *, sqlite3 *, int eTextRep, const char *)),
                 (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_collation_needed16,
                 (sqlite3 * arg1, void *arg2, void (*arg3)(void *, sqlite3 *, int eTextRep, const void *)),
                 (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_RET(const void *, sqlite3_column_blob, (sqlite3_stmt * arg1, int iCol), (arg1, iCol), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_column_bytes, (sqlite3_stmt * arg1, int iCol), (arg1, iCol), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_column_bytes16, (sqlite3_stmt * arg1, int iCol), (arg1, iCol), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_column_count, (sqlite3_stmt * pStmt), (pStmt), (_))
MOCK_SQLITE3_RET(const char *, sqlite3_column_decltype, (sqlite3_stmt * arg1, int arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(const void *, sqlite3_column_decltype16, (sqlite3_stmt * arg1, int arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(double, sqlite3_column_double, (sqlite3_stmt * arg1, int iCol), (arg1, iCol), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_column_int, (sqlite3_stmt * arg1, int iCol), (arg1, iCol), (_, _))
MOCK_SQLITE3_RET(sqlite3_int64, sqlite3_column_int64, (sqlite3_stmt * arg1, int iCol), (arg1, iCol), (_, _))
MOCK_SQLITE3_RET(const char *, sqlite3_column_name, (sqlite3_stmt * arg1, int N), (arg1, N), (_, _))
MOCK_SQLITE3_RET(const void *, sqlite3_column_name16, (sqlite3_stmt * arg1, int N), (arg1, N), (_, _))
MOCK_SQLITE3_RET(const unsigned char *, sqlite3_column_text, (sqlite3_stmt * arg1, int iCol), (arg1, iCol), (_, _))
MOCK_SQLITE3_RET(const void *, sqlite3_column_text16, (sqlite3_stmt * arg1, int iCol), (arg1, iCol), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_column_type, (sqlite3_stmt * arg1, int iCol), (arg1, iCol), (_, _))
MOCK_SQLITE3_RET(sqlite3_value *, sqlite3_column_value, (sqlite3_stmt * arg1, int iCol), (arg1, iCol), (_, _))
MOCK_SQLITE3_RET(void *, sqlite3_commit_hook, (sqlite3 * arg1, int (*arg2)(void *), void *arg3), (arg1, arg2, arg3),
                 (_, _, _))
MOCK_SQLITE3_RET(const char *, sqlite3_compileoption_get, (int N), (N), (_))
MOCK_SQLITE3_RET(int, sqlite3_compileoption_used, (const char *zOptName), (zOptName), (_))
MOCK_SQLITE3_RET(int, sqlite3_complete, (const char *sql), (sql), (_))
MOCK_SQLITE3_RET(int, sqlite3_complete16, (const void *sql), (sql), (_))
MOCK_SQLITE3_RET(sqlite3 *, sqlite3_context_db_handle, (sqlite3_context * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_create_collation,
                 (sqlite3 * arg1, const char *zName, int eTextRep, void *pArg,
                  int (*xCompare)(void *, int, const void *, int, const void *)),
                 (arg1, zName, eTextRep, pArg, xCompare), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_create_collation16,
                 (sqlite3 * arg1, const void *zName, int eTextRep, void *pArg,
                  int (*xCompare)(void *, int, const void *, int, const void *)),
                 (arg1, zName, eTextRep, pArg, xCompare), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_create_collation_v2,
                 (sqlite3 * arg1, const char *zName, int eTextRep, void *pArg,
                  int (*xCompare)(void *, int, const void *, int, const void *), void (*xDestroy)(void *)),
                 (arg1, zName, eTextRep, pArg, xCompare, xDestroy), (_, _, _, _, _, _))
MOCK_SQLITE3_RET(sqlite3_filename, sqlite3_create_filename,
                 (const char *zDatabase, const char *zJournal, const char *zWal, int nParam, const char **azParam),
                 (zDatabase, zJournal, zWal, nParam, azParam), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_create_function,
                 (sqlite3 * db, const char *zFunctionName, int nArg, int eTextRep, void *pApp,
                  void (*xFunc)(sqlite3_context *, int, sqlite3_value **),
                  void (*xStep)(sqlite3_context *, int, sqlite3_value **), void (*xFinal)(sqlite3_context *)),
                 (db, zFunctionName, nArg, eTextRep, pApp, xFunc, xStep, xFinal), (_, _, _, _, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_create_function16,
                 (sqlite3 * db, const void *zFunctionName, int nArg, int eTextRep, void *pApp,
                  void (*xFunc)(sqlite3_context *, int, sqlite3_value **),
                  void (*xStep)(sqlite3_context *, int, sqlite3_value **), void (*xFinal)(sqlite3_context *)),
                 (db, zFunctionName, nArg, eTextRep, pApp, xFunc, xStep, xFinal), (_, _, _, _, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_create_function_v2,
                 (sqlite3 * db, const char *zFunctionName, int nArg, int eTextRep, void *pApp,
                  void (*xFunc)(sqlite3_context *, int, sqlite3_value **),
                  void (*xStep)(sqlite3_context *, int, sqlite3_value **), void (*xFinal)(sqlite3_context *),
                  void (*xDestroy)(void *)),
                 (db, zFunctionName, nArg, eTextRep, pApp, xFunc, xStep, xFinal, xDestroy), (_, _, _, _, _, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_create_module,
                 (sqlite3 * db, const char *zName, const sqlite3_module *p, void *pClientData),
                 (db, zName, p, pClientData), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_create_module_v2,
                 (sqlite3 * db, const char *zName, const sqlite3_module *p, void *pClientData,
                  void (*xDestroy)(void *)),
                 (db, zName, p, pClientData, xDestroy), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_create_window_function,
                 (sqlite3 * db, const char *zFunctionName, int nArg, int eTextRep, void *pApp,
                  void (*xStep)(sqlite3_context *, int, sqlite3_value **), void (*xFinal)(sqlite3_context *),
                  void (*xValue)(sqlite3_context *), void (*xInverse)(sqlite3_context *, int, sqlite3_value **),
                  void (*xDestroy)(void *)),
                 (db, zFunctionName, nArg, eTextRep, pApp, xStep, xFinal, xValue, xInverse, xDestroy),
                 (_, _, _, _, _, _, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_data_count, (sqlite3_stmt * pStmt), (pStmt), (_))
MOCK_SQLITE3_RET(sqlite3_file *, sqlite3_database_file_object, (const char *arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_db_cacheflush, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(sqlite3_filename, sqlite3_db_filename, (sqlite3 * db, const char *zDbName), (db, zDbName), (_, _))
MOCK_SQLITE3_RET(sqlite3 *, sqlite3_db_handle, (sqlite3_stmt * arg1), (arg1), (_))
MOCK_SQLITE3_RET(sqlite3_mutex *, sqlite3_db_mutex, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(const char *, sqlite3_db_name, (sqlite3 * db, int N), (db, N), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_db_readonly, (sqlite3 * db, const char *zDbName), (db, zDbName), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_db_release_memory, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_db_status, (sqlite3 * arg1, int op, int *pCur, int *pHiwtr, int resetFlg),
                 (arg1, op, pCur, pHiwtr, resetFlg), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_db_status64,
                 (sqlite3 * arg1, int arg2, sqlite3_int64 *arg3, sqlite3_int64 *arg4, int arg5),
                 (arg1, arg2, arg3, arg4, arg5), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_declare_vtab, (sqlite3 * arg1, const char *zSQL), (arg1, zSQL), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_deserialize,
                 (sqlite3 * db, const char *zSchema, unsigned char *pData, sqlite3_int64 szDb, sqlite3_int64 szBuf,
                  unsigned mFlags),
                 (db, zSchema, pData, szDb, szBuf, mFlags), (_, _, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_drop_modules, (sqlite3 * db, const char **azKeep), (db, azKeep), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_enable_load_extension, (sqlite3 * db, int onoff), (db, onoff), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_enable_shared_cache, (int arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_errcode, (sqlite3 * db), (db), (_))
MOCK_SQLITE3_RET(const char *, sqlite3_errmsg, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(const void *, sqlite3_errmsg16, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_error_offset, (sqlite3 * db), (db), (_))
MOCK_SQLITE3_RET(const char *, sqlite3_errstr, (int arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_exec,
                 (sqlite3 * arg1, const char *sql, int (*callback)(void *, int, char **, char **), void *arg4,
                  char **errmsg),
                 (arg1, sql, callback, arg4, errmsg), (_, _, _, _, _))
MOCK_SQLITE3_RET(char *, sqlite3_expanded_sql, (sqlite3_stmt * pStmt), (pStmt), (_))
MOCK_SQLITE3_RET(int, sqlite3_expired, (sqlite3_stmt * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_extended_errcode, (sqlite3 * db), (db), (_))
MOCK_SQLITE3_RET(int, sqlite3_extended_result_codes, (sqlite3 * arg1, int onoff), (arg1, onoff), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_file_control, (sqlite3 * arg1, const char *zDbName, int op, void *arg4),
                 (arg1, zDbName, op, arg4), (_, _, _, _))
MOCK_SQLITE3_RET(const char *, sqlite3_filename_database, (sqlite3_filename arg1), (arg1), (_))
MOCK_SQLITE3_RET(const char *, sqlite3_filename_journal, (sqlite3_filename arg1), (arg1), (_))
MOCK_SQLITE3_RET(const char *, sqlite3_filename_wal, (sqlite3_filename arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_finalize, (sqlite3_stmt * pStmt), (pStmt), (_))
MOCK_SQLITE3_VOID(void, sqlite3_free, (void *arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_free_filename, (sqlite3_filename arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_free_table, (char **result), (result), (_))
MOCK_SQLITE3_RET(int, sqlite3_get_autocommit, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(void *, sqlite3_get_auxdata, (sqlite3_context * arg1, int N), (arg1, N), (_, _))
MOCK_SQLITE3_RET(void *, sqlite3_get_clientdata, (sqlite3 * arg1, const char *arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_get_table,
                 (sqlite3 * db, const char *zSql, char ***pazResult, int *pnRow, int *pnColumn, char **pzErrmsg),
                 (db, zSql, pazResult, pnRow, pnColumn, pzErrmsg), (_, _, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_global_recover, (), (), ())
MOCK_SQLITE3_RET(sqlite3_int64, sqlite3_hard_heap_limit64, (sqlite3_int64 N), (N), (_))
MOCK_SQLITE3_RET(int, sqlite3_initialize, (), (), ())
MOCK_SQLITE3_VOID(void, sqlite3_interrupt, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_is_interrupted, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_keyword_check, (const char *arg1, int arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_keyword_count, (), (), ())
MOCK_SQLITE3_RET(int, sqlite3_keyword_name, (int arg1, const char **arg2, int *arg3), (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_RET(sqlite3_int64, sqlite3_last_insert_rowid, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(const char *, sqlite3_libversion, (), (), ())
MOCK_SQLITE3_RET(int, sqlite3_libversion_number, (), (), ())
MOCK_SQLITE3_RET(int, sqlite3_limit, (sqlite3 * arg1, int id, int newVal), (arg1, id, newVal), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_load_extension, (sqlite3 * db, const char *zFile, const char *zProc, char **pzErrMsg),
                 (db, zFile, zProc, pzErrMsg), (_, _, _, _))
MOCK_SQLITE3_RET(void *, sqlite3_malloc, (int arg1), (arg1), (_))
MOCK_SQLITE3_RET(void *, sqlite3_malloc64, (sqlite3_uint64 arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_memory_alarm, (void (*arg1)(void *, sqlite3_int64, int), void *arg2, sqlite3_int64 arg3),
                 (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_RET(sqlite3_int64, sqlite3_memory_highwater, (int resetFlag), (resetFlag), (_))
MOCK_SQLITE3_RET(sqlite3_int64, sqlite3_memory_used, (), (), ())
MOCK_SQLITE3_RET(sqlite3_uint64, sqlite3_msize, (void *arg1), (arg1), (_))
MOCK_SQLITE3_RET(sqlite3_mutex *, sqlite3_mutex_alloc, (int arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_mutex_enter, (sqlite3_mutex * arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_mutex_free, (sqlite3_mutex * arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_mutex_leave, (sqlite3_mutex * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_mutex_try, (sqlite3_mutex * arg1), (arg1), (_))
MOCK_SQLITE3_RET(sqlite3_stmt *, sqlite3_next_stmt, (sqlite3 * pDb, sqlite3_stmt *pStmt), (pDb, pStmt), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_open, (const char *filename, sqlite3 **ppDb), (filename, ppDb), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_open16, (const void *filename, sqlite3 **ppDb), (filename, ppDb), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_open_v2, (const char *filename, sqlite3 **ppDb, int flags, const char *zVfs),
                 (filename, ppDb, flags, zVfs), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_os_end, (), (), ())
MOCK_SQLITE3_RET(int, sqlite3_os_init, (), (), ())
MOCK_SQLITE3_RET(int, sqlite3_overload_function, (sqlite3 * arg1, const char *zFuncName, int nArg),
                 (arg1, zFuncName, nArg), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_prepare,
                 (sqlite3 * db, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail),
                 (db, zSql, nByte, ppStmt, pzTail), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_prepare16,
                 (sqlite3 * db, const void *zSql, int nByte, sqlite3_stmt **ppStmt, const void **pzTail),
                 (db, zSql, nByte, ppStmt, pzTail), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_prepare16_v2,
                 (sqlite3 * db, const void *zSql, int nByte, sqlite3_stmt **ppStmt, const void **pzTail),
                 (db, zSql, nByte, ppStmt, pzTail), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_prepare16_v3,
                 (sqlite3 * db, const void *zSql, int nByte, unsigned int prepFlags, sqlite3_stmt **ppStmt,
                  const void **pzTail),
                 (db, zSql, nByte, prepFlags, ppStmt, pzTail), (_, _, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_prepare_v2,
                 (sqlite3 * db, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail),
                 (db, zSql, nByte, ppStmt, pzTail), (_, _, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_prepare_v3,
                 (sqlite3 * db, const char *zSql, int nByte, unsigned int prepFlags, sqlite3_stmt **ppStmt,
                  const char **pzTail),
                 (db, zSql, nByte, prepFlags, ppStmt, pzTail), (_, _, _, _, _, _))
MOCK_SQLITE3_RET(void *, sqlite3_profile,
                 (sqlite3 * arg1, void (*xProfile)(void *, const char *, sqlite3_uint64), void *arg3),
                 (arg1, xProfile, arg3), (_, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_progress_handler, (sqlite3 * arg1, int arg2, int (*arg3)(void *), void *arg4),
                  (arg1, arg2, arg3, arg4), (_, _, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_randomness, (int N, void *P), (N, P), (_, _))
MOCK_SQLITE3_RET(void *, sqlite3_realloc, (void *arg1, int arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(void *, sqlite3_realloc64, (void *arg1, sqlite3_uint64 arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_release_memory, (int arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_reset, (sqlite3_stmt * pStmt), (pStmt), (_))
MOCK_SQLITE3_VOID(void, sqlite3_reset_auto_extension, (), (), ())
MOCK_SQLITE3_VOID(void, sqlite3_result_blob, (sqlite3_context * arg1, const void *arg2, int arg3, void (*arg4)(void *)),
                  (arg1, arg2, arg3, arg4), (_, _, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_blob64,
                  (sqlite3_context * arg1, const void *arg2, sqlite3_uint64 arg3, void (*arg4)(void *)),
                  (arg1, arg2, arg3, arg4), (_, _, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_double, (sqlite3_context * arg1, double arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_error, (sqlite3_context * arg1, const char *arg2, int arg3), (arg1, arg2, arg3),
                  (_, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_error16, (sqlite3_context * arg1, const void *arg2, int arg3),
                  (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_error_code, (sqlite3_context * arg1, int arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_error_nomem, (sqlite3_context * arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_result_error_toobig, (sqlite3_context * arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_result_int, (sqlite3_context * arg1, int arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_int64, (sqlite3_context * arg1, sqlite3_int64 arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_null, (sqlite3_context * arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_result_pointer,
                  (sqlite3_context * arg1, void *arg2, const char *arg3, void (*arg4)(void *)),
                  (arg1, arg2, arg3, arg4), (_, _, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_subtype, (sqlite3_context * arg1, unsigned int arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_text, (sqlite3_context * arg1, const char *arg2, int arg3, void (*arg4)(void *)),
                  (arg1, arg2, arg3, arg4), (_, _, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_text16,
                  (sqlite3_context * arg1, const void *arg2, int arg3, void (*arg4)(void *)), (arg1, arg2, arg3, arg4),
                  (_, _, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_text16be,
                  (sqlite3_context * arg1, const void *arg2, int arg3, void (*arg4)(void *)), (arg1, arg2, arg3, arg4),
                  (_, _, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_text16le,
                  (sqlite3_context * arg1, const void *arg2, int arg3, void (*arg4)(void *)), (arg1, arg2, arg3, arg4),
                  (_, _, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_text64,
                  (sqlite3_context * arg1, const char *z, sqlite3_uint64 n, void (*arg4)(void *),
                   unsigned char encoding),
                  (arg1, z, n, arg4, encoding), (_, _, _, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_value, (sqlite3_context * arg1, sqlite3_value *arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_VOID(void, sqlite3_result_zeroblob, (sqlite3_context * arg1, int n), (arg1, n), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_result_zeroblob64, (sqlite3_context * arg1, sqlite3_uint64 n), (arg1, n), (_, _))
MOCK_SQLITE3_RET(void *, sqlite3_rollback_hook, (sqlite3 * arg1, void (*arg2)(void *), void *arg3), (arg1, arg2, arg3),
                 (_, _, _))
MOCK_SQLITE3_RET(unsigned char *, sqlite3_serialize,
                 (sqlite3 * db, const char *zSchema, sqlite3_int64 *piSize, unsigned int mFlags),
                 (db, zSchema, piSize, mFlags), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_set_authorizer,
                 (sqlite3 * arg1, int (*xAuth)(void *, int, const char *, const char *, const char *, const char *),
                  void *pUserData),
                 (arg1, xAuth, pUserData), (_, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_set_auxdata, (sqlite3_context * arg1, int N, void *arg3, void (*arg4)(void *)),
                  (arg1, N, arg3, arg4), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_set_clientdata, (sqlite3 * arg1, const char *arg2, void *arg3, void (*arg4)(void *)),
                 (arg1, arg2, arg3, arg4), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_set_errmsg, (sqlite3 * db, int errcode, const char *zErrMsg), (db, errcode, zErrMsg),
                 (_, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_set_last_insert_rowid, (sqlite3 * arg1, sqlite3_int64 arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_setlk_timeout, (sqlite3 * arg1, int ms, int flags), (arg1, ms, flags), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_shutdown, (), (), ())
MOCK_SQLITE3_RET(int, sqlite3_sleep, (int arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_soft_heap_limit, (int N), (N), (_))
MOCK_SQLITE3_RET(sqlite3_int64, sqlite3_soft_heap_limit64, (sqlite3_int64 N), (N), (_))
MOCK_SQLITE3_RET(const char *, sqlite3_sourceid, (), (), ())
MOCK_SQLITE3_RET(const char *, sqlite3_sql, (sqlite3_stmt * pStmt), (pStmt), (_))
MOCK_SQLITE3_RET(int, sqlite3_status, (int op, int *pCurrent, int *pHighwater, int resetFlag),
                 (op, pCurrent, pHighwater, resetFlag), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_status64, (int op, sqlite3_int64 *pCurrent, sqlite3_int64 *pHighwater, int resetFlag),
                 (op, pCurrent, pHighwater, resetFlag), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_step, (sqlite3_stmt * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_stmt_busy, (sqlite3_stmt * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_stmt_explain, (sqlite3_stmt * pStmt, int eMode), (pStmt, eMode), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_stmt_isexplain, (sqlite3_stmt * pStmt), (pStmt), (_))
MOCK_SQLITE3_RET(int, sqlite3_stmt_readonly, (sqlite3_stmt * pStmt), (pStmt), (_))
MOCK_SQLITE3_RET(int, sqlite3_stmt_status, (sqlite3_stmt * arg1, int op, int resetFlg), (arg1, op, resetFlg), (_, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_str_append, (sqlite3_str * arg1, const char *zIn, int N), (arg1, zIn, N), (_, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_str_appendall, (sqlite3_str * arg1, const char *zIn), (arg1, zIn), (_, _))
MOCK_SQLITE3_VOID(void, sqlite3_str_appendchar, (sqlite3_str * arg1, int N, char C), (arg1, N, C), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_str_errcode, (sqlite3_str * arg1), (arg1), (_))
MOCK_SQLITE3_RET(char *, sqlite3_str_finish, (sqlite3_str * arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_str_free, (sqlite3_str * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_str_length, (sqlite3_str * arg1), (arg1), (_))
MOCK_SQLITE3_RET(sqlite3_str *, sqlite3_str_new, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_str_reset, (sqlite3_str * arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_str_truncate, (sqlite3_str * arg1, int N), (arg1, N), (_, _))
MOCK_SQLITE3_RET(char *, sqlite3_str_value, (sqlite3_str * arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_str_vappendf, (sqlite3_str * arg1, const char *zFormat, va_list arg3),
                  (arg1, zFormat, arg3), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_strglob, (const char *zGlob, const char *zStr), (zGlob, zStr), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_stricmp, (const char *arg1, const char *arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_strlike, (const char *zGlob, const char *zStr, unsigned int cEsc), (zGlob, zStr, cEsc),
                 (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_strnicmp, (const char *arg1, const char *arg2, int arg3), (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_system_errno, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_table_column_metadata,
                 (sqlite3 * db, const char *zDbName, const char *zTableName, const char *zColumnName,
                  char const **pzDataType, char const **pzCollSeq, int *pNotNull, int *pPrimaryKey, int *pAutoinc),
                 (db, zDbName, zTableName, zColumnName, pzDataType, pzCollSeq, pNotNull, pPrimaryKey, pAutoinc),
                 (_, _, _, _, _, _, _, _, _))
MOCK_SQLITE3_VOID(void, sqlite3_thread_cleanup, (), (), ())
MOCK_SQLITE3_RET(int, sqlite3_threadsafe, (), (), ())
MOCK_SQLITE3_RET(int, sqlite3_total_changes, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(sqlite3_int64, sqlite3_total_changes64, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(void *, sqlite3_trace, (sqlite3 * arg1, void (*xTrace)(void *, const char *), void *arg3),
                 (arg1, xTrace, arg3), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_trace_v2,
                 (sqlite3 * arg1, unsigned uMask, int (*xCallback)(unsigned, void *, void *, void *), void *pCtx),
                 (arg1, uMask, xCallback, pCtx), (_, _, _, _))
MOCK_SQLITE3_RET(int, sqlite3_transfer_bindings, (sqlite3_stmt * arg1, sqlite3_stmt *arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_txn_state, (sqlite3 * arg1, const char *zSchema), (arg1, zSchema), (_, _))
MOCK_SQLITE3_RET(void *, sqlite3_update_hook,
                 (sqlite3 * arg1, void (*arg2)(void *, int, char const *, char const *, sqlite3_int64), void *arg3),
                 (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_uri_boolean, (sqlite3_filename z, const char *zParam, int bDefault),
                 (z, zParam, bDefault), (_, _, _))
MOCK_SQLITE3_RET(sqlite3_int64, sqlite3_uri_int64, (sqlite3_filename arg1, const char *arg2, sqlite3_int64 arg3),
                 (arg1, arg2, arg3), (_, _, _))
MOCK_SQLITE3_RET(const char *, sqlite3_uri_key, (sqlite3_filename z, int N), (z, N), (_, _))
MOCK_SQLITE3_RET(const char *, sqlite3_uri_parameter, (sqlite3_filename z, const char *zParam), (z, zParam), (_, _))
MOCK_SQLITE3_RET(void *, sqlite3_user_data, (sqlite3_context * arg1), (arg1), (_))
MOCK_SQLITE3_RET(const void *, sqlite3_value_blob, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_value_bytes, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_value_bytes16, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(double, sqlite3_value_double, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(sqlite3_value *, sqlite3_value_dup, (const sqlite3_value *arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_value_encoding, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_VOID(void, sqlite3_value_free, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_value_frombind, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_value_int, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(sqlite3_int64, sqlite3_value_int64, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_value_nochange, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_value_numeric_type, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(void *, sqlite3_value_pointer, (sqlite3_value * arg1, const char *arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(unsigned int, sqlite3_value_subtype, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(const unsigned char *, sqlite3_value_text, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(const void *, sqlite3_value_text16, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(const void *, sqlite3_value_text16be, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(const void *, sqlite3_value_text16le, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_value_type, (sqlite3_value * arg1), (arg1), (_))
MOCK_SQLITE3_RET(sqlite3_vfs *, sqlite3_vfs_find, (const char *zVfsName), (zVfsName), (_))
MOCK_SQLITE3_RET(int, sqlite3_vfs_register, (sqlite3_vfs * arg1, int makeDflt), (arg1, makeDflt), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_vfs_unregister, (sqlite3_vfs * arg1), (arg1), (_))
MOCK_SQLITE3_RET(char *, sqlite3_vmprintf, (const char *arg1, va_list arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(char *, sqlite3_vsnprintf, (int arg1, char *arg2, const char *arg3, va_list arg4),
                 (arg1, arg2, arg3, arg4), (_, _, _, _))
MOCK_SQLITE3_RET(const char *, sqlite3_vtab_collation, (sqlite3_index_info * arg1, int arg2), (arg1, arg2), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_vtab_distinct, (sqlite3_index_info * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_vtab_in, (sqlite3_index_info * arg1, int iCons, int bHandle), (arg1, iCons, bHandle),
                 (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_vtab_in_first, (sqlite3_value * pVal, sqlite3_value **ppOut), (pVal, ppOut), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_vtab_in_next, (sqlite3_value * pVal, sqlite3_value **ppOut), (pVal, ppOut), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_vtab_nochange, (sqlite3_context * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_vtab_on_conflict, (sqlite3 * arg1), (arg1), (_))
MOCK_SQLITE3_RET(int, sqlite3_vtab_rhs_value, (sqlite3_index_info * arg1, int arg2, sqlite3_value **ppVal),
                 (arg1, arg2, ppVal), (_, _, _))
MOCK_SQLITE3_RET(int, sqlite3_wal_autocheckpoint, (sqlite3 * db, int N), (db, N), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_wal_checkpoint, (sqlite3 * db, const char *zDb), (db, zDb), (_, _))
MOCK_SQLITE3_RET(int, sqlite3_wal_checkpoint_v2, (sqlite3 * db, const char *zDb, int eMode, int *pnLog, int *pnCkpt),
                 (db, zDb, eMode, pnLog, pnCkpt), (_, _, _, _, _))
MOCK_SQLITE3_RET(void *, sqlite3_wal_hook,
                 (sqlite3 * arg1, int (*arg2)(void *, sqlite3 *, const char *, int), void *arg3), (arg1, arg2, arg3),
                 (_, _, _))
