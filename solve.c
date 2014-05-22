//
// g++ -DDEBUG -o s solve.c; ./s
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define DEBUG3(x)  x
#else
#define DEBUG3(x)  /* */
#endif

#define update_board(X) update_board2(X, col, cols, row, rows, sqr, sqrs, cnb, board, sqrtopos, num);

#if DEBUG | DEBUG1

// Test boards

    //char board[] = "123456789123456789123456789123456789123456789123456789123456789123456789123456789";
    //char board[] = "---------------------------------------------------------------------------------";
    //char board[] = "---3--67-5--4--8--628---3-----2-4-37---------41-7-5-----2---961--6--1--4-84--9---"; // Time 207
    //char board[] = "--71-6--24---9------128-63-2--315-6-93-47-----58---7-3----4752-6-5--19-------238-"; // Time: 25 - OK
    //char board[] = "-17--3-6---2-8--546---2---9--31---7--8-9---2346------11--37-8----4-651--9--------"; // Time: 29 - OK
    //char board[] = "3----87-------9--41---6---2--5-23-8--7---4-9--6---15--8---5---67--1-------98----5"; // Time 192 (2000 medium) - OK
    //char board[] = "-2-9--83---7--4--6--5--1-9--3-2----4--6-7-5--1---89-2--4-3--1--8--7--6---72--6-4-"; // Time 29 (2001 medium) -OK
    //char board[] = "-1---8-5--6--4-7-------2--9-3---1-8-7---6---2-5-97--4-8--3-------9-1--6--4-5---7-";   // Time 209 (2125 medium) - OK
    //char board[] = "-9--3-6------7-2----48----51--5---8---7---9---3---6--42----17----8-2------6-5--9-";   // Left 44  (2151 medium) -OK
    //char board[] = "8--6----2-4--5--1----7----3-9---4--62-------87---1--5-3----9----1--8--9-4----2--5"; // Left 56 (insane) 
    //char board[] = "-6---39--5--1-----8-------7-4-2--6--7-------8--3--9-1-2-------5-----4--3--87---2-"; // Left 43 (hard)
    char board[] = "5-64----2-7--9--5-8---5-7--7----3----89-6-37----5----1--3-4---6-5--2--4-9----51-7";
    //char board[] = "-17--3-6---2-8--546---2---9--31---7--8-9---2346------11--37-8----4-651--9--------";


static void print_board(char *board)
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

static inline void update_board2(
				 int pos, 
				 unsigned int *col,
				 char *cols, 
				 unsigned int *row,
				 char *rows,
				 unsigned int *sqr, 
				 char *sqrs,
				 int *cnb, 
				 char *board, 
				 int sqrtopos[][9],
				 unsigned int num)
{
  int numbit;
  /* update board */
  board[pos] = num;
  
  numbit = (1 << (board[pos] - (int) '0') - 1);

  /* update cross refs */
  col[cols[pos]] = col[cols[pos]] | numbit; // mark candidate or solution exist in col
  row[rows[pos]] = row[rows[pos]] | numbit; // mark candidate or solution exist in col
  sqr[sqrs[pos]] = sqr[sqrs[pos]] | numbit; // mark candidate or solution exist in col

  /* Update scratchpad */
  numbit = ~numbit & 0x3ff; // Make mask of bit position and preserve solved bit (0x200)
  for (int k = 0; k < 9; k++)
  {
    cnb[pos % 9 + k  *  9]       &= numbit; // clear candidates in same column as pos
    cnb[k       + 9 * (pos / 9)] &= numbit; // clear candidates in same row as pos
    cnb[sqrtopos[sqrs[pos]][k]]  &= numbit; // clear candidates in same square as pos
  }
  numbit = ~numbit & 0x1ff;   // Make bit position of mask
  cnb[pos] = 0x200 | numbit;  // Mark position as solved

  DEBUG3(printf("Col: "); for (int i = 0; i < 9; i++) printf("%03x ", col[i]); printf("\n"););
  DEBUG3(printf("Row: "); for (int i = 0; i < 9; i++) printf("%03x ", row[i]); printf("\n"););
  DEBUG3(printf("Sqr: "); for (int i = 0; i < 9; i++) printf("%03x ", sqr[i]); printf("\n"););
  DEBUG3(printf("Cnb: "); for (int i = 0; i < 81; i++) printf("%s%03x", (i % 9) == 0 ? "\n     " : " ", cnb[i]); printf("\n"););
  DEBUG3(print_board(board););
}

