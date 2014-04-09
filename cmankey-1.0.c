//
// Implementation of solve_board() for the Sudoku Challange using A C-based sudoku solver by the Code Mankeys 
//
// https://github.com/austinglaser/sudoku
//
// Read main.c for instructions how to make this work
//
//
// Joakim Larsson, joakim@bildrulle.nu, 2014-03-07
//
// g++ -o d -DDEBUG -I sudoku sudoku.c sudoku/main.c sudoku/sudoku.c sudoku/fifo.c; ./d
// g++ -o d -DBENCH -I sudoku main.c sudoku.c sudoku/main.c sudoku/sudoku.c sudoku/fifo.c; ./d
// g++ -o d -DBENCH -I sudoku main.c sudoku.c sudoku/main.c sudoku/sudoku.c sudoku/lifo.c; ./d
// 

#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

sudoku solve(queue q, sudoku s, int * guesses);
int checkCell(short int cell, int * v);
struct _sudoku {
	short int cell[0];
};

static void dS(void * s) { deleteSudoku((sudoku) s); }
extern int verbose, pretty, manual; //global variables
#define CELL(s, r, c)	s->cell[9*(r-1) + (c-1)]

// Stringification macros from http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
#define XSTR(x) STR(x)
#define STR(x)  #x
#define MYNAME  XSTR(NAME)

extern "C" char *solver_name(){ return (char *) MYNAME; }
extern "C" void solve_board(char *board)
{
  int i, j;
  int cost, round;
  int bx, by, x, y, val, rx, ry;
  
  sudoku s;
  
  s = newSudoku();
  
  for ( i = 0; i < 9; ++i) {
    for ( j = 0; j < 9; ++j) {
      if (board[i * 9 + j] != '-')
	{
	  rx = j + 1;
	  ry = i + 1;
	  val = board[i * 9 + j] - '0';
	  setCell(s, ry, rx, val);
	}
    }
  }

  //  printSudoku(s, 1);
  
  queue q = newQueue(-1, dS); //create a new queue
  int n,r,c,v;
  int guesses;
  sudoku sol = solve(q, s, &guesses);

  //  printSudoku(sol, 1);
  
  // Dump board to generic format
  for ( i = 0; i < 9; ++i) {
    for ( j = 0; j < 9; ++j) {
      n = checkCell(CELL(sol, j + 1, i + 1), &v);
      board[i * 9 + j] = ((n == 1) ? v + '0' : '-');
    }
  }

  deleteSudoku(sol);
  deleteQueue(q);
}

