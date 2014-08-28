//
// g++ -DDEBUG -o s solve.c; ./s
//

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define DEBUG3(x) x
#define DEBUG4(x) 
#else

/* Debug statements as plugin */
#ifdef DEBUG2
#define DEBUG3(x) x
#define DEBUG4(x) x
#else
#define DEBUG3(x)  /* */
#define DEBUG4(x)  /* */
#endif
#endif


#if DEBUG | DEBUG1| DEBUG2

// Test boards

    //char board[] = "123456789123456789123456789123456789123456789123456789123456789123456789123456789";
    //char board[] = "---------------------------------------------------------------------------------";
//    char board[] = "8--6----2-4--5--1----7----3-9---4--62-------87---1--5-3----9----1--8--9-4----2--5"; // Left 56 (insane) 
char board[] = "1...3468....8..54..84.6.321.18...2..3..1824.6......81.521378964..6..9..889.64...2";
//    char board[] = "2-3-8----8--7-----------1---6-5-7---4------3----1------------82-5----6---1-------";
//char board[]="82917658456432879131754938213346-83363628145-4-66532123418322-6756934128338712-45";
    //char board[] = "5-64----2-7--9--5-8---5-7--7----3----89-6-37----5----1--3-4---6-5--2--4-9----51-7"; // A test board fro solve.c
//char board[] = "-6---39--5--1-----8-------7-4-2--6--7-------8--3--9-1-2-------5-----4--3--87---2-"; // Left 43 (hard)


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

static void print_line(char *board)
{
    int a, b;

    printf("\n\n");
    for (a = 0; a < 9; a++)
    {
        for (b = 0; b < 9; b++)
        {
	  printf("%c",board[a * 9 + b] != '-' ? board[a * 9 + b] : '.');
        }    
    }
    printf("\n");
}

static inline unsigned int bits2nbr(unsigned int bits)
{
  unsigned int res;
  unsigned int fact = 0;
  for (int i = 0; i < 10; i++)
  {
    res += ((bits & (1 << i)) != 0) ? (i + 1) * (10 ^ fact++) : 0;
  }
  return res;
}

static inline void print_candidates(unsigned int *candidates)
{
  printf("Candidates: "); for (int i = 0; i < 81; i++) printf("%s%03x", (i % 9) == 0 ? "\n     " : " ", candidates[i]); printf("\n");
  //  printf("Candidates:\n");
  printf("o-----o-----o-----o-----o-----o-----o-----o-----o-----o\n");
  for (int l = 0; l < 9; l++){
    for (int i = 0; i < 3; i++){
      printf("| ");
      for (int j = 0; j < 9; j++){
	for (int k = 0; k < 3; k++){
	  printf("%c", ~candidates[j + 9 * l] & (1 << (k + i * 3)) ? '.' : '0' + k + i * 3 + 1);
	}
	printf(" | ");
      }
      printf("\n");
    }
    char ch = (l + 1) % 3 ? '+' : 'o';
    printf("o-----%c-----%c-----o-----%c-----%c-----o-----%c-----%c-----o\n", ch, ch, ch, ch, ch, ch);
  }
}
#endif

static inline int maskrow(int pos, unsigned int mask, unsigned int *cnb, char *sqrs, char *rows)
{
  int res = 0;
  int row = rows[pos];
  mask &= 0x1ff;
  for (int q = 0; q < 9; q++)
  {
    if (sqrs[row * 9 + q] != sqrs[pos])
    {
      if ((cnb[row * 9 + q] & mask) != 0)
      {
	DEBUG3(printf("Found a Pointing in Square, maskrow %03x in row %d and col %d", row * 9 + q & mask, row, q););
	DEBUG3(print_board(board););
	DEBUG3(print_candidates(cnb););
	cnb[row * 9 + q] &= ~mask;
	res += 1;
      }
    }
  }
  return res;
}

