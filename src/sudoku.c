/*
 * Created by Matt Kreul on 7/3/21.
 */

#include "../lib/sudoku.h"

/**
 * This is a sudoku solving algorithm that uses a bruteforce, backtracking method.  It will run through the board until
 * it reaches the end, checking each of the empty spaces for valid numbers.  If a valid number is found, then it will
 * go to the next unsolved index.  After each tile is solved, it will add that index's row and column to a stack.  If
 * the algorithm reaches a tile in which no numbers work, then it will go back to the previously solved tile and try
 * another number.
 *
 * Splint command:
 *      splint src/sudoku.c -mustfreefresh -usedef -compmempass
 *
 * TODO: fix the algorithm to return the initial board and print a message if the given board is not a valid sudoku.
 * TODO: If one solution is found, check to see if there is another valid sudoku.
 *
 * @param board
 *  board to solve
 * @return
 *  solved board
 */

/*@null@*/ int** solve(int** board) {
    /*@out@*/ int** returnBoard;
    Stack * previousSolvedIndex;
    Index * tempIndex;
    int i = 0, j = 0, k = 1;
    printf("Beginning to solve board\n");

    /* create a copy of the sudoku board*/
     returnBoard = copyBoard(board);
     if (returnBoard == NULL){
         fprintf(stderr, "\n\nreturnBoard did not init\n\n");
         free(returnBoard);
         return NULL;
     }

    /*TODO: add check here to make sure sudoku board is valid*/

    /* initialize the indices and counter*/

    /* initialize the previously solved indices*/
    previousSolvedIndex = createStack(sizeof(Index ));
    if(previousSolvedIndex == NULL){
        fprintf(stderr, "\n\nstack did not init\n\n");
        /* this is returning a number to determine if it freed the stack or not... do not need it though */
        /*(void) freeStack(previousSolvedIndex);*/
        free(returnBoard);
        return NULL;
    }

    while (i < 9) {
        /* reset the row counter */
        j = 0;
        while (j < 9) {
            /* if this current box has not been solved*/
            if (returnBoard[i][j] == 0) {
                /* Check to see if k is a valid choice*/
                if (validMove(i, j, k, returnBoard) == 1) {
                    /* if it is a valid choice then save this index as the last "correct" tile*/
                    tempIndex = (Index *) malloc(sizeof(Index));
                    if (tempIndex == NULL){
                        fprintf(stderr, "\n\ntempIndex Null\n\n");
                        free(tempIndex);
                        free(returnBoard);
                        /*(void) freeStack(previousSolvedIndex);*/
                        free(previousSolvedIndex->items);
                        free(previousSolvedIndex);
                        return NULL;
                    }
                    else{
                        tempIndex->i = i;
                        tempIndex->j = j;
                        /*TODO: this is returning a boolean but not used at all*/
                        (void) push(tempIndex, previousSolvedIndex);
                        /* set k to the index*/
                        returnBoard[i][j] = k;
                        /* move to the next tile and set k back to 1*/
                        j++;
                        k = 1;
                    }

                    free(tempIndex);

                }
                    /* otherwise, we need to check the next number at that particular index*/
                else {
                    k++;
                    /* if k is greater than 9 now then we have looked through all of the possibilities and need to*/
                    /* backtrack our current solution*/
                    /* if the previous value was 9 then it will continue to backtrack until it can try different*/
                    /* numbers*/
                    while (k > 9) {
                        /*go back to previously "solved" indices*/
                        /*i = row of prevSolvedBox*/
                        /*j = col of prevSolvedBox*/
                        if(previousSolvedIndex->top < 0){
                            printf("Given board has no solution");
                            /*(void) freeStack(previousSolvedIndex);*/
                            free(previousSolvedIndex->items);
                            free(previousSolvedIndex);
                            return returnBoard;
                        }
                        tempIndex = (Index *) malloc(sizeof(Index));
                        if (tempIndex == NULL){
                            fprintf(stderr, "\n\ntempIndex Null\n\n");
                            free(tempIndex);
                            free(returnBoard);
                            /*(void) freeStack(previousSolvedIndex);*/
                            free(previousSolvedIndex->items);
                            free(previousSolvedIndex);
                            return NULL;
                        }
                        else{
                            (void) pop(tempIndex, previousSolvedIndex);
                            i = tempIndex->i;
                            j = tempIndex->j;
                            k = returnBoard[i][j] + 1;
                            /* reset this value to 0 so that we can check it again*/
                            returnBoard[i][j] = 0;
                        }
                        free(tempIndex);
                    }
                }
            }
            /* otherwise go to the next box and start at k = 1*/
            else {
                j++;
                k = 1;
            }
        }
        /*once we've gotten past all of the columns and "solved them"*/
        i++;
    }
    /* Don't forget to free the memory you allocated!*/
    /*(void) freeStack(previousSolvedIndex);*/
    free(previousSolvedIndex->items);
    free(previousSolvedIndex);
    return returnBoard;
}

