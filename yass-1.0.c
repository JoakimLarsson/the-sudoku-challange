//
// Example implementation of solve_board() for the Sudoku Challange using Yass 1.0 by Binesh Bannerjee
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

#if DEBUG | DEBUG1

// Test boards

    // char board[] = "123456789123456789123456789123456789123456789123456789123456789123456789123456789";
    // char board[] = "---------------------------------------------------------------------------------";
    //char board[] = "---3--67-5--4--8--628---3-----2-4-37---------41-7-5-----2---961--6--1--4-84--9---"; // Time 207
    //char board[] = "--71-6--24---9------128-63-2--315-6-93-47-----58---7-3----4752-6-5--19-------238-"; // Time: 25
    //char board[] = "-17--3-6---2-8--546---2---9--31---7--8-9---2346------11--37-8----4-651--9--------"; // Time: 29
    //char board[] = "3----87-------9--41---6---2--5-23-8--7---4-9--6---15--8---5---67--1-------98----5"; // Time 192 (2000 medium)
    //char board[] = "-2-9--83---7--4--6--5--1-9--3-2----4--6-7-5--1---89-2--4-3--1--8--7--6---72--6-4-"; // Time 29 (2001 medium)
    //char board[] = "-1---8-5--6--4-7-------2--9-3---1-8-7---6---2-5-97--4-8--3-------9-1--6--4-5---7-";   // Time 209 (2125 medium)
    char board[] = "-9--3-6------7-2----48----51--5---8---7---9---3---6--42----17----8-2------6-5--9-";   // Left 44  (2151 medium)
    //char board[] = "8--6----2-4--5--1----7----3-9---4--62-------87---1--5-3----9----1--8--9-4----2--5"; // Left 56 (insane)
    // char board[] = "-6---39--5--1-----8-------7-4-2--6--7-------8--3--9-1-2-------5-----4--3--87---2-"; // Left 43 (hard)



void print_board(char *board)
{
    int a, b;

    printf("\n\n");
    for (a = 0; a < 9; a++)
    {
        for (b = 0; b < 9; b++)
        {
            printf("%c ",board[a * 9 + b]);
        }    
        printf("\n");
    }
}
#endif

char *solver_name(){ return "Yass 1.0"; }

void solve_board(char *board)
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

#if DEBUG | DEBUG1

main()
{
    print_board(board);
    solve_board(board);
    print_board(board);

//    exit(0);
}

#endif
