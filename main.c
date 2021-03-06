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

#include <ctype.h>
#include <dlfcn.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <sys/utsname.h>

#include "hrtimer.h"

#define COMMENT_LENGTH 25

extern int store_board(char *board, char *comment);
extern int store_result(int bid, char *hardware, char *solver, long long diff, int rest, char *result);
extern "C" char *solver_name();
extern "C" void solve_board(char *cp);

void print_board(char *board)
{
    int a, b;
    
    printf("\n Brd ");
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


main(int argc, char **argv)
{
  uint64_t tt1, tt2;
  char board[1024];
  char board2[1024];
  FILE *fp;
  int i, left, left2; 
  int bid;
  long long diff;
  char *cmnt, *tmp;
  void *lib_handle;
  void (*do_solve)(char *);
  char *(*get_name)(void);
  char * error;

  struct utsname sys;
  char name[80];
  char fnam[80];

  uname(&sys);
  snprintf(name, sizeof(name), "%s %s", sys.sysname, sys.machine);
  snprintf(fnam, sizeof(fnam), "%s", argc >= 2 ? argv[2] : "boards.txt");
  board[0] = '\0';
  board2[0] = '\0';
  left2 = -1;

  fp = fopen(fnam, "r");

  while (fgets(board, sizeof(board) - 1, fp) != NULL){

    if (memchr(board, '.', 81) == NULL && memchr(board, '-', 81) == NULL )
    {
      i = 0;
      if (left2 == 0)
	printf(" Cmp %s and %s and got a %s\n", board, board2, (i = memcmp(board, board2, 81)) == 0 ? "match!!" : "fail!!");
      if (i == 0)
	continue;
      else
	exit(-1);
    }

    board[81] = '\0';
#ifdef BENCH      
    board[81 + COMMENT_LENGTH + 1] = '\0';
    printf(" Run %s ", board);
    cmnt = &board[82];
#else
    printf("\n\n-----------------");
    printf("\n%s", &board[81]);
    printf("=================");
    print_board(board);
#endif
    for (i = 0; i < 81; i++) board[i] = isdigit(board[i]) ? board[i] : '-'; /* Convert from other delimiters to dash */

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
    bid = store_board(board, cmnt);
    left = count_left(board);    

    lib_handle = dlopen(argv[1], RTLD_LAZY);
    if (!lib_handle){
      fprintf(stderr, "%s\n", dlerror());
      exit(1);
    }

    do_solve = (void (*)(char*)) dlsym(lib_handle, "solve_board");
    get_name = (char* (*)())     dlsym(lib_handle, "solver_name");

    if ((error = dlerror()) != NULL)  
    {
	fprintf(stderr, "%s\n", error);
	exit(1);
    }

    tt1 = get_hrtimer();
    (*do_solve)(board);
    tt2 = get_hrtimer();
    
#ifndef BENCH
    printf("-----------------");
    print_board(board);
    printf("-----------------\n");
#endif
    
    left2 = count_left(board);

    board[81] = 0;
    printf("Res %s ", board);
    
    diff = tt2 - tt1;
    
    printf("Time:%16llu ", diff);
    printf("Left:%3u ", left2);
    printf("Solved:%3u\n", left - left2);

    store_result(bid, name, (*get_name)(), diff, left2, board);

    strncpy(board2, board, sizeof(board)); // Copy in case next row is the correct solution
    board2[sizeof(board2) - 1] = '\0';
  }
  
  fclose(fp);
  
  exit(0);
}