/**
 * Checks to see if the given is a valid move.  First it checks the square area of the current box, then it will check
 * the column and row.  If there is not a valid move in any of these then it will return a 0.  Otherwise, it will return
 * a 1.
 *
 * @param i
 *  i index of the board
 * @param j
 *  j index of the board
 * @param k
 *  number to check
 * @param board
 *  current board
 * @return
 *  1 if move is valid, 0 if not
 */
static int validMove(int i, int j, int k, int** board){
    int row, col, l, m;
    /* This figures out which "square" the algorithm needs to check*/
    row = ((i) / 3 + 1) * 3;
    col = ((j) / 3 + 1) * 3;
    /*Check the square*/
    for (l = row - 3; l < row; l++) {
        for ( m = col - 3; m < col; m++) {
            if (i == l && j == m) m++;
            else if (board[l][m] == k) {
                return 0;
            }
        }
    }
    /*check the column*/
    for ( l = 0; l < 9; l++) {
        /*if(l == j){
            l++;
        }*/
        if (board[i][l] == k) {
            return 0;
        }
    }
    /*check the row*/
    for ( m = 0; m < 9; m++) {
        /*if(m == i){
            m++;
        }*/
        if (board[m][j] == k) {
            return 0;
        }
    }
    /*return true if it passes all of the tests*/
    return 1;
}

/**
 * Simple script that prints out a given board in a nice formatted way.
 *
 * @param board
 *  board to print out
 */
void printBoard(int** board){
    /* initialize variables */
    int i,j,k,l;

    printf("-------------------------------------\n");
    l = 1;
    for ( i = 0; i < 9; i++){ /*: originalBoard) {*/
        printf("| ");
        k = 1;
        for ( j = 0; j < 9; j++){ /*: i) {*/
            if (board[i][j] < 1) printf(" _ ");
            else printf(" %d ", board[i][j]);
            if (k % 3 == 0) printf(" | ");
            k++;
        }
        printf("\n");
        if (l % 3 == 0) printf("-------------------------------------\n");
        l++;
    }
}

/**
 * Copies the given board to a new board so as to not overwrite the original board.
 *
 * @param board
 *  board to copy
 * @return
 *  copied board
 */
/*@null@*/static int** copyBoard(/*@null@*/ int** board){
    /* allocate memory for the board*/
    /*@out@*/int** toRet;
    int i, j;
    toRet = (int**) malloc(sizeof(int*) * 9);
    if(toRet == NULL){
        fprintf(stderr, "\n\ntoRet is Null\n\n");
        free(toRet);
        return NULL;
    }
    for( i = 0; i < 9; i++){
        toRet[i] = (int*) malloc(sizeof(int) * 9);
        if(toRet[i] == NULL){
            fprintf(stderr, "\n\ntoRet is Null\n\n");
            free(toRet);
            return NULL;
        }
    }

    if (board == NULL){
        fprintf(stderr, "\n\nboard is Null\n\n");
        for(i = 0; i < 9; i++){
            free(toRet[i]);
        }
        free(toRet);
        return NULL;
    }

/*    printf("Malloced board\n");*/

    /*copy over the values from the original board to the new one*/
    for( i = 0; i < 9; i++){
        for ( j = 0; j < 9; ++j) {
            toRet[i][j] = board[i][j];
        }
    }

    /* return the new board*/
    return toRet;
}

/**
 * This will build a board from a text file.  The file format must be 9 numbers across with spaces in between each
 * number.  For an empty space, the number in the file must be a 0.  Each row is deliminated by a newline character.
 * Once the algorithm parses through the text file, it will save it as a int** and return it.
 *
 * @param filename
 *  name of file and path
 * @return
 *  int** of the sudoku board
 */
