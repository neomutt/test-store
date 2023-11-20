// gcc lmdb-cull.c -o lmdb-cull -llmdb
#include <errno.h>
#include <lmdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
    return 1;

  int percent = atoi(argv[2]);
  if ((percent < 1) || (percent > 100))
    return 2;

  printf("Opening %s\n", argv[1]);
  printf("Culling %d%%\n", percent);

  srand(time(NULL));

  MDB_env *env = NULL;
  MDB_txn *txn = NULL;
  MDB_dbi db;
  MDB_cursor *cursor = NULL;
  int rc;

  rc = mdb_env_create(&env);
  if (rc != MDB_SUCCESS)
    return 3;

  rc = mdb_env_open(env, argv[1], MDB_NOSUBDIR, 0644);
  if (rc != MDB_SUCCESS)
    return 4;

  rc = mdb_txn_begin(env, NULL, 0, &txn);
  if (rc != MDB_SUCCESS)
    return 5;

  rc = mdb_dbi_open(txn, NULL, MDB_CREATE, &db);
  if (rc != MDB_SUCCESS)
    return 6;

  rc = mdb_cursor_open(txn, db, &cursor);
  if (rc != MDB_SUCCESS)
    return 7;

  MDB_val key;
  MDB_val data;
  int culled = 0;
  for (int count = 0;; count++)
  {
    rc = mdb_cursor_get(cursor, &key, &data, MDB_NEXT);
    if (rc != MDB_SUCCESS)
    {
      printf("%d records\n", count);
      printf("%d culled\n", culled);
      break;
    }

    if ((rand() % 100) < percent)
    {
      mdb_cursor_del(cursor, 0);
      culled++;
    }
  }

  mdb_cursor_close(cursor);
  mdb_dbi_close(env, db);
  mdb_txn_commit(txn);
  mdb_env_close(env);

  return 0;
}
