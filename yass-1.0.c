//
// Implementation of solve_board() for the Sudoku Challange using Yass 1.0 by Binesh Bannerjee
//
// GPL contaminated by design 
//
// Read main.c for instructions how to make this work
//
// Visit http://bildrulle.nu/tech/sudoku_challange.html
//
// Joakim Larsson, joakim@bildrulle.nu, 2006-02-24
//
// g++ -DDEBUG -o s yass-1.0.c yass-1.0/src/solver.cc; ./s 
// 

#include <stdio.h>
#include "include/solver.h"

extern "C" char *solver_name(){ return (char *) "Yass 1.0"; }

extern "C" void solve_board(char *board)
{
    int i, j;
    int yboard[9][9];
    int yboard_solved[9][9];

    for ( i = 0; i < 9; ++i) {
        for ( j = 0; j < 9; ++j) {
            yboard[i][j] = (board[i * 9 + j] != '-') ? board[i * 9 + j] - '0' : 0;
        }
    }

    solve(yboard, yboard_solved);

    for ( i = 0; i < 9; ++i) {
        for ( j = 0; j < 9; ++j) {
            board[i * 9 + j] = (yboard_solved[i][j] != 0) ? yboard_solved[i][j] + '0' : '-';
        }
    }

}
