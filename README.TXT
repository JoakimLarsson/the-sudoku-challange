
 The Sudoku Challange

 This is free software copyrighted by me and may be used freely under BSD kindof 
 license however some example solvers below are GPL:ed so this file will be contaminated 
 to GPL as well if distributed together as source or binaries. That's ok with me.

 Compile reference solver, link and run from emacs with:

   M-x compile
   gcc -o s main.c solve.c; ./s

 The solve.c implements solve_board(char * board); function which takes
 a buffer with one line from the example board file boards.txt, for example:

 ---3--67-5--4--8--628---3-----2-4-37---------41-7-5-----2---961--6--1--4-84--9--- Medel

 The trailing characters may have some optional comments about the board

   Open source sudoku solvers
 ===============================================================================

 Just run 'make' to download and build plugins suitable for test bench from the 
 following sources:

   C-based sudoku solver by the Code Mankeys - https://github.com/austinglaser/sudoku
   YASS 1.0              by Binesh Bannerjee - http://sourceforge.net/project/showfiles.php?group_id=145574&package_id=160138
   Sudoku Solver 0.3     by Robert Sandland  - http://robert.rsa3.com/sudoku.html

 So can you beat these implementations??!!

   Benchmarking
=================================================================================
 Compiling the program with the BENCH symbol defined:

   M-x compile
   gcc -DBENCH -o s main.c solve.c; ./s

 will generate output suitable for a boards.txt file and replace/prepend 
 benchmark figures It will also preserve any comments not exceeding the COMMENT_LENGTH below.

  HAVE FUN!

 Joakim Larsson, joakim@bildrulle.nu

