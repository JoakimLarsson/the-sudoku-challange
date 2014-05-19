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

#define update_board(X) update_board2(X, col, row, sqr, cnb, board, num); fnd++;

// Cell  (0-81) to what square table (0-8) sqr[]
char sqrs[]     = "000111222000111222000111222333444555333444555333444555666777888666777888666777888";
int sqrtopos[9][9] = {
                    { 0,  1,  2,  9, 10, 11, 18, 19, 20},
		    { 3,  4,  5, 12, 13, 14, 21, 22, 23},
		    { 6,  7,  8, 15, 16, 17, 24, 25, 26},
		    {27, 28, 29, 36, 37, 38, 45, 46, 47},
		    {30, 31, 32, 39, 40, 41, 48, 49, 50},
		    {33, 34, 35, 42, 43, 44, 51, 52, 53},
		    {54, 55, 56, 63, 64, 66, 72, 73, 74},
		    {57, 58, 59, 66, 67, 68, 75, 76, 77},
		    {60, 61, 62, 69, 70, 71, 78, 79, 80} };


// Cell (0-81) to column (0-8) col[]                
char cols[]     = "012345678012345678012345678012345678012345678012345678012345678012345678012345678";

// Cell (0-81) to row table (0-8) row[]
char rows[]     = "000000000111111111222222222333333333444444444555555555666666666777777777888888888";

// Cell to first of two adjacent col/rows in a square
char n1[]    = "120453786";

// Cell to second of two adjacent col/rows in a square
char n2[]    = "201534867";

inline void update_board2(int pos, 
		   unsigned int *col, 
		   unsigned int *row, 
		   unsigned int *sqr, 
		   int *cnb, 
		   char *board, 
		   unsigned int num)
{
  /* update board */
  board[pos] = num;
  
  /* update cross refs */
  col[cols[pos]] = col[cols[pos]] | (1 << (board[pos] - (int) '0') - 1);
  row[rows[pos]] = row[rows[pos]] | (1 << (board[pos] - (int) '0') - 1); 
  sqr[sqrs[pos]] = sqr[sqrs[pos]] | (1 << (board[pos] - (int) '0') - 1);

  /* Update scratchpad */
  memset(cnb, 0, 81);
  for (int y = 0; y < 81; y++)
  {
    cnb[y] = (board[y] == '-') ? 
      ((row[rows[y]] | col[cols[y]] | sqr[sqrs[y]]) ^ 0x1ff) & 0x1ff :
      0x200 | (1 << (board[y] - (int) '0') - 1);
  }
}

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

extern "C" char *solver_name(){ return (char *) "Reference"; }

extern "C" void solve_board(char *board)
{

    // Number of bits in nybble representing 0-8
    char bits[512];

    // Keep track of what numbers is in what column,row and square
    unsigned int col[9]; // What number in column
    unsigned int row[9]; // What number in row 
    unsigned int sqr[9]; // What number in sqr
    //    unsigned int occ[9]; // What cells are occupied in sqr 

    int cnb[81]; // scratchpad

    //    int cocc[9]; // Each number temp storage for in square status

    int i, j, k, cn, cnt, fnd, type, afnd, num, a, b, left, cell, ind, res;

    for (i = 0; i < sizeof(sqrs); i++){ sqrs[i] -= (int) '0'; }
    //    for (i = 0; i < sizeof(sqri); i++){ sqri[i] -= (int) '0'; }
    for (i = 0; i < sizeof(cols); i++){ cols[i] -= (int) '0'; }
    for (i = 0; i < sizeof(rows); i++){ rows[i] -= (int) '0'; }
    for (i = 0; i < sizeof(n1); i++){ n1[i] -= (int) '0'; }
    for (i = 0; i < sizeof(n2); i++){ n2[i] -= (int) '0'; }
    //    for (i = 0; i < sizeof(sqrc); i++){ sqrc[i] -= (int) '0'; }
    for (i = 0; i < sizeof(bits); i++){ bits[i] -= (int) '0'; }

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

    //printf("\nres:");
    while (fnd > 0 || type < SINGLE_IN_ROW)
    {
      DEBUG3(print_board(board););

      type = (fnd == 0) ? type + 1 : 0;
      fnd = 0;

      DEBUG3(printf("col: "); for (i = 0; i < 9; i++) printf("%03x ", col[i]); printf("\n"););
      DEBUG3(printf("row: "); for (i = 0; i < 9; i++) printf("%03x ", row[i]); printf("\n"););
      DEBUG3(printf("sqr: "); for (i = 0; i < 9; i++) printf("%03x ", sqr[i]); printf("\n"););
      DEBUG3(printf("cnb: "); for (i = 0; i < 81; i++) printf("%s%03x", (i % 9) == 0 ? "\n     " : " ", cnb[i]); printf("\n"););
      DEBUG3(printf("Searching..using %d\n", type););

      for (i = 0; i < 9; i++)
      {
	int res;
	//printf("\n");
	for (j = 0; j < 9; j++)
	{
	  if ((cnb[j * 9 + i] & 0x200) == 0)
	  {
	    res = 0;
	    for (k = 0; k < 9; k++)
	    {
	      switch (type)
	      {
	      case SINGLE_IN_COL:
		res = res | ((k != j) ? cnb[k * 9 + i] : 0); // Collect all cells in column but the current one (i, j)
		break;
	      case SINGLE_IN_ROW:
		res = res | ((k != i) ? cnb[j * 9 + k] : 0); // Collect all cells in row but the current one (i, j)
		break;
	      case SINGLE_IN_SQR:
		printf("Square To Pos %d/%d => %d => ??\n", k, j, sqrtopos[k][j]);
		res = res | (((k != i) && (k != j)) ? cnb[sqrtopos[k][j]] : 0); // Collect all cells in sqare but the current one (i, j)
		break;
	      default:
		printf("Wrong strategy type\n");
		continue;
		break;
	      }
	    }
	    switch((res & 0x1ff) ^ 0x1ff) /* Check if there is single digit that can only fit in the current position */
	    {
	    case 0x001: DEBUG3(printf("Found a 1 in pos %d,%d\n", i, j);) num = '1'; update_board(j * 9 + i); break;
	    case 0x002: DEBUG3(printf("Found a 2 in pos %d,%d\n", i, j);) num = '2'; update_board(j * 9 + i); break;
	    case 0x004: DEBUG3(printf("Found a 3 in pos %d,%d\n", i, j);) num = '3'; update_board(j * 9 + i); break;
	    case 0x008: DEBUG3(printf("Found a 4 in pos %d,%d\n", i, j);) num = '4'; update_board(j * 9 + i); break;
	    case 0x010: DEBUG3(printf("Found a 5 in pos %d,%d\n", i, j);) num = '5'; update_board(j * 9 + i); break;
	    case 0x020: DEBUG3(printf("Found a 6 in pos %d,%d\n", i, j);) num = '6'; update_board(j * 9 + i); break;
	    case 0x040: DEBUG3(printf("Found a 7 in pos %d,%d\n", i, j);) num = '7'; update_board(j * 9 + i); break;
	    case 0x080: DEBUG3(printf("Found a 8 in pos %d,%d\n", i, j);) num = '8'; update_board(j * 9 + i); break;
	    case 0x100: DEBUG3(printf("Found a 9 in pos %d,%d\n", i, j);) num = '9'; update_board(j * 9 + i); break;
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