static inline int maskcol(int pos, unsigned int mask, unsigned int *cnb, char *sqrs, char *cols)
{
  int res = 0;
  int col = cols[pos];
  mask &= 0x1ff;

  for (int q = 0; q < 9; q++)
  {
    if (sqrs[q * 9 + col] != sqrs[pos])
    {
      if ((cnb[q * 9 + col] & mask) != 0)
      {
	DEBUG3(printf("Found a Pointing in Square, maskcol %03x in row %d and col %d", q * 9 + col & mask, q, col););
	DEBUG3(print_board(board););
	DEBUG3(print_candidates(cnb););
	cnb[q * 9 + col] &= ~mask;
	res += 1;
      }
    }
  }
  return res;
}

static inline void update_board(int pos, char *board, unsigned int *cnb, unsigned int num)
{
  int numbit;

  /* update board */
  board[pos] = num;
  numbit = (1 << (board[pos] - (int) '0') - 1);
  cnb[pos] = 0x200 | numbit;  // Mark position as solved  

  DEBUG3(printf("Cnb: "); for (int i = 0; i < 81; i++) printf("%s%03x", (i % 9) == 0 ? "\n     " : " ", cnb[i]); printf("\n"););
  DEBUG3(print_board(board););
  DEBUG4(print_candidates(cnb););
}

static inline void reduce_candidates(
				     int pos, 
				     unsigned int *col,
				     char *cols, 
				     unsigned int *row,
				     char *rows,
				     unsigned int *sqr, 
				     char *sqrs,
				     unsigned int *cnb, 
				     char *board, 
				     int sqrtopos[][9],
				     unsigned int num)
{
  int numbit;
  
  numbit = (1 << (num - (int) '0') - 1);

  /* update cross refs */
  col[cols[pos]] = col[cols[pos]] | numbit; // mark candidate or solution exist in col
  row[rows[pos]] = row[rows[pos]] | numbit; // mark candidate or solution exist in row
  sqr[sqrs[pos]] = sqr[sqrs[pos]] | numbit; // mark candidate or solution exist in sqr

  /* Update scratchpad */
  numbit = ~numbit & 0x3ff; // Make mask of bit position and preserve solved bit (0x200)
  for (int k = 0; k < 9; k++)
  {
    cnb[pos % 9 + k  *  9]       &= numbit; // clear candidates in same column as pos
    cnb[k       + 9 * (pos / 9)] &= numbit; // clear candidates in same row as pos
    cnb[sqrtopos[sqrs[pos]][k]]  &= numbit; // clear candidates in same square as pos
  }

  DEBUG3(printf("Col: "); for (int i = 0; i < 9; i++) printf("%03x ", col[i]); printf("\n"););
  DEBUG3(printf("Row: "); for (int i = 0; i < 9; i++) printf("%03x ", row[i]); printf("\n"););
  DEBUG3(printf("Sqr: "); for (int i = 0; i < 9; i++) printf("%03x ", sqr[i]); printf("\n"););
}

extern "C" char *solver_name(){ return (char *) "Ref 1.3"; }

