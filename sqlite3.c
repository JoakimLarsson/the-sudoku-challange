/*
 * This file contains sqlite storage of results for the sudoku challange
 */


#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int load_result(int bid, char *hardware, char *solver)
{
}

int store_result(int bid, char *hardware, char *solver, long long diff)
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql = (char *) "INSERT INTO LAPS (ID, BOARD, CPU, SOLVER, TIME) VALUES (%d, %d, \'%s\', \'%s\', %llu);";
  char qry[1024];
  int id = 0;

  rc = sqlite3_open("tss.db", &db);
  qry[0] = '\0';

  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  }else{
    fprintf(stderr, "Opened database successfully\n");
  }

  snprintf(qry, sizeof(qry), sql, id, bid, hardware, solver, diff);
  printf("%s\n", qry);
  rc = sqlite3_exec(db, qry, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }else{
    fprintf(stdout, "Records created successfully\n");
  }
  sqlite3_close(db);

  return 0;
}

/*
 * Store board in database unless already stored
 * return values: 0 = OK, -1 = failed
 */
int store_board(char *board)
{
  return -1;
}

/*
 * Loads board from database  or fail if none is available.
 * arguments    : bid = board ID, sz = size of the board buffer 
 * return values: 0 = OK, -1 = failed
 *   If OK the board with id that equals or is closest larger than 
 *   bid is returned.
 */
int load_board(int bid, char *board, int sz)
{
  return -1;
}


