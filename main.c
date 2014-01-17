//
// The Sudoku Challange main.c program
//
// This is free software copyrighted by me and may be used freely under BSD kindof license
//
// Some example solvers below are GPL:ed so this file will be contaminated to GPL as well if 
// distributed together as source or binaries. That's ok with me. 
//
// 
// Compile, link and run from emacs with:
//
//   M-x compile
//   gcc -o s main.c solve.c; ./s
//
// The solve.c should implement solve_board(char * board); function which takes
// a buffer with one line from the example board file boards.txt, for example:
//
// ---3--67-5--4--8--628---3-----2-4-37---------41-7-5-----2---961--6--1--4-84--9--- Medel
//
// The trailing characters may have some optional comments about the board
//
//   Example open source sudoku solvers
// ======================================
//   YASS 1.0 (GPL): get the source from 
// --------------------------------------
//       http://sourceforge.net/project/showfiles.php?group_id=145574&package_id=160138
//   tar xzvf yass-1.0.tar.gz
//   cd yass-1.0; ./configure; make; cd ..
//   g++ -I yass-1.0/ -DBENCH -o s yass-1.0.c yass-1.0/src/solver.cc main.c; ./s 
//   
//   Sudoku Solver 0.3 (GPL) get the source from
// --------------------------------------
//       http://robert.rsa3.com/sudoku.html
//   tar xzvf sudoku-0.3.tar.gz
//   Apply patch sudoku-0.3.patch to make it work in this context. (include/game.h and src/game.cc)
//   cd sudoku-0.3; ./configure; make; cd ..
//   g++ -I sudoku-0.3/include -DBENCH -o s main.c sudoku-0.3/src/game.cc sudoku-0.3/src/block3x3.cc sudoku-0.3.c; ./s  
//
//  So can you beat these implementations??!! Send your results to me and I will publish the results, at joakim@bildrulle.nu
//
// Compiling the program with the BENCH symbol defined:
//
//   M-x compile
//   gcc -DBENCH -o s main.c solve.c; ./s
//
// will generate output suitable for a boards.txt file and replace/prepend benchmark figures
// It will also preserve any comments not exceeding the COMMENT_LENGTH below.
//
// Visit http://bildrulle.nu/tech/sudoku_challange.html
//
//  HAVE FUN!
//
// Joakim Larsson, joakim@bildrulle.nu, 2006-02-24
//
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define COMMENT_LENGTH 25

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

main()
{
    struct timeval tv1, tv2;
    char board[1024];
    FILE *fp;
    int i, left, left2, diff;
    char *cmnt, *tmp;

    fp = fopen("boards.txt", "r");

    while (fgets(board, 1024, fp) != NULL)
    {

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
        if ((tmp = strstr(cmnt, "Time")) != NULL)
        {
            *tmp = '\0';
        }
        else if ((tmp = strstr(cmnt, "\n")) != NULL)
        {
            *tmp = '\0';
        }
        while (strlen(cmnt) < COMMENT_LENGTH)
        {
            strcat(cmnt, " ");
        }
        cmnt[COMMENT_LENGTH] = '\0';
        left = count_left(board);
        gettimeofday(&tv1, NULL);
        solve_board(board);
	gettimeofday(&tv2, NULL);

#ifndef BENCH
        printf("-----------------");
        print_board(board);
        printf("-----------------\n");
#endif

        printf (" %s", cmnt);
        left2 = count_left(board);

        diff = (tv2.tv_sec - tv1.tv_sec) * 1000000 + tv2.tv_usec - tv1.tv_usec;

        printf("Time:%6u ", diff);
        printf("Left:%3u ", left2);
        printf("Solved:%3u\n", left - left2);
    }

    fclose(fp);

    exit(0);
}
