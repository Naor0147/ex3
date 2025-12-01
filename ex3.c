/******************
Name:Naor Biton
ID:329218416
Assignment:ex2
*******************/

#include <stdio.h>

#ifndef ROWS
#define ROWS 4
#endif

#ifndef COLS
#define COLS 4
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

//reomved unnecesery int parameters 
int isColumnFull(char board[][COLS], int coulmIndex);

int isBoardFull(char board[][COLS], int rows, int columns);

int isInBounds(int, int, int, int);

/* Return index of row where token will land, or -1 if column full removed int unccery*/
int getFreeRow(char board[][COLS], int rows, int coulmIndex);

/* Place token in column (0-based). Return row index or -1 if illegal removed int  */
int makeMove(char board[][COLS], int rows, int coulmIndex, char playerToken);

/// changed the function parmeters removed uncecery int
int checkVictory(char board[][COLS], int rows, int columns, int numberConnect, char playerToken);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) and reomved int parameter */
int humanChoose(char board[][COLS], int columns);

/* Computer
 removed unnecesery char  parameter */

int computerChoose(char board[][COLS], int rows, int columns, char token);

void runConnectFour(char board[][COLS], int rows, int columns, int, int);

void initBoard(char board[][COLS], int rows, int columns);

void printBoard(char board[][COLS], int rows, int cols);

int getPlayerType(int);

/// custom
int calculateColumnPriority(int columns, int priorityIndex);

/// check for each turn if the game should continue or end
int executeTurn(char board[][COLS], int rows, int columns, int pType, char playerToken);
/// helps to make the games loop less packed
void performMove(char board[][COLS], int rows, int columns, int pType, char playerToken);

// print victory message
void PrintVictoryMessage(char playerToken);

// computer helper functions
int computerHelper(char board[][COLS], int rows, int columns, char token, int priorityOrder[COLS], int customConnectN);

// custom check
int countMatches(char board[][COLS], int rows, int cols, int startRow, int startCol, int stepRow, int stepCol, char token);// count matches in a specific direction
int checkVictoryFromCell(char board[][COLS], int rows, int columns, int row, int col, int targetLength, char token);// check victory from a specific cell
int checkNextMoveWins(char board[][COLS], int rows, int columns, char playerToken, int priorityOrder[COLS], int targetLength);// check if you move ceritan place youwin 

int main()
{
    char board[ROWS][COLS];
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

void printBoard(char board[][COLS], int rows, int cols)
{
    printf("\n");
    for (int r = 0; r < rows; r++)
    {
        printf("|");
        for (int c = 0; c < cols; c++)
        {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++)
    {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}

int getPlayerType(int playerNumber)
{
    char ch;
    while (1)
    {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1)
        {
            printf("Input error. Try again.\n");
            while (getchar() != '\n')
                ; // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H')
            return HUMAN;
        if (ch == 'c' || ch == 'C')
            return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n')
            ; // clear rest of input
    }
}

// fill the board with empty
void initBoard(char board[][COLS], int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            // using Empty macro to fill the the board
            board[i][j] = EMPTY;
        }
    }
}

// main game loop
void runConnectFour(char board[][COLS], int rows, int columns, int p1Type, int p2Type)
{
    int gameRunneing = 1; // flag to keep the game running
    int turn = 1;         // keeps track of the turn number and which player's turn it is
    while (gameRunneing)
    {
        if (turn % 2 == 1)
        {
            gameRunneing = executeTurn(board, rows, columns, p1Type, TOKEN_P1);
        }
        else
        {
            gameRunneing = executeTurn(board, rows, columns, p2Type, TOKEN_P2);
        }

        turn++; // increment turn to switch players
    }
}

int executeTurn(char board[][COLS], int rows, int columns, int pType, char playerToken)
{
    // check if board is full
    if (isBoardFull(board, rows, columns))
    {
        printf("Board full and no winner. It's a tie!\n");
        return 0;
    }

    // prints player number and player token
    if (playerToken == TOKEN_P1)
    {
        printf("Player 1 (%c) turn.\n", playerToken);
    }
    else if (playerToken == TOKEN_P2)
    {
        printf("Player 2 (%c) turn.\n", playerToken);
    }

    // helper function to handle the game logic for each turn
    performMove(board, rows, columns, pType, playerToken);
    printBoard(board, rows, columns);

    if (checkVictory(board, rows, columns, CONNECT_N, playerToken))
    {
        PrintVictoryMessage(playerToken);
        return 0;
    }
    return 1;
}

void performMove(char board[][COLS], int rows, int columns, int pType, char playerToken)
{
    int playeChosenColumn;

    if (pType == HUMAN)
    {
        playeChosenColumn = humanChoose(board, columns);
        makeMove(board, rows, playeChosenColumn, playerToken);
    }
    else if (pType == COMPUTER)
    {

        playeChosenColumn = computerChoose(board, rows, columns, playerToken);
        makeMove(board, rows, playeChosenColumn, playerToken);
        printf("Computer chose column %d\n", playeChosenColumn + 1);
    }
}

int isColumnFull(char board[][COLS], int coulmIndex)
{
    return board[0][coulmIndex] != EMPTY; // return 0 isn't full and 1 is it is full
}

int isBoardFull(char board[][COLS], int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (board[i][j] == EMPTY)
            {
                return 0; // board is not full
            }
        }
    }

    return 1; // board is full
}

