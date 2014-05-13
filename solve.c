//
// g++ -DDEBUG -o s solve.c; ./s
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define update_board(X)     board[X] = num; \
                            col[cols[X]] = col[cols[X]] | (1 << (board[X] - (int) '0') - 1); \
                            row[rows[X]] = row[rows[X]] | (1 << (board[X] - (int) '0') - 1); \
                            sqr[sqrs[X]] = sqr[sqrs[X]] | (1 << (board[X] - (int) '0') - 1); \
                            occ[sqrs[X]] = occ[sqrs[X]] | (1 <<  sqri[X] - 1);


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
    //char board[] = "-9--3-6------7-2----48----51--5---8---7---9---3---6--42----17----8-2------6-5--9-";   // Left 44  (2151 medium)
    //char board[] = "8--6----2-4--5--1----7----3-9---4--62-------87---1--5-3----9----1--8--9-4----2--5"; // Left 56 (insane)
    // char board[] = "-6---39--5--1-----8-------7-4-2--6--7-------8--3--9-1-2-------5-----4--3--87---2-"; // Left 43 (hard)
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
    // Cell  (0-81) to what square table (0-8) sqr[]
    char sqrs[]     = "000111222000111222000111222333444555333444555333444555666777888666777888666777888";

    // board-cell (0-81) to what sqr-cell (1-9) bits for occ[]
    char sqri[]     = "123123123456456456789789789123123123456456456789789789123123123456456456789789789";

    // sqr [0-8 * 9] to cell [(0-8*9) + 0-8] table
    char sqrc[]     = {  0,  1,  2,  9, 10, 11, 18, 19, 20,
                         3,  4,  5, 12, 13, 14, 21, 22, 23,
                         6,  7,  8, 15, 16, 17, 24, 25, 26,
                        27, 28, 29, 36, 37, 38, 45, 46, 47,
                        30, 31, 32, 39, 40, 41, 48, 49, 50,
			33, 34, 35, 42, 43, 44, 51, 52, 53,
                        54, 55, 56, 63, 64, 65, 72, 73, 74,
                        57, 58, 59, 66, 67, 68, 75, 76, 77} ;  


    // Cell (0-81) to column (0-8) col[]                
    char cols[]     = "012345678012345678012345678012345678012345678012345678012345678012345678012345678";
    
    // Cell (0-81) to row table (0-8) row[]
    char rows[]     = "000000000111111111222222222333333333444444444555555555666666666777777777888888888";

    // Cell to first of two adjacent col/rows in a square
    char n1[]    = "120453786";

    // Cell to second of two adjacent col/rows in a square
    char n2[]    = "201534867";

    // Keep track of what numbers is in what column,row and square
    int col[9]; // What number in column
    int row[9]; // What number in row 
    int sqr[9]; // What number in sqr
    int occ[9]; // What cells are occupied in sqr 


    int cocc[9]; // Each number temp storage for in square status

    int i, j, k, cn, cnt, fnd, afnd, num, a, b, left, type, cell, ind, res;

    for (i = 0; i < sizeof(sqrs); i++){ sqrs[i] -= (int) '0'; }
    for (i = 0; i < sizeof(sqri); i++){ sqri[i] -= (int) '0'; }
    for (i = 0; i < sizeof(cols); i++){ cols[i] -= (int) '0'; }
    for (i = 0; i < sizeof(rows); i++){ rows[i] -= (int) '0'; }
    for (i = 0; i < sizeof(n1); i++){ n1[i] -= (int) '0'; }
    for (i = 0; i < sizeof(n2); i++){ n2[i] -= (int) '0'; }
    for (i = 0; i < sizeof(sqrc); i++)

    //    print_board(board);

    // Clear bittables
    memset(col, 0, sizeof(col));
    memset(row, 0, sizeof(row));
    memset(sqr, 0, sizeof(sqr));
    memset(occ, 0, sizeof(occ));

    // Populate bittables and count empty cells
    left = 0;
    for (i = 0; i < 81; i++)
    {
        if (board[i] != '-')
        {
            col[cols[i]] = col[cols[i]] | (1 << (board[i] - (int) '0') - 1);
            row[rows[i]] = row[rows[i]] | (1 << (board[i] - (int) '0') - 1); 
            sqr[sqrs[i]] = sqr[sqrs[i]] | (1 << (board[i] - (int) '0') - 1);
            occ[sqrs[i]] = occ[sqrs[i]] | (1 << (sqri[i] - 1));
        }
        else
        {
            left++;            
        }
    }

    type = 0;

    // Go as long as one of the analysistypes parts genrates results
    while (type < 6)
    {
        fnd  = 1; // Use a type as long as it generates results, then switch to the next
        afnd = 0; // accumulate all results over one scan of the board

#if DEBUG | DEBUG1
        printf("\nChecking with type %d\n-------------------------------", type); 
#endif
#if DEBUG & 0
        print_board(board);
#endif

        // So as long as we find stuff
        while (fnd)
        {
            fnd = 0; // found nothing yet

            // Select analysis type
            switch (type)
            {
            case 0:
                for (i = 0; i < 81; i++)
                {
                    if ( board[i] == '-' ){
#ifdef DEBUG2
                        printf("\nChecking cell %d (%d,%d) in sqr %d", i, rows[i], cols[i], sqrs[i]);
#endif
                        // Select numbers not in same row, col or sqr
                        cn = row[rows[i]] | col[cols[i]] | sqr[sqrs[i]];
#ifdef DEBUG1
                        printf("[%d-%02x]",i,cn);
#endif

                        // Count how many possible number this cell has
                        cnt = 0;
                        for (j = 0; j < 9; j++) 
                        {
                            if (! (cn & (1 << j)))
                            {
                                cnt++;
                                num = j + '0' + 1;
                            }
                        }

                        // If only one number fits the cell we have one less cell to go...
                        if (cnt == 1)
                        {
#ifdef DEBUG
                            printf("Populating cell %d (%d,%d) with %c\n", i, rows[i] + 1, cols[i] + 1, num);
#endif

                            update_board(i);

                            fnd++;  // Count the catch
                            --left; // Count the flock

#ifdef DEBUG
                            print_board(board);
#endif
#ifdef DEBUG1
                        printf("+\n");
#endif
                        }
                    }
                }
                break;
            case 1:
                for (i = 0; i < 81; i++)
                {
                    if ( board[i] == '-' ){
#ifdef DEBUG2
                        printf("\nChecking cell %d (%d,%d) in sqr %d", i, rows[i], cols[i], sqrs[i]);
#endif
#ifdef DEBUG1
                        printf(".");
#endif

                        // Numbers that has nowhere else to go (TODO: only takes full house, not obstructions by other numbers)
                        cn = (row[n1[rows[i]]] & row[n2[rows[i]]] & col[n1[cols[i]]] & col[n2[cols[i]]] & ~sqr[sqrs[i]] );
//                      printf("Comparing rows %d, %d and col %d, %d ==> %04x | ",n1[rows[i]], n2[rows[i]], n1[cols[i]], n2[cols[i]], cn);

                        cnt = 0;
                        for (j = 0; j < 9; j++) 
                        {
                            if ( cn & (1 << j))
                            {
                                cnt++;
                                num = j + '0' + 1;
                            }
                        }
 
                        if (cnt == 1)
                        {
#ifdef DEBUG
                            printf("Populating cell %d (%d,%d) with %c\n", i, rows[i] + 1, cols[i] + 1, num);
#endif
                            update_board(i);

                            fnd++;
                            left--;
                            type = 0;

#ifdef DEBUG
                          print_board(board);
#endif
#ifdef DEBUG1
                        printf("+\n");
#endif
                            break;
                        }
                    }
                }
                break;
            case 2: // Go over each square and find numbers with only one place to go
                {

		    //                    print_board(board);

                    for (i = 0; i < 81; i++)
                    {
                        if (board[i] == '-')
                        {
                            for (j = 0; j < 9; j++)
                            {
			        if ((sqr[sqrs[i]] & (1 << j)) == 0)
                                {
#ifdef DEBUG1
                        printf(".");
#endif
                                    res = occ[sqrs[i]] |
                                          ((col[n1[cols[i]]] & (1 << j)) != 0 ? 0x49 <<      n1[cols[i]] % 3 : 0) |
                                          ((col[n2[cols[i]]] & (1 << j)) != 0 ? 0x49 <<      n2[cols[i]] % 3 : 0) |
                                          ((row[n1[rows[i]]] & (1 << j)) != 0 ? 0x7  << 3 * (n1[rows[i]] % 3) : 0) |
                                          ((row[n2[rows[i]]] & (1 << j)) != 0 ? 0x7  << 3 * (n2[rows[i]] % 3) : 0) ;
                                    cn = res ^ 0x1ff;
                                    cnt = 0;

                                    for (k = 0; k < 9; k++) 
                                    {
                                        if ( cn & (1 << k))
                                        {
                                            cnt++;
                                            num = j + '0' + 1;
                                        }
                                    }
 
                                    if (cnt == 1)
                                    {
#ifdef DEBUG
                            printf("Populating cell %d (%d,%d) with %c\n", i, rows[i] + 1, cols[i] + 1, num);
#endif
                                        update_board(i);

                                        fnd++;
                                        left--;
                                        type = 0;
#ifdef DEBUG
                          print_board(board);
#endif
#ifdef DEBUG1
                        printf("+\n");
#endif
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            case 3: // Go over each row to see if a missing number has only one place to go
                for (i = 0; i < 9; i++)
                {
//printf("\nRow %d Number:", i);
                    for (j = 0; j < 9; j++) // Checkout all numbers
                    {
                        if (! (row[i] & 1 << j)) // So where does this number go?
                        {
                            cnt = 0;
                            for (k = i * 9; k < (i + 1) * 9; k++) // Checkout all cells in row
                            {
                                // Test if it is an empty cell in sqr and col not containing the number
                                if (board[k] == '-' && ! (sqr[sqrs[k]] & 1 << j) && ! (col[cols[k]] & 1 << j))
                                {
                                    cnt++;
                                    num = j + 1 + '0';
                                    ind = k;
                                }
                            }
                            if (cnt == 1)
                            {
#ifdef DEBUG
                            printf("Populating cell %d (%d,%d) with %c\n", ind, rows[ind] + 1, cols[ind] + 1, num);
#endif
                                update_board(ind);

                                fnd++;
                                left--;
                                type = 0;
#ifdef DEBUG
                          print_board(board);
#endif
#ifdef DEBUG1
                        printf("+\n");
#endif
                                break;
                            }
                        }
                    }
                }           
                break;
            case 4: // Go over each col to see if a missing number has only one place to go
                for (i = 0; i < 9; i++)
                {
//printf("\nCol %d Number:", i);
                    for (j = 0; j < 9; j++) // Checkout all numbers
                    {
                        if (! (col[i] & 1 << j)) // So where does this number go?
                        {
                            cnt = 0;
                            for (k = i; k < 81; k += 9) // Checkout all cells in col
                            {
                                // Test if it is an empty cell in sqr and col not containing the number
                                if (board[k] == '-' && ! (sqr[sqrs[k]] & 1 << j) && ! (row[rows[k]] & 1 << j))
                                {
                                    cnt++;
                                    num = j + 1 + '0';
                                    ind = k;
                                }
                            }
                            if (cnt == 1)
                            {
#ifdef DEBUG
                            printf("Populating cell %d (%d,%d) with %c\n", ind, rows[ind] + 1, cols[ind] + 1, num);
#endif
                                update_board(ind);

                                fnd++;
                                left--;
                                type = 0;
#ifdef DEBUG
                          print_board(board);
                          printf("x");
#endif
#ifdef DEBUG1
                        printf("+\n");
#endif
                                break;
                            }
                        }
                    }
                }           
                break;
            case 5: // Elimination
                for (i = 0; i < 9; i++) // Each square on board
                {

                    memset(cocc, 0x1ff, sizeof(cocc)); // All numbers anywhere, ehh...?
                    for (j = 0; j < 9; j++) // Each cell in square
                    {
                        if (board[sqrc[i * 9 + j]] == '-')
                        {
//printf("[%d/%d/%d/%d]", sqrc[i * 9 + j], rows[sqrc[i * 9 + j]], cols[sqrc[i * 9 + j]], sqrs[sqrc[i * 9 + j]]);
                            res = row[rows[sqrc[i * 9 + j]]] | 
                                  col[cols[sqrc[i * 9 + j]]] | 
                                  sqr[sqrs[sqrc[i * 9 + j]]];

                            for (k = 0; k < 9; k++) // Each number
                            {
                                if (res & (1 << k)) // Can't this number go into this square?
                                {
                                    cocc[k] &= ~(1 << j); // No, so we check it off
                                }
                            }
                        }
                        else
                        {
                            for (k = 0; k < 9; k++) // Each number
                            {
                                cocc[k] &= ~(1 << j); // No, so we check it off
                            }
                        }
                    }
#if 0
                    for (j = 0; j < 9; j++) // Check results
                    {
                        printf("Number %d in Square %d can go in cell: ", j + 1, i + 1);
                        for (k = 0; k < 9; k++)
                        {
                            if (cocc[j] & (1 << k))
                            {
                                printf("%d ", k + 1);
                            }
                        }
                        printf("\n");
                    }
#endif
                };
                
	        break;
            default:
	        break;
            }
            afnd += fnd;
        }
        type += afnd ? 0 : 1;
    }
#if DEBUG | DEBUG1
    print_board(board);
    printf("\nLeft: %d\n", left);
#endif
}

#if DEBUG | DEBUG1

main()
{
    solve_board(board);

    exit(0);
}

#endif
