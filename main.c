//
// The Sudoku Challange main.c program
//
// This is free software copyrighted by me and may be used freely under BSD kindof license
//
// Some example solvers below are GPL:ed so this file will be contaminated to GPL as well if 
// distributed together as source or binaries. That's ok with me. 
//
// See README.TXT for build instructions.
//
// Joakim Larsson, joakim@bildrulle.nu, 2006-02-24
//
//

#include <sqlite3.h>

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#define COMMENT_LENGTH 25

extern char *solver_name();
extern void solve_board(char *cp);

void print_board(char *board)
{
    int a, b;
    
    printf("\n");
    for (a = 0; a < 9; a++)
    {
        for (b = 0; b < 9; b++)
        {
            printf("%c ",board[a * 9 + b]);
        }    
        printf("\n");
    }
}

int count_left(char *board)
{
    int i, left;

    left = 0;

    for (i = 0; i < 81; i++)
    {
        if (board[i] == '-')
        {
            left++;            
        }
    }

    return left;
}

/*
 * This function was ripped from: 
 * http://stackoverflow.com/questions/7935518/is-clock-gettime-adequate-for-submicrosecond-timing
 */
__inline__ uint64_t rdtsc(void) {
  uint32_t lo, hi;
  __asm__ __volatile__ (      // serialize
  "xorl %%eax,%%eax \n        cpuid"
  ::: "%rax", "%rbx", "%rcx", "%rdx");
  /* We cannot use "=A", since this would use %rax on x86_64 and return only the lower 32bits of the TSC */
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return (uint64_t)hi << 32 | lo;
}

#if SQLITE
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}
#endif

main()
{
  uint64_t tt1, tt2;
  char board[1024];
  FILE *fp;
  int i, left, left2; 
  long long diff;
  char *cmnt, *tmp;
  
#ifdef SQLITE
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql = "INSERT INTO LAPS (ID, BOARD, CPU, SOLVER, TIME) VALUES (%d, %d, \'%s\', \'%s\', %llu);";
  char qry[1024];
  int id = 0;
  int bid = 45;

  rc = sqlite3_open("tss.db", &db);
  qry[0] = '\0';

  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  }else{
    fprintf(stderr, "Opened database successfully\n");
  }
#endif

  fp = fopen("boards.txt", "r");

  while (fgets(board, 1024, fp) != NULL){
      
#ifdef BENCH
      
    board[81] = '\0';
    board[sizeof(board)] = '\0';
    printf("%s ", board);
#else
    printf("\n\n-----------------");
    printf("\n%s", &board[81]);
    printf("=================");
    print_board(board);
#endif
    
    cmnt = &board[82];
    if ((tmp = strstr(cmnt, "Time")) != NULL){
      *tmp = '\0';
    }
    else if ((tmp = strstr(cmnt, "\n")) != NULL){
      *tmp = '\0';
    }
    while (strlen(cmnt) < COMMENT_LENGTH){
      strcat(cmnt, " ");
    }
    cmnt[COMMENT_LENGTH] = '\0';
    left = count_left(board);
    
    tt1 = rdtsc();
    solve_board(board);
    tt2 = rdtsc();
    
#ifndef BENCH
    printf("-----------------");
    print_board(board);
    printf("-----------------\n");
#endif
    
    printf (" %s", cmnt);
    left2 = count_left(board);
    
    diff = tt2 - tt1;
    
#ifdef SQLITE
    snprintf(qry, sizeof(qry), sql, id, bid, "KVM", solver_name(), diff);
    printf("%s\n", qry);
    rc = sqlite3_exec(db, qry, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Records created successfully\n");
    }
#else
    printf("Time:%16llu ", diff);
    printf("Left:%3u ", left2);
    printf("Solved:%3u\n", left - left2);
#endif
  }
  
  fclose(fp);
  sqlite3_close(db);
  
  exit(0);
}
