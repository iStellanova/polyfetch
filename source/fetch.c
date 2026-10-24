#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sqlite3.h>

void os(void) {
  struct utsname info;
  if (uname(&info) != 0) {
    perror("uname");
    return;
  }
  printf("System: %s %s\n", info.sysname, info.release);
}

void pretty(void) {
  FILE *release = fopen("/etc/os-release", "r");
  char line[256];
  if (release != NULL) {
    while (fgets(line, sizeof line, release) != NULL) {
      if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
	char *prim = line + 13;
	char *end = strchr(prim, '"');
	if (end) *end = '\0';
	printf("OS: %s\n", prim);
	break;
      }
    }
    fclose(release);
  }
}

void nixClosureSize(void) {
  char root[PATH_MAX];
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  const char *sql =
    "WITH RECURSIVE closure(id) AS ("
    " SELECT id FROM ValidPaths WHERE path = ?1"
    " UNION"
    " SELECT Refs.reference FROM Refs JOIN closure"
    " ON Refs.referrer = closure.id"
    ")"
    " SELECT count(*), sum(narSize)"
    " FROM closure JOIN ValidPaths USING (id)";
  if (realpath("/run/current-system", root) &&
      sqlite3_open_v2("/nix/var/nix/db/db.sqlite", &db,
      SQLITE_OPEN_READONLY, NULL) == SQLITE_OK &&
      sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK &&
      sqlite3_bind_text(stmt, 1, root, -1, SQLITE_TRANSIENT) == SQLITE_OK &&
      sqlite3_step(stmt) == SQLITE_ROW) {
    printf("System Closure: %lld paths, %.2f GB\n",
	   (long long)sqlite3_column_int64(stmt, 0),
	   (long long)sqlite3_column_int64(stmt, 1) / 1e9);
  }
  sqlite3_finalize(stmt);
  if (db) sqlite3_close(db);
}