int isInBounds(int rows, int columns, int desiredRow, int desiredColumn)
{
    // makes sure the desired row and column are in bounds
    if (desiredRow >= 0 && desiredRow < rows && desiredColumn >= 0 && desiredColumn < columns)
    {
        return 1;
    }
    return 0;
}

// returns the free row index in the given column or -1 if the column is full
int getFreeRow(char board[][COLS], int rows, int coulmIndex)
{
    if (!isColumnFull(board, coulmIndex))
    {
        for (int i = 1; i <= rows; i++)
        {
            if (board[rows - i][coulmIndex] == EMPTY)
            {
                return rows - i;
            }
        }
    }
    return -1;
}

int makeMove(char board[][COLS], int rows, int coulmIndex, char playerToken)
{
    int rowIndex = getFreeRow(board, rows, coulmIndex);
    if (rowIndex == -1)
    {
        return -1; // illegal move
    }
    board[rowIndex][coulmIndex] = playerToken; // enter the player token in player desierd column

    return 0;
}

// checks for victory in all directions+print victory message
// Checks for victory by scanning the board and using the efficient helper
int checkVictory(char board[][COLS], int rows, int columns, int numberConnect, char playerToken)
{
    // Iterate through every cell in the board
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            // If we find a token belonging to the player, check if it forms a sequence
            if (board[i][j] == playerToken)
            {
                // checkVictoryFromCell checks all 4 directions from this specific point
                // effectively replacing all your old manual loops
                if (checkVictoryFromCell(board, rows, columns, i, j, numberConnect, playerToken))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void PrintVictoryMessage(char playerToken)
{
    if (TOKEN_P1 == playerToken)
    {
        printf("Player 1 (%c) wins!\n", playerToken);
    }
    else if (TOKEN_P2 == playerToken)
    {
        printf("Player 2 (%c) wins!\n", playerToken);
    }
}

int humanChoose(char board[][COLS], int columns)
{
    int choice;
    int isNumber;

    while (1)
    {
        printf("Enter column (1-%d): ", columns);
        isNumber = scanf("%d", &choice);

        // Check if input was not a number
        if (isNumber != 1)
        {
            printf("Invalid input. Enter a number.\n");
            // Clear input buffer from invalid characters
            while (getchar() != '\n')
                ;
            continue;
        }

        // Check if the number is with in valid range
        if (choice < 1 || choice > columns)
        {
            printf("Invalid column. Choose between 1 and %d.\n", columns);
            continue;
        }

        // Check if the selected column is full
        if (isColumnFull(board, choice - 1))
        {
            printf("Column %d is full. Choose another column.\n", choice);
            continue;
        }

        // If we reach here, the choice is valid so i return the column index
        return choice - 1;
    }

    // computer logic
}

int computerChoose(char board[][COLS], int rows, int columns, char token)
{
    int possibleMove;
    int priorityOrder[COLS];
    for (int i = 0; i < columns; i++)
    {
        priorityOrder[i] = calculateColumnPriority(columns, i);
    }
    possibleMove = computerHelper(board, rows, columns, token, priorityOrder, CONNECT_N);

    if (possibleMove != -1)
    {
        return possibleMove;
    }
    char opponentToken = (token == TOKEN_P1) ? TOKEN_P2 : TOKEN_P1; // gets the opponent token
    possibleMove = computerHelper(board, rows, columns, opponentToken, priorityOrder, CONNECT_N);
    if (possibleMove != -1)
    {
        return possibleMove;
    }
    if (CONNECT_N - 1 > 1)
    {
        possibleMove = computerHelper(board, rows, columns, token, priorityOrder, CONNECT_N - 1);
        if (possibleMove != -1)
        {
            return possibleMove;
        }
        possibleMove = computerHelper(board, rows, columns, opponentToken, priorityOrder, CONNECT_N - 1);
        if (possibleMove != -1)
        {
            return possibleMove;
        }
        // If no strategic move found, pick the first available column based on priority order
        for (int i = 0; i < columns; i++)
        {
            int col = priorityOrder[i];
            if (!isColumnFull(board, col))
            {
                return col;
            }
        }
    }
    return 0; // default return value, should not reach here if the board is not full
}

int computerHelper(char board[][COLS], int rows, int columns, char token, int priorityOrder[COLS], int customConnectN)
{
    int possibleMove = -1; // initialize to -1 to indicate no move found

    possibleMove = checkNextMoveWins(board, rows, columns, token, priorityOrder, customConnectN);
    if (possibleMove != -1)
    {
        return possibleMove;
    }

    return possibleMove;
}

// Helper: Counts consecutive tokens in a specific direction (deltaRow, deltaCol)
int countMatches(char board[][COLS], int rows, int cols, int startRow, int startCol, int stepRow, int stepCol, char token)
{
    int count = 0;
    int currentRow = startRow + stepRow;
    int currentCol = startCol + stepCol;

    // Keep moving in the direction while inside bounds and tokens match
    while (isInBounds(rows, cols, currentRow, currentCol) &&
           board[currentRow][currentCol] == token)
    {
        count++;
        currentRow += stepRow;
        currentCol += stepCol;
    }
    return count;
}
// Checks if placing a token at [row][col] creates a winning sequence
int checkVictoryFromCell(char board[][COLS], int rows, int columns, int row, int col, int targetLength, char token)
{
    // Directions: 1.right  2.down  3.down right  4.down-Left , their opposites will be checked in the loop
    int stepRows[] = {0, 1, 1, 1};
    int stepCols[] = {1, 0, 1, -1};
    int totalDirections = 4;

    for (int i = 0; i < totalDirections; i++)
    {
        int dirctionRow = stepRows[i];
        int dirctionCol = stepCols[i];

        // how many tokens are in line sequence
        int totalSequence = 1;

        // Add matches from the right or up direction
        totalSequence += countMatches(board, rows, columns, row, col, dirctionRow, dirctionCol, token);

        // Add matches from the left or down direction
        totalSequence += countMatches(board, rows, columns, row, col, -dirctionRow, -dirctionCol, token);

        // check if the total sequence is enough for victory
        if (totalSequence >= targetLength)
        {
            return 1;
        }
    }
    return 0;
}
int checkNextMoveWins(char board[][COLS], int rows, int columns, char playerToken, int priorityOrder[COLS], int targetLength)
{
    // Iterate through columns based on the strategy (center columns first)
    for (int i = 0; i < columns; i++)
    {
        int colIndex = priorityOrder[i];
        int rowIndex = getFreeRow(board, rows, colIndex);

        // Skip if the column is full
        if (rowIndex == -1)
            continue;

        // I simulate the move
        board[rowIndex][colIndex] = playerToken;

        //  I check if this move wins the game
        int isVictory = checkVictoryFromCell(board, rows, columns, rowIndex, colIndex, targetLength, playerToken);

        //  Undo the move (restore board state)
        board[rowIndex][colIndex] = EMPTY;

        if (isVictory)
        {
            return colIndex; // Found a winning column
        }
    }

    return -1; // No winning move found
}

int calculateColumnPriority(int columns, int priorityIndex)
{
    // calcualte the proity
    int center = (columns - 1) / 2;

    if (priorityIndex == 0)
        return center;

    int offset = (priorityIndex + 1) / 2;

    if (columns % 2 == 0) // Even number
    {

        if (priorityIndex % 2 != 0)
            return center + offset;
        else
            return center - offset;
    }
    else // Odd number
    {

        if (priorityIndex % 2 != 0)
            return center - offset;
        else
            return center + offset;
    }
}