extern "C" char *solver_name(){ return (char *) "Ref 1.1"; }

extern "C" void solve_board(char *board)
{
// Cell  (0-81) to what square (0-8)
char sqrs[]     = "000111222000111222000111222333444555333444555333444555666777888666777888666777888";

// pos (0-8) in square (0-8) converted to global pos (0-81)
int sqrtopos[9][9] = {
                    { 0,  1,  2,  9, 10, 11, 18, 19, 20},
		    { 3,  4,  5, 12, 13, 14, 21, 22, 23},
		    { 6,  7,  8, 15, 16, 17, 24, 25, 26},
		    {27, 28, 29, 36, 37, 38, 45, 46, 47},
		    {30, 31, 32, 39, 40, 41, 48, 49, 50},
		    {33, 34, 35, 42, 43, 44, 51, 52, 53},
		    {54, 55, 56, 63, 64, 65, 72, 73, 74},
		    {57, 58, 59, 66, 67, 68, 75, 76, 77},
		    {60, 61, 62, 69, 70, 71, 78, 79, 80} };


// Cell (0-81) to column (0-8)                
char cols[]     = "012345678012345678012345678012345678012345678012345678012345678012345678012345678";

// Cell (0-81) to row (0-8)
char rows[]     = "000000000111111111222222222333333333444444444555555555666666666777777777888888888";

// Cell to first of two adjacent col/rows in a square
char n1[]    = "120453786";

// Cell to second of two adjacent col/rows in a square
char n2[]    = "201534867";


    // Number of bits in nybble representing 0-8
    //char bits[512];

    // Keep track of what numbers is in what column,row and square
    unsigned int col[9]; // What number in column
    unsigned int row[9]; // What number in row 
    unsigned int sqr[9]; // What number in sqr
    //    unsigned int occ[9]; // What cells are occupied in sqr 

    int cnb[81]; // scratchpad

    //    int cocc[9]; // Each number temp storage for in square status

    int i, j, k, cn, cnt, fnd, type, afnd, num, a, b, left, cell, ind, res;
    int ci, ri;

    for (i = 0; i < sizeof(sqrs); i++){ sqrs[i] -= (int) '0'; }
    //    for (i = 0; i < sizeof(sqri); i++){ sqri[i] -= (int) '0'; }
    for (i = 0; i < sizeof(cols); i++){ cols[i] -= (int) '0'; }
    for (i = 0; i < sizeof(rows); i++){ rows[i] -= (int) '0'; }
    //for (i = 0; i < sizeof(n1); i++){ n1[i] -= (int) '0'; }
    //for (i = 0; i < sizeof(n2); i++){ n2[i] -= (int) '0'; }
    //    for (i = 0; i < sizeof(sqrc); i++){ sqrc[i] -= (int) '0'; }
    //for (i = 0; i < sizeof(bits); i++){ bits[i] -= (int) '0'; }

    DEBUG3(print_board(board);)

    // Clear bittables
    memset(col, 0, sizeof(col));
    memset(row, 0, sizeof(row));
    memset(sqr, 0, sizeof(sqr));
    //    memset(occ, 0, sizeof(occ));

    // Populate bittables and count empty cells
    left = 0;
    for (i = 0; i < 81; i++)
    {
        if (board[i] != '-')
        {
            col[cols[i]] = col[cols[i]] | (1 << (board[i] - (int) '0') - 1);
            row[rows[i]] = row[rows[i]] | (1 << (board[i] - (int) '0') - 1); 
            sqr[sqrs[i]] = sqr[sqrs[i]] | (1 << (board[i] - (int) '0') - 1);
        }
        else
        {
            left++;            
        }
    }

    memset(cnb, 0, sizeof(cnb));
    for (i = 0; i < 81; i++)
    { 	
      if (board[i] == '-') // Check only empty squares
      { 
	cnb[i] = ((row[rows[i]] | col[cols[i]] | sqr[sqrs[i]]) ^ 0x1ff) & 0x1ff;
      }
      else
      {
	cnb[i] = 0x200 | (1 << (board[i] - (int) '0') - 1);
      }
    }

    fnd = 1;
    type = 0;

#define SINGLE_IN_COL 0
#define SINGLE_IN_ROW 1
#define SINGLE_IN_SQR 2
#define NAKED_IN_COL  3

    while (fnd > 0 || type < SINGLE_IN_SQR)
    {
      DEBUG3(print_board(board););

      type = (fnd == 0) ? type + 1 : 0;
      fnd = 0;

      DEBUG3(printf("col: "); for (i = 0; i < 9; i++) printf("%03x ", col[i]); printf("\n"););
      DEBUG3(printf("row: "); for (i = 0; i < 9; i++) printf("%03x ", row[i]); printf("\n"););
      DEBUG3(printf("sqr: "); for (i = 0; i < 9; i++) printf("%03x ", sqr[i]); printf("\n"););
      DEBUG3(printf("cnb: "); for (i = 0; i < 81; i++) printf("%s%03x", (i % 9) == 0 ? "\n     " : " ", cnb[i]); printf("\n"););
      DEBUG3(printf("Searching..using %d\n", type););

      for (ci = 0; ci < 9; ci++)
      {
	int res;

	for (ri = 0; ri < 9; ri++)
	{
	  if ((cnb[ri * 9 + ci] & 0x200) == 0)
	  {
	    res = 0;
	    // Check neigbours in row, col and sqr
	    for (k = 0; k < 9; k++)
	    {
	      switch (type)
	      {
	      case SINGLE_IN_COL:
		res = res | ((k != ri) ? cnb[k * 9 + ci] : 0); // Collect all cells in column but the current one (ci, ri)
		break;
	      case SINGLE_IN_ROW:
		res = res | ((k != ci) ? cnb[ri * 9 + k] : 0); // Collect all cells in row but the current one (ci, ri)
		break;
	      case SINGLE_IN_SQR:
		res = res | (sqrtopos[sqrs[ri * 9 + ci]][k] != (ri * 9 + ci) ? cnb[sqrtopos[sqrs[ri * 9 + ci]][k]] : 0); // Collect all cells in sqare but the current one (ci, ri)
		break;
	      default:
		printf("Wrong strategy type\n");
		continue;
		break;
	      }
	    }
	    switch((res & 0x1ff) ^ 0x1ff) /* Check if there is single digit that can only fit in the current position */
	    {
	    case 0x001: DEBUG3(printf("Found a 1 in pos %d,%d\n", ci, ri);) num = '1'; update_board(ri * 9 + ci); fnd++; break;
	    case 0x002: DEBUG3(printf("Found a 2 in pos %d,%d\n", ci, ri);) num = '2'; update_board(ri * 9 + ci); fnd++; break;
	    case 0x004: DEBUG3(printf("Found a 3 in pos %d,%d\n", ci, ri);) num = '3'; update_board(ri * 9 + ci); fnd++; break;
	    case 0x008: DEBUG3(printf("Found a 4 in pos %d,%d\n", ci, ri);) num = '4'; update_board(ri * 9 + ci); fnd++; break;
	    case 0x010: DEBUG3(printf("Found a 5 in pos %d,%d\n", ci, ri);) num = '5'; update_board(ri * 9 + ci); fnd++; break;
	    case 0x020: DEBUG3(printf("Found a 6 in pos %d,%d\n", ci, ri);) num = '6'; update_board(ri * 9 + ci); fnd++; break;
	    case 0x040: DEBUG3(printf("Found a 7 in pos %d,%d\n", ci, ri);) num = '7'; update_board(ri * 9 + ci); fnd++; break;
	    case 0x080: DEBUG3(printf("Found a 8 in pos %d,%d\n", ci, ri);) num = '8'; update_board(ri * 9 + ci); fnd++; break;
	    case 0x100: DEBUG3(printf("Found a 9 in pos %d,%d\n", ci, ri);) num = '9'; update_board(ri * 9 + ci); fnd++; break;
	    default: break;
	    }
	  }
	}
      }
    }
    DEBUG3(print_board(board););  
}

#if DEBUG | DEBUG1

main()
{
    solve_board(board);

    exit(0);
}

#endif
