
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

 When you have built all the plugins you want you should delete the database and run the solvers 
 on all the sudoku boards in boards.txt to create a new database with results, for example:

 rm tss.db
 ./psolv ./cmankey.lilo.so.1.0.0  
 ./psolv ./cmankey.so.1.0.0  
 ./psolv ./reference.so.1.0.0  
 ./psolv ./sudokusolver.so.0.3.0  
 ./psolv ./yass.so.1.0.0

 Then create data sets from the database:

 sqlite3 tss.db 'select SOLVER from LAPS GROUP BY SOLVER;'

 You will get a list of the tested solvers, for instance

  Reference
  Sudoku Solver 0.3
  Yass 1.0
  cmankey-1.0
  cmankey-1.0-lilo

 Choose the solvers you want to compare and generate data files

sqlite3 tss.db 'select avg(TIME),BOARD from LAPS WHERE SOLVER LIKE "Reference" GROUP BY BOARD;' > ref.dat
sqlite3 tss.db 'select avg(TIME),BOARD from LAPS WHERE SOLVER LIKE "Sudoku Solver 0.3" GROUP BY BOARD;' > solv.dat
sqlite3 tss.db 'select avg(TIME),BOARD from LAPS WHERE SOLVER LIKE "Yass 1.0" GROUP BY BOARD;' > yass.dat

 Start gnuplot and import the data sets:

   gnuplot
   set term pdf
   set output 'report.pdf'
   plot 'ref.dat' with lines, 'solv.dat' with lines, 'yass.dat' with lines
   quit
 
 The result is now available in report.pdf, use a pdf viewer to check the results!

 If you enter more Sudoku boards to the boards.txt file, please consider submitting these for others to use.

  HAVE FUN!

 Joakim Larsson, joakim@bildrulle.nu

