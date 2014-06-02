/*
 * This file contains sqlite storage of results for the sudoku challange
 */


#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *ret, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    if (strncmp(azColName[i], "ID", 3) == 0){
      *((int *)ret) = atoi(argv[i]);
    } 
  }
  return 0;
}

int load_result(int bid, char *hardware, char *solver)
{
  return 0;
}

/*
 * Store result in a database
 * Arguments:
 *  bid - Board ID, must exist in the BOARDS table
 *  hardware - identify the hardware used, timings from different hardwares
 *             OR different boards doesn't make much sense to compare here.
 *  solver - identify the solver routines uniquely
 *  diff - integer value describing one meassure execution time for the routine 
 *
 */
int store_result(int bid, char *hardware, char *solver, long long diff, int rest, char *result)
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql = (char *) "INSERT INTO LAPS (ID, BOARD, CPU, SOLVER, TIME, REST, RESULT) VALUES (NULL, %d, \'%s\', \'%s\', %llu, %d, \'%s\');";
  char qry[1024];
  int id = 0;
  int ret;

  rc = sqlite3_open("tss.db", &db);
  qry[0] = '\0';

  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  snprintf(qry, sizeof(qry), sql, bid, hardware, solver, diff, rest, result);
  //  printf("%s\n", qry);
  while(1){
    rc = sqlite3_exec(db, qry, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %d - %s\n", rc, zErrMsg);
      sqlite3_free(zErrMsg);
      if (rc == SQLITE_ERROR){
	char *sql2 = (char *) "CREATE TABLE LAPS(ID INTEGER PRIMARY KEY ASC, BOARD int, CPU text, SOLVER text, TIME int8, REST int, RESULT text);";
	rc = sqlite3_exec(db, sql2, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
	  fprintf(stderr, "SQL error: %d - %s\n", rc, zErrMsg);
	  sqlite3_free(zErrMsg);
	  ret = -1; // Error, can't create table 
	  break;
	}
	continue; // Table created, try again
      }
      else
	ret = -1;
	break; // Another error, result NOT stored
    }else{
      ret = 0; // Result stored
      break;
    }
  }
  sqlite3_close(db);

  return ret;
}

/*
 * Store board in database unless already stored
 * return values: 0 = OK, -1 = failed
 */
int store_board(char *board, char *comment)
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  int ret;
  char *sql = (char *) "INSERT INTO BOARDS (ID, BOARD, COMMENT) VALUES (NULL, \'%s\', \'%s\');";
  char qry[1024];
  int id = 0;

  rc = sqlite3_open("tss.db", &db);
  qry[0] = '\0';
  ret = 0;

  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return -1;
  }else{
    //    fprintf(stderr, "Opened database successfully\n");
  }

  snprintf(qry, sizeof(qry), sql, board, comment);
  //  printf("%s\n", qry);
  while(1){
    rc = sqlite3_exec(db, qry, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      //      fprintf(stderr, "SQL error: %d - %s\n", rc, zErrMsg);
      sqlite3_free(zErrMsg);
      if (rc == SQLITE_ERROR){
	//	fprintf(stderr, "Creating table\n");
	char *sql2 = (char *) "CREATE TABLE BOARDS(ID INTEGER PRIMARY KEY ASC, BOARD text UNIQUE, COMMENT text);";
	rc = sqlite3_exec(db, sql2, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
	  fprintf(stderr, "SQL error: %d - %s\n", rc, zErrMsg);
	  sqlite3_free(zErrMsg);
	  ret = -1;
	  break;
	}
	continue;
      }
      else if (rc == SQLITE_CONSTRAINT){
	char *sql3 = (char *) "SELECT ID FROM BOARDS WHERE BOARD LIKE '%s';";
	snprintf(qry, sizeof(qry), sql3, board);
	//	printf("Q: %s\n", qry);
	rc = sqlite3_exec(db, qry, callback, &ret, &zErrMsg);
	//	printf("Selected ID = %d, (%d)\n", ret, rc);
	break;;
      }
      else{
	ret = sqlite3_last_insert_rowid(db);
	break;
      }
    }
    else{
      //      fprintf(stdout, "Board created successfully\n");
      ret = sqlite3_last_insert_rowid(db);
      break;
    }
  }
  sqlite3_close(db);

  return ret;
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


