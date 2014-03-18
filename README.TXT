
 The Sudoku Challange

 This is free software copyrighted by me and may be used freely under BSD kindof 
 license however some example solvers below are GPL:ed so this file will be contaminated 
 to GPL as well if distributed together as source or binaries. That's ok with me.

 Compile, link and run from emacs with:

   M-x compile
   gcc -o s main.c solve.c; ./s

 The solve.c should implement solve_board(char * board); function which takes
 a buffer with one line from the example board file boards.txt, for example:

 ---3--67-5--4--8--628---3-----2-4-37---------41-7-5-----2---961--6--1--4-84--9--- Medel

 The trailing characters may have some optional comments about the board

   Example open source sudoku solvers
 ===============================================================================

   C-based sudoku solver by the Code Mankeys - https://github.com/austinglaser/sudoku
 -------------------------------------------------------------------------------
   git clone https://github.com/austinglaser/sudoku
   patch -p0 < sudoku.patch
   g++ -o d -DBENCH -I sudoku main.c sudoku/main.c sudoku/sudoku.c sudoku/fifo.c sudoku.c; ./d

   YASS 1.0 (GPL) - http://sourceforge.net/project/showfiles.php?group_id=145574&package_id=160138
 -------------------------------------------------------------------------------
   tar xzvf yass-1.0.tar.gz
   cd yass-1.0; ./configure; make; cd ..
   g++ -I yass-1.0/ -DBENCH -o s yass-1.0.c yass-1.0/src/solver.cc main.c; ./s

   Sudoku Solver 0.3 (GPL) - http://robert.rsa3.com/sudoku.html
 -------------------------------------------------------------------------------
   tar xzvf sudoku-0.3.tar.gz
   patch -p0 < sudoku-0.3.patch
   cd sudoku-0.3; ./configure; make; cd ..
   g++ -I sudoku-0.3/include -DBENCH -o s main.c sudoku-0.3/src/game.cc sudoku-0.3/src/block3x3.cc sudoku-0.3.c; ./s

 ===============================================================================

  So can you beat these implementations??!! Send your results to me and I will 
  publish the results, at joakim@bildrulle.nu

 Compiling the program with the BENCH symbol defined:

   M-x compile
   gcc -DBENCH -o s main.c solve.c; ./s

 will generate output suitable for a boards.txt file and replace/prepend 
 benchmark figures It will also preserve any comments not exceeding the COMMENT_LENGTH below.

  HAVE FUN!

 Joakim Larsson, joakim@bildrulle.nu