extern "C" void solve_board(char *board)
{
// Cell  (0-81) to what square (0-8)
char sqrs[]     = "000111222000111222000111222333444555333444555333444555666777888666777888666777888";

// Cell  (0-81) to what position in square (0-8)
char sqrp[]     = "012012012345345345678678678012012012345345345678678678012012012345345345678678678";

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
//char n1[]    = "120453786";

// Cell to second of two adjacent col/rows in a square
//char n2[]    = "201534867";


    // Number of bits in nybble representing 0-8
    //char bits[512];

    // Keep track of what numbers is in what column,row and square
    unsigned int col[9]; // What number in column
    unsigned int row[9]; // What number in row 
    unsigned int sqr[9]; // What number in sqr
    //    unsigned int occ[9]; // What cells are occupied in sqr 

    unsigned int cnb[81]; // scratchpad
    unsigned int countbits[0x3ff]; // Lookup table for how many set bits there are in a value

    //    int cocc[9]; // Each number temp storage for in square status

    int i, j, k, cn, cnt, fnd, type, afnd, num, a, b, left, cell, ind, res;
    int ci, ri;

    for (i = 0; i < sizeof(sqrs); i++){ sqrs[i] -= (int) '0'; }
    for (i = 0; i < sizeof(sqrp); i++){ sqrp[i] -= (int) '0'; }
    //    for (i = 0; i < sizeof(sqri); i++){ sqri[i] -= (int) '0'; }
    for (i = 0; i < sizeof(cols); i++){ cols[i] -= (int) '0'; }
    for (i = 0; i < sizeof(rows); i++){ rows[i] -= (int) '0'; }
    //for (i = 0; i < sizeof(n1); i++){ n1[i] -= (int) '0'; }
    //for (i = 0; i < sizeof(n2); i++){ n2[i] -= (int) '0'; }
    //    for (i = 0; i < sizeof(sqrc); i++){ sqrc[i] -= (int) '0'; }
    //for (i = 0; i < sizeof(bits); i++){ bits[i] -= (int) '0'; }

    // Initiate the bitcount lookup table, shamelessly based on 
    // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetTable
    memset(countbits, 0, sizeof(countbits));
    for (i = 0; i < 512; i++)
    {
      countbits[i] = (i & 1) + countbits[i / 2];
      //printf("%03x:%d%s", i, countbits[i], ((i + 1) % 16) == 0 ? "\n" : " ");
    }

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
    DEBUG3(printf("col: "); for (i = 0; i < 9; i++) printf("%03x ", col[i]); printf("\n"););
    DEBUG3(printf("row: "); for (i = 0; i < 9; i++) printf("%03x ", row[i]); printf("\n"););
    DEBUG3(printf("sqr: "); for (i = 0; i < 9; i++) printf("%03x ", sqr[i]); printf("\n"););

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

    DEBUG3(print_board(board);)
    DEBUG3(print_candidates(cnb););

    enum SolverStrategiesType {
  // Single out the right value techniques
    SINGLE_IN_COL,
    SINGLE_IN_ROW,
    SINGLE_IN_SQR,
    // Reduction strategies
    NAKED_PAIR_IN_COL,
    NAKED_PAIR_IN_ROW,
    NAKED_PAIR_IN_SQR,
    NAKED_TRIPLE_IN_COL,
    NAKED_TRIPLE_IN_ROW,
    NAKED_TRIPLE_IN_SQR,
    HIDDEN_PAIR_IN_COL,
    HIDDEN_PAIR_IN_ROW,
    HIDDEN_PAIR_IN_SQR,
    POINTINGS_IN_SQR,
    LINES_FROM_SQR,
    SQR_REDUCTION
    };

#ifdef DEBUG
    const char *SST[] = { 
      "Single in Column",        "Single in Row",        "Single in Square",  
      "Naked Pair in Column",    "Naked Pair in Row",    "Naked Pair in Square",  
      "Naked Triple in Column",  "Naked Triple in Row",  "Naked Triple in Square",  
      "Hidden Pair in Column",   "Hidden Pair in Row",   "Hidden Pair in Square",
      "Pointings in Square",     "Lines From Box",       "Box Reduction"
    };
#endif

    fnd = 1;
    type = SINGLE_IN_COL;
    while (fnd > 0 || type < LINES_FROM_SQR)
    {
      DEBUG4(print_board(board););
      type = (fnd == 0) ? type + 1 : 0;
      fnd = 0;

      DEBUG4(printf("col: "); for (i = 0; i < 9; i++) printf("%03x ", col[i]); printf("\n"););
      DEBUG4(printf("row: "); for (i = 0; i < 9; i++) printf("%03x ", row[i]); printf("\n"););
      DEBUG4(printf("sqr: "); for (i = 0; i < 9; i++) printf("%03x ", sqr[i]); printf("\n"););
      DEBUG4(printf("cnb: "); for (i = 0; i < 81; i++) printf("%s%03x", (i % 9) == 0 ? "\n     " : " ", cnb[i]); printf("\n"););
      DEBUG4(print_candidates(cnb););
      DEBUG3(printf("Searching using %s\n", SST[type]););

      for (ci = 0; ci < 9; ci++)
      {
	int res;

	for (ri = 0; ri < 9; ri++)
	{
	  DEBUG4(printf("Searching at %d/%d using %s\n", ci, ri, SST[type]););

	  if ((cnb[ri * 9 + ci] & 0x200) == 0)
	  {
	    if (type < NAKED_PAIR_IN_COL) /* find positions with single candidates and promote them to solved */
	    {
	      res = 0x1ff;
	      switch (type)
		{
		case SINGLE_IN_COL: // single instance for col in pos
		  for (k = 0; k < 9; k++) { res = res & ((k != ri) ? ~cnb[k * 9 + ci] : 0x1ff); } 
		  break;
		case SINGLE_IN_ROW: // single instance for row in pos
		  for (k = 0; k < 9; k++) { res = res & ((k != ci) ? ~cnb[ri * 9 + k] : 0x1ff);  }
		  break;
		case SINGLE_IN_SQR: // collect all cells in sqare but the current one (ci, ri)
		  for (k = 0; k < 9; k++) { res = res & (sqrtopos[sqrs[ri * 9 + ci]][k] != (ri * 9 + ci) ? ~cnb[sqrtopos[sqrs[ri * 9 + ci]][k]] : 0x1ff); }
		  break;
		default:
		  printf("Wrong strategy type %d\n", type);
		  continue;
		  break;
		}

	      /* Check if there is single digit that can only fit in the current position */
	      res = (res & 0x1ff); // Mask
	      switch(res)
	      {
	      case 0x001: num = '1'; break;
	      case 0x002: num = '2'; break;
	      case 0x004: num = '3'; break;
	      case 0x008: num = '4'; break;
	      case 0x010: num = '5'; break;
	      case 0x020: num = '6'; break;
	      case 0x040: num = '7'; break;
	      case 0x080: num = '8'; break;
	      case 0x100: num = '9'; break;
	      default: 	  num = '0'; break;
	      }
	      if (num != '0')
	      {
		DEBUG3(printf("Found a %c in pos %d,%d\n", num, ci, ri);) ;

		reduce_candidates( ri * 9 + ci, col, cols, row, rows, sqr, sqrs, cnb, board, sqrtopos, num);
		update_board( ri * 9 + ci, board, cnb, num);
		fnd++;
	      }
	    }
	    else /* Use reduction techniques to enable single candidates */
	    {
	      switch (type)
	      {
	      case LINES_FROM_SQR:
		{
		  
		}
		break;
	      case SQR_REDUCTION:
		break;
	      case POINTINGS_IN_SQR:
		{
		  DEBUG4(printf("Checking Box: %d for number %d modulus 3: %d\n", ci, ri + 1, ci % 3););
		  // Find all numbers possible in rows and cols
		  int brow1 = cnb[sqrtopos[ci][0]] | cnb[sqrtopos[ci][1]] | cnb[sqrtopos[ci][2]];
		  int brow2 = cnb[sqrtopos[ci][3]] | cnb[sqrtopos[ci][4]] | cnb[sqrtopos[ci][5]];
		  int brow3 = cnb[sqrtopos[ci][6]] | cnb[sqrtopos[ci][7]] | cnb[sqrtopos[ci][8]];
		  int bcol1 = cnb[sqrtopos[ci][0]] | cnb[sqrtopos[ci][3]] | cnb[sqrtopos[ci][6]];
		  int bcol2 = cnb[sqrtopos[ci][1]] | cnb[sqrtopos[ci][4]] | cnb[sqrtopos[ci][7]];
		  int bcol3 = cnb[sqrtopos[ci][2]] | cnb[sqrtopos[ci][5]] | cnb[sqrtopos[ci][8]];
		  // Find unique numbers in each row and col
		  if (brow1 & ~(brow2 | brow3)) fnd += maskrow(sqrtopos[ci][0], brow1 & ~(brow2 | brow3), cnb, sqrs, rows);
		  if (brow2 & ~(brow1 | brow3)) fnd += maskrow(sqrtopos[ci][3], brow2 & ~(brow1 | brow3), cnb, sqrs, rows);
		  if (brow3 & ~(brow2 | brow1)) fnd += maskrow(sqrtopos[ci][6], brow3 & ~(brow2 | brow1), cnb, sqrs, rows);

		  if (bcol1 & ~(bcol2 | bcol3)) fnd += maskcol(sqrtopos[ci][0], bcol1 & ~(bcol2 | bcol3), cnb, sqrs, cols);
		  if (bcol2 & ~(bcol1 | bcol3)) fnd += maskcol(sqrtopos[ci][1], bcol2 & ~(bcol1 | bcol3), cnb, sqrs, cols);
		  if (bcol3 & ~(bcol2 | bcol1)) fnd += maskcol(sqrtopos[ci][2], bcol3 & ~(bcol2 | bcol1), cnb, sqrs, cols);
		  ri = 9;
		}
		
		break;
	      case HIDDEN_PAIR_IN_COL:
		res = 0x1ff;
		for (int k1 = 0; k1 < 9; k1++)
		{
		  if (k1 != ri)
		  {
		    int numbit;

		    numbit = cnb[ri * 9 + ci] & cnb[k1 * 9 + ci]; // set candidates present in both pos of pair
		    for (int k2 = 0; k2 < 9; k2++)
		    {
		      if (k2 != k1 && k2 != ri)
		      {
			numbit &= ~cnb[k2  *  9 + ci]; // clear candidates that can exist in other pos
		      }
		    }
		    if (countbits[numbit] == 2)
		    {
		      DEBUG4(printf("  Found a hidden pair in col at %d/%d and %d/%d with %03x\n", ci, ri, ci, k1, numbit););
		      fnd += (cnb[ri * 9 + ci] & numbit) != cnb[ri * 9 + ci] || (cnb[k1 * 9 + ci] & numbit) != cnb[k1 * 9 + ci] ? 1 : 0;
		      cnb[ri * 9 + ci] &= numbit;
		      cnb[k1 * 9 + ci] &= numbit;
		    }
		  }
		}  
		break;
	      case HIDDEN_PAIR_IN_ROW:
		res = 0x1ff;
		for (int k1 = 0; k1 < 9; k1++)
		{
		  if (k1 != ri)
		  {
		    int numbit;

		    numbit = cnb[ri * 9 + ci] & cnb[ri * 9 + k1]; // set candidates present in both pos of pair
		    for (int k2 = 0; k2 < 9; k2++)
		    {
		      if (k2 != k1 && k2 != ri)
		      {
			numbit &= ~cnb[ri  *  9 + k2]; // clear candidates that can exist in other pos
		      }
		    }
		    if (countbits[numbit] == 2)
		    {
		      fnd += (cnb[ri * 9 + ci] & numbit) != cnb[ri * 9 + ci] || (cnb[ri * 9 + k1] & numbit) != cnb[ri * 9 + k1] ? 1 : 0;
		      cnb[ri * 9 + ci] &= numbit;
		      cnb[ri * 9 + k1] &= numbit;
		      DEBUG4(printf("  Found a hidden pair in row at %d/%d and %d/%d with %03x\n", ci, ri, ci, k1, numbit););
		    }
		  }
		}  
		break;
	      case HIDDEN_PAIR_IN_SQR:
		res = 0x1ff;
		for (int k1 = 0; k1 < 9; k1++)
		{
		  if (k1 != sqrp[ri * 9 + ci])
		  {
		    int numbit;

		    numbit = cnb[ri * 9 + ci] & cnb[sqrtopos[sqrs[ri * 9 + ci]][k1]]; // set candidates present in both pos of pair
		    for (int k2 = 0; k2 < 9; k2++)
		    {
		      if (k2 != k1 && k2 != sqrp[ri * 9 + ci])
		      {
			numbit &= ~cnb[sqrtopos[sqrs[ri * 9 + ci]][k2]]; // clear candidates that can exist in other pos
		      }
		    }
		    if (countbits[numbit] == 2)
		    {
		      int tmp1, tmp2;
		      tmp1 = cnb[ri * 9 + ci];
		      cnb[ri * 9 + ci] &= numbit;
		      tmp2 = cnb[sqrtopos[sqrs[ri * 9 + ci]][k1]];
		      cnb[sqrtopos[sqrs[ri * 9 + ci]][k1]] &= numbit;
		      if (tmp1 != cnb[ri * 9 + ci] || tmp2 != cnb[sqrtopos[sqrs[ri * 9 + ci]][k1]])
		      {
			DEBUG4(printf("Found a hidden pair in sqr and changed %d - %d from %03x - %03x to %03x - %03x\n", 
			       ri * 9 + ci, sqrtopos[sqrs[ri * 9 + ci]][k1], tmp1, tmp2, cnb[ri * 9 + ci],  
				      cnb[sqrtopos[sqrs[ri * 9 + ci]][k1]]););
			fnd++;
			DEBUG4(print_candidates(cnb););
		      }
		    }
		  }
		}
		
		break;
	      case NAKED_PAIR_IN_COL:
		/* Find to positions in the same column with the same single two candidates */
		if (countbits[cnb[ri * 9 + ci]] == 2)
		{
		  for (k = ri; k < 9; k++)
		  {
		    if (k != ri && countbits[cnb[k * 9 + ci] & 0x1ff] == 2 && cnb[k * 9 + ci] == cnb[ri * 9 + ci])
		    {
		      int numbit;

		      DEBUG4(printf("Found naked pair in col %03x in both (%d/%d) and (%d/%d)\n", cnb[ri * 9 + ci], ri, ci, k, ci););
		      /* Update scratchpad */
		      numbit = ~cnb[ri * 9 + ci] & 0x3ff; // Make mask of bit position and preserve solved bit (0x200)
		      for (int r = 0; r < 9; r++)
		      {
			if (r != k && r != ri)
			{
			  fnd += (cnb[r  *  9 + ci] & (~numbit & 0x1ff)) ? 1 : 0;
			  cnb[r  *  9 + ci] &= numbit; // clear candidates in same column as pos
			}
		      }
		    }
		  }  
		}
		break;
	      case NAKED_PAIR_IN_ROW:
		/* Find to positions in the same row with the same single two candidates */
		if (countbits[cnb[ri * 9 + ci]] == 2)
		{
		  for (k = ci; k < 9; k++)
		  {
		    if (k != ci && countbits[cnb[ri * 9 + k] & 0x1ff] == 2 && cnb[ri * 9 + k] == cnb[ri * 9 + ci])
		    {
		      int numbit;

		      DEBUG4(printf("Found naked pair in row %03x in both (%d/%d) and (%d/%d)\n", cnb[ri * 9 + ci], ri, ci, ri, k););
		      /* Update scratchpad */
		      numbit = ~cnb[ri * 9 + ci] & 0x3ff; // Make mask of bit position and preserve solved bit (0x200)
		      for (int c = 0; c < 9; c++)
		      {
			if (c != k && c != ci)
			{
			  fnd += (cnb[ri  *  9 + c] & (~numbit & 0x1ff)) ? 1 : 0;
			  cnb[ri  *  9 + c] &= numbit; // clear candidates in same row as pos
			}
		      }
		    }
		  }  
		}
		break;
	      case NAKED_PAIR_IN_SQR:
		/* Find to positions in the same square with the same single two candidates */
		if (countbits[cnb[ri * 9 + ci]] == 2)
		{
		  for (k = 0; k < 9; k++)
		  {
		    if (ri * 9 + ci != sqrtopos[sqrs[ri * 9 + ci]][k] &&                   // Not the same square 
			countbits[cnb[sqrtopos[sqrs[ri * 9 + ci]][k]] & 0x1ff] == 2 &&  // same bitcount
			cnb[sqrtopos[sqrs[ri * 9 + ci]][k]] == cnb[ri * 9 + ci])        // same bits 
		    {
		      int numbit;

		      DEBUG4(printf("Found naked pair in sqr %03x in both (%d/%d) and (%d/%d), masking out candidates\n", 
				    cnb[ri * 9 + ci], ri, ci, 
				    rows[sqrtopos[sqrs[ri * 9 + ci]][k]], 
				    cols[sqrtopos[sqrs[ri * 9 + ci]][k]]););
		      /* Update scratchpad */
		      numbit = ~cnb[ri * 9 + ci] & 0x3ff; // Make mask of bit position and preserve solved bit (0x200)
		      for (int s = 0; s < 9; s++) // Go over square to mask all but the two found positions
		      {
			if (ri * 9 + ci != sqrtopos[sqrs[ri * 9 + ci]][s] && s != k)
			{
			  fnd += (cnb[sqrtopos[sqrs[ri * 9 + ci]][s]] & (~numbit & 0x1ff)) ? 1 : 0;
			  cnb[sqrtopos[sqrs[ri * 9 + ci]][s]] &= numbit; // clear candidates in same row as pos
			}
		      }
		    }
		  }  
		}
		break;
	      case NAKED_TRIPLE_IN_COL:
		/* Find to positions in the same square with the same single two candidates */
		if (countbits[cnb[ri * 9 + ci]] <= 3)
		{
		  for (int k1 = ri + 1; k1 < 9; k1++)
		  {
		    for (int k2 = k1 + 1; k2 < 9; k2++)
		    {
		      if (countbits[cnb[k1 * 9 + ci]] <= 3 &&
			  countbits[cnb[k2 * 9 + ci]] <= 3)
		      {
			int mask = cnb[ri * 9 + ci] | cnb[k1 * 9 + ci] | cnb[k2 * 9 + ci];
			if (countbits[mask] == 3)
			{
			  mask = (~mask) & 0x3ff;
			  DEBUG3(printf("Found a Naked Triple in col at %d/%d, %d/%d and %d/%d\n", ci, ri, ci, k1, ci, k2););
			  DEBUG4(print_candidates(cnb););
			  for (int k3 = 0; k3 < 9; k3++)
			  {
			    if (k3 != ri && k3 != k1 && k3 != k2)
			    {
			      DEBUG4(printf("  Masking %d/%d with mask: %03x\n", ci, k3, mask););
			      fnd += (cnb[k3  *  9 + ci] & (~mask & 0x1ff)) ? 1 : 0;
			      cnb[k3 * 9 + ci] &= mask;
			    }
			  }
			}
		      }
		    }
		  }
		}
		break;
	      case NAKED_TRIPLE_IN_ROW:
		/* Find to positions in the same square with the same single two candidates */
		if (countbits[cnb[ri * 9 + ci]] <= 3)
		{
		  for (int k1 = ci + 1; k1 < 9; k1++)
		  {
		    for (int k2 = k1 + 1; k2 < 9; k2++)
		    {
		      if (countbits[cnb[ri * 9 + k1]] <= 3 &&
			  countbits[cnb[ri * 9 + k2]] <= 3)
		      {
			int mask = cnb[ri * 9 + ci] | cnb[ri * 9 + k1] | cnb[ri * 9 + k2];
			if (countbits[mask] == 3)
		        {
			  mask = (~mask) & 0x3ff;
			  DEBUG4(printf("Found Triple in row at %d/%d, %d/%d and %d/%d\n", ci, ri, k1, ri, k2, ri););
			  for (int k3 = 0; k3 < 9; k3++)
		          {
			    if (k3 != ci && k3 != k1 && k3 != k2)
			    {
			      DEBUG4(printf("  Masking %d/%d\n", k3, ri););
			      fnd += (cnb[ri  *  9 + k3] & (~mask & 0x1ff)) ? 1 : 0;
			      cnb[ri * 9 + k3] &= mask;
			    }
			  }
			}
		      }
		    }
		  }
		}
		break;
	      case NAKED_TRIPLE_IN_SQR:
		/* Find to positions in the same square with the same single two candidates */
		if (countbits[cnb[ri * 9 + ci]] <= 3)
		{
		  DEBUG4(printf("Suspecting triple at %d/%d\n", ci, ri););
		  for (int k1 = 0; k1 < 9; k1++)
		  {
		    for (int k2 = k1 + 1; k2 < 9; k2++)
		    {
		      DEBUG4(printf("  Checking out pos %d and %d - ", 
				    sqrtopos[sqrs[ri * 9 + ci]][k1],  sqrtopos[sqrs[ri * 9 + ci]][k2]););
		      if (ri * 9 + ci != sqrtopos[sqrs[ri * 9 + ci]][k1] &&  
			  ri * 9 + ci != sqrtopos[sqrs[ri * 9 + ci]][k2] &&
			  countbits[cnb[sqrtopos[sqrs[ri * 9 + ci]][k1]] & 0x1ff] <= 3 &&
			  countbits[cnb[sqrtopos[sqrs[ri * 9 + ci]][k2]] & 0x1ff] <= 3)
		      {
			DEBUG4(printf("Looks good - "););
			int mask = cnb[ri * 9 + ci] | cnb[sqrtopos[sqrs[ri * 9 + ci]][k1]] | cnb[sqrtopos[sqrs[ri * 9 + ci]][k2]];
			if (countbits[mask] == 3)
			{
			  mask = (~mask) & 0x3ff;
			  for (int k3 = 0; k3 < 9; k3++)
			  {
			    if (k3 != sqrp[ri * 9 + ci] && k3 != k1 && k3 != k2)
			    {
			  DEBUG4(printf("Found Triple in sqr at %d/%d, %d/%d and %d/%d\n", ci, ri, 
					sqrtopos[sqrs[ri * 9 + ci]][k1] % 9, sqrtopos[sqrs[ri * 9 + ci]][k1] / 9, 
					sqrtopos[sqrs[ri * 9 + ci]][k2] % 9, sqrtopos[sqrs[ri * 9 + ci]][k2] / 9););
			      DEBUG4(printf("  Masking %d/%d ", sqrtopos[sqrs[ri * 9 + ci]][k3] % 9, sqrtopos[sqrs[ri * 9 + ci]][k3] / 9););
			      fnd += (cnb[sqrtopos[sqrs[ri * 9 + ci]][k3]] & (~mask & 0x1ff)) ? 1 : 0;
			      DEBUG4(printf("FND:%d\n", fnd););
			      cnb[sqrtopos[sqrs[ri * 9 + ci]][k3]] &= mask;
			    }
			  }
			}
		      }
		      DEBUG4(printf("\n"););
		    }
		  }
		}
		break;
	      default:
		printf("Wrong strategy type %d\n", type);
		continue;
		break;
	      }
	    }
	  }
	}
      }
    }
    DEBUG3(print_board(board););  
    DEBUG3(print_line(board););  
    DEBUG3(print_candidates(cnb););  
}

#if DEBUG | DEBUG1

main()
{
  for (int i = 0; i < 81; i++) board[i] = isdigit(board[i]) ? board[i] : '-'; /* Convert from other delimiters to dash */

  solve_board(board);

  exit(0);
}

#endif
