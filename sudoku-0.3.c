//
// Implementation of solve_board() for the Sudoku Challange using Sudoku Solver 0.3 by Robert Sandiland
//
// GPL contaminated by design 
//
// Read main.c for instructions how to make this work
//
// Visit http://bildrulle.nu/tech/sudoku_challange.html
//
// Joakim Larsson, joakim@bildrulle.nu, 2006-02-24
//
// To generate shared library:
// g++ -I sudoku-0.3/include -fPIC -c sudoku-0.3/src/game.cc sudoku-0.3/src/block3x3.cc sudoku-0.3.c
// g++ -shared -Wl,-soname,solver.so.0.3 -Wl,--version-script=tss.version -o solver.so.0.3.0 *.o
// 

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

extern "C" char *solver_name(){ return (char *) "Sudoku Solver 0.3"; }

extern "C" void solve_board(char *board)
{
    int i, j;

    int cost, round;
    game mygame;
    int bx, by, x, y, val, rx, ry;

    mygame.seed();
    for ( i = 0; i < 9; ++i) {
        for ( j = 0; j < 9; ++j) {
            if (board[i * 9 + j] != '-')
            {
  	        rx = j;
	        ry = i;
	        val = board[i * 9 + j] - '0';
	        bx = rx / BLOCKSIZE;
	        by = ry / BLOCKSIZE;
	        x = rx % BLOCKSIZE;
	        y = ry % BLOCKSIZE;

                mygame.setFixedVal( bx, by, x, y, val );
            }
        }
    }

    round = 0;
    cost = mygame.calculate_cost();
    do 
    {
	cost = mygame.rotate( cost );
	round+=1;
	if ( ( round % 100000 ) == 0 ) 
	{
            break;
	}
    } while ( cost );

    mygame.dump_board(board);
}