/*@null@*/ int** buildBoard(char * filename){

    FILE * file;
    /*@out@*/ int** toRet;
    int i, j, k, temp;
    /* creates a buffer for the line to be read*/
    char line[256];
    char* token;
    const char* delim;


    /* create the return int***/
    toRet = (int**) malloc((size_t) sizeof(int*) * 9);
    if(toRet == NULL){
        fprintf(stderr, "\n\ntoRet is Null\n\n");
        free(toRet);
        return NULL;
    }
    for( i = 0; i < 9; i++){
        toRet[i] = (int*) malloc((size_t) sizeof(int) * 9);
        if(toRet[i] == NULL){
            fprintf(stderr, "\n\ntoRet is Null\n\n");
            free(toRet);
            return NULL;
        }
    }
    i = 0;
    /* open the file*/
    file = fopen(filename, "r");
    if(file == NULL){
        fprintf(stderr, "\n\n OPEN FILE ERROR \n\n");
        //        fclose(file);
        for(k = 0; k < 9; k++){
            free(toRet[k]);
        }
        free(toRet);
        return NULL;
    }
    /* while there is still a line in the file OR it reaches 9 lines*/
    while((fgets(line, 100, file) !=NULL) && i < 9){
        j = 0;
        /* run through the file*/
        delim = " " ;
        token = strtok(line, delim);
        /* if the token reaches the end of the line then it will stop*/
        while(token != NULL && j < 9){
            /* change the string into an integer*/
            temp = atoi(token);
            toRet[i][j] = temp;
            token = strtok(NULL, delim);
            j++;
        }
        i++;
    }
    /* close the file*/
    (void) fclose(file);
    return (int**) toRet;
}

/**
 * This is an algorithm that will verify that the given solution is valid.
 *
 * @param board
 * @return
 */
int isSolution(int** board){
    /* TODO: SERIOUSLY reduce runtime... this shouldn't be hard to do. Ask if checking against known answers is ok*/
    /* TODO: fix the actual checker to not check the current space */
    int i,j;
    int row, col, l, m;
    for ( i = 0; i < 9; i++) {
        for ( j = 0; j < 9; j++) {
            /* if this is not a valid move, it is not a valid solution */
            row = ((i) / 3 + 1) * 3;
            col = ((j) / 3 + 1) * 3;
            /*Check the square*/
            for (l = row - 3; l < row; l++) {
                for ( m = col - 3; m < col; m++) {
                    if (i == l && j == m) m++;
                    else if (board[l][m] == board[i][j]) {
                        printf("invalid block at (%d,%d)\n", i, j);
                        return 0;
                    }
                }
            }
            /*check the column*/
            for ( l = 0; l < 9; l++) {
                if(l == j){
                    l++;
                }
                else if (board[i][l] == board[i][j]) {
                    printf("invalid block at (%d,%d)\n", i, j);
                    return 0;
                }
            }
            /*check the row*/
            for ( m = 0; m < 9; m++) {
                if(m == i){
                    m++;
                }
                else if (board[m][j] == board[i][j]) {
                    printf("invalid block at (%d,%d)\n", i, j);
                    return 0;
                }
            }
            /*if(validMove(i, j, board[i][j], board) == 0){
                printf("invalid block at (%d,%d)\n", i, j);
                return 0;
            }*/
        }
    }
    /* if it goes through every number without an error then it is a solution! */
    return 1;
}


/**
 * @brief 
 * 
 * @param board 
 * @return int* 
 */
int* parseIntPointer(int* board){
    int i;
    int j;
    int * toRet;
    int ** inputBoard;
    int ** returnedBoard;

    // Initialize inputBoard
    inputBoard = (int**) malloc((size_t) sizeof(int*) * 9);
    if(inputBoard == NULL){
        fprintf(stderr, "\n\ninputBoard is Null\n\n");
        free(inputBoard);
    }
    for( i = 0; i < 9; i++){
        inputBoard[i] = (int*) malloc((size_t) sizeof(int) * 9);
        if(inputBoard[i] == NULL){
            fprintf(stderr, "\n\ninputBoard is Null\n\n");
            free(inputBoard);
        }
    }

    // copy over the int* values into the int**
    i = 0;
    for(j = 0; j < 81; j++){
        inputBoard[i][j%9] = board[j];
        if (j%9==8) i++;
    }
    //print the board (for debugging purposes)
    // printf("*********************************************\n*************** Input Board *****************\n*********************************************\n\n");
    // printBoard(inputBoard);

    //now pass in our new board to be solved
    returnedBoard = solve(inputBoard);

    // //print our new board
    // printf("*********************************************\n*************** Output Board ****************\n*********************************************\n\n");
    // printBoard(returnedBoard);
    //now copy our values back into our in/out param

    toRet = (int*) malloc(sizeof(int)* 81);
    i = 0;
    for(j = 0; j < 81; j++){
        toRet[j] = returnedBoard[i][j%9];
        board[j] = returnedBoard[i][j%9];
        if (j%9==8) i++;
    }
    
    // don't forget to free our pointers :) 
    free(inputBoard);
    free(returnedBoard);

    return toRet;
}

void freeIntPointer(int* board){
    free(board);
}


/**
 *
 * @param board
 *  board to check
 * @return
 *  1 if the board is solveable or 0 if a solution cannot be found
 */
//int solvable(int** board){
//
//
//    return 1;
//}