#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

int isColumnFull(char board[][COLS], int rows, int columns, int coulmIndex);

int isBoardFull(char board[][COLS], int rows, int columns);

int isInBounds(int, int, int, int);

/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char board[][COLS], int rows, int columns, int coulmIndex);

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char board[][COLS], int rows, int columns, int coulmIndex, char playerToken);

/// changed the function parmeters removed uncecery int
int checkVictory(char board[][COLS], int rows, int columns, char playerToken);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char board[][COLS], int rows, int columns);

/* Computer*/
int computerChoose(char board[][COLS], int rows, int columns, char, char);

void runConnectFour(char board[][COLS], int rows, int columns, int, int);

void initBoard(char board[][COLS], int rows, int columns);

void printBoard(char board[][COLS], int rows, int cols);

int getPlayerType(int);

/// custom
int areTheSameValueAndNotEmpty(char board[][COLS], int row, int col, int startRow, int startCol, int deltaRow, int deltaCol, char playerToken);
int gameTurnHelper(char board[][COLS], int rows, int columns, int pType, char playerToken);
void gameHelper(char board[][COLS], int rows, int columns, int pType, char playerToken);

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

void runConnectFour(char board[][COLS], int rows, int columns, int p1Type, int p2Type)
{
    int gameRunneing = 1;
    int turn = 1;
    while (gameRunneing)
    {
        if (turn % 2 == 1)
        {
            gameRunneing = gameTurnHelper(board, rows, columns, p1Type, TOKEN_P1);
        }
        else
        {
            gameRunneing = gameTurnHelper(board, rows, columns, p2Type, TOKEN_P2);
        }

        turn++;
    }
}

int gameTurnHelper(char board[][COLS], int rows, int columns, int pType, char playerToken)
{
    if (isBoardFull(board, rows, columns))
    {
        printf("Board full and no winner. It's a tie!\n");
        return 0;
    }

    // makeMove(board, rows, columns, TOKEN_P1);
    gameHelper(board, rows, columns, pType, playerToken);
    printBoard(board, rows, columns);

    if (checkVictory(board, rows, columns, playerToken))
    {
        return 0;
    }
    return 1;
}

void gameHelper(char board[][COLS], int rows, int columns, int pType, char playerToken)
{
    int playeChosenColumn;
    int rowIndex;
    if (pType == HUMAN)
    {
        playeChosenColumn = humanChoose(board, rows, columns);
        rowIndex = makeMove(board, rows, columns, playeChosenColumn, playerToken);
    }
    else if (pType == COMPUTER)
    {
        // playeChosenColumn= computerChoose();
    }
}

int isColumnFull(char board[][COLS], int rows, int columns, int coulmIndex)
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

int getFreeRow(char board[][COLS], int rows, int columns, int coulmIndex)
{
    if (!isColumnFull(board, rows, columns, coulmIndex))
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

int makeMove(char board[][COLS], int rows, int columns, int coulmIndex, char playerToken)
{
    int rowIndex = getFreeRow(board, rows, columns, coulmIndex);
    if (rowIndex == -1)
    {
        /* code */
    }
    board[rowIndex][coulmIndex] = playerToken;

    return rowIndex;
}

// neeed to fill
int checkVictory(char board[][COLS], int rows, int columns, char playerToken)
{
    // horizontal check
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j <= columns - CONNECT_N; j++) // checks from index to n after if they are the same and not empty
        {
            if (areTheSameValueAndNotEmpty(board, rows, columns, i, j, 0, CONNECT_N - 1, playerToken))
            {
                printf("Player %c wins!\n", playerToken);
                return 1;
            }
        }
    }

    // vertical check
    for (int i = 0; i <= rows - CONNECT_N; i++)
    {
        for (int j = 0; j < columns; j++) // checks from index to n after if they are the same and not empty
        {
            if (areTheSameValueAndNotEmpty(board, rows, columns, i, j, CONNECT_N - 1, 0, playerToken))
            {
                printf("Player %c wins!\n", playerToken);
                return 1;
            }
        }
    }

    // diagonal left check

    // diagonal right check
    return 0;
}

int areTheSameValueAndNotEmpty(char board[][COLS], int row, int col, int startRow, int startCol, int deltaRow, int deltaCol, char playerToken)
{
    if (!isInBounds(ROWS, COLS, startRow + deltaRow, startCol + deltaCol))
    {
        return 0;
    }
    for (int i = 0; i <= deltaRow; i++)
    {
        for (int j = 0; j <= deltaCol; j++)
        {
            if (board[startRow + i][startCol + j] != playerToken) // checks if the value is the same as the player token if not return 0
            {
                return 0;
            }
        }
    }

    return 1;
}

int humanChoose(char board[][COLS], int rows, int columns)
{
    int choice;
    int n;

    while (1)
    {
        printf("Enter column (1-%d): ", columns);

        // Try to read an integer from input
        n = scanf("%d", &choice);

        // Check if input was not a number
        if (n != 1)
        {
            printf("Invalid input. Enter a number.\n");
            // Clear input buffer from invalid characters
            while (getchar() != '\n')
                ;
            continue;
        }

        // Check if the number is within valid range
        if (choice < 1 || choice > columns)
        {
            printf("Invalid column. Choose between 1 and %d.\n", columns);
            continue;
        }

        // Check if the selected column is full
        // Note: passing choice - 1 because array is 0-based
        if (isColumnFull(board, rows, columns, choice - 1))
        {
            printf("Column %d is full. Choose another column.\n", choice);
            continue;
        }

        // If we got here, the move is valid. Return 0-based index.
        return choice - 1;
    }
}
