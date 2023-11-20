// gcc lmdb-dump.c -o lmdb-dump -llmdb
#include <errno.h>
#include <lmdb.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
    return 1;

  fprintf(stderr, "Opening %s\n", argv[1]);

  MDB_env *env = NULL;
  MDB_txn *txn = NULL;
  MDB_dbi db;
  MDB_cursor *cursor = NULL;
  int rc;

  rc = mdb_env_create(&env);
  if (rc != MDB_SUCCESS)
    return 2;

  rc = mdb_env_open(env, argv[1], MDB_NOSUBDIR, 0644);
  if (rc != MDB_SUCCESS)
    return 3;

  rc = mdb_txn_begin(env, NULL, 0, &txn);
  if (rc != MDB_SUCCESS)
    return 4;

  rc = mdb_dbi_open(txn, NULL, MDB_CREATE, &db);
  if (rc != MDB_SUCCESS)
    return 5;

  rc = mdb_cursor_open(txn, db, &cursor);
  if (rc != MDB_SUCCESS)
    return 6;

  MDB_val key;
  MDB_val data;
  for (int count = 0;; count++)
  {
    rc = mdb_cursor_get(cursor, &key, &data, MDB_NEXT);
    if (rc != MDB_SUCCESS)
    {
      fprintf(stderr, "%d records\n", count);
      break;
    }

    int len = key.mv_size;
    printf("key: %.*s\n", len, (const char *) key.mv_data);
    printf("\tdata length: %zu\n", data.mv_size);
  }

  mdb_cursor_close(cursor);
  mdb_dbi_close(env, db);
  mdb_txn_abort(txn);
  mdb_env_close(env);

  return 0;
}