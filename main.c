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

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#include "hrtimer.h"

#define COMMENT_LENGTH 25

extern int store_result(int bid, char *hardware, char *solver, long long diff);
extern "C" char *solver_name();
extern "C" void solve_board(char *cp);

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


main()
{
  uint64_t tt1, tt2;
  char board[1024];
  FILE *fp;
  int i, left, left2; 
  long long diff;
  char *cmnt, *tmp;
  

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
    
    tt1 = get_hrtimer();
    solve_board(board);
    tt2 = get_hrtimer();
    
#ifndef BENCH
    printf("-----------------");
    print_board(board);
    printf("-----------------\n");
#endif
    
    printf (" %s", cmnt);
    left2 = count_left(board);
    
    diff = tt2 - tt1;
    
    printf("Time:%16llu ", diff);
    printf("Left:%3u ", left2);
    printf("Solved:%3u\n", left - left2);

    store_result(0, "KVM", solver_name(), diff);

  }
  
  fclose(fp);
  
  exit(0);
}
