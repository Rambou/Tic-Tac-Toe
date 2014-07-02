/*Copyright [2014] [Bousios Nikolaos - Rambou]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

#include <stdio.h>
#include <windows.h>
#define R 5 // Rows
#define L 5 // Lines

// Define the OS which gonna be compiled
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#endif

int main() {
    int gameOver = -1,
        replay = 1, //
        line, row; // Given Rows and lines by players
    char symbolBoard[R][L];
    char players[2][20]; // Store Player names
    char symbols[2] = {'+','-'}; // Store symbols to be used in game (+,-), player 1 will have always + and - for player 2

    // Show menu with game info
    char result;
    do {
        gameIntro(&result);
    } while(result!='s'&& result!='S');

    // Initialize game
    createPlayers(players);
    printPlayers(players);

    do { //GamePlay
        createSymbolBoard(symbolBoard);
        printGameBoard(symbolBoard);

        do {
            // Loop for two players
            for(int i=0; i<2; i++) {
                // Who's playing - get Coordinates
                printf("\n%s you play with symbol %c\n", players[i], symbols[i]);
                getCoordinates(&line,&row);

                // Check if position on board is empty else get Coordinates again
                while( ((symbolBoard[line][row] == ' ') ? 1 : -1) == -1) {
                    system("cls");
                    printGameBoard(symbolBoard);
                    printf("\n%s you play with symbol %c\n", players[i], symbols[i]);
                    printf("\nAlready in use!!!\n");
                    printf("\a"); // generate sound
                    getCoordinates(&line,&row);
                }

                // Place the Symbol on board
                symbolBoard[line][row] = symbols[i];
                system("cls");
                printGameBoard(symbolBoard);

                // Check winner
                int res = gameWon(symbolBoard,line,row);
                if( res == 1) {
                    printf("\nAnd the Winner is %s !!!\n", players[i]);
                    printf("\a"); // generate sound
                    gameOver = 1;
                    break;
                } else if(res == 0) {
                    printf("\nWe have no winner :(\n");
                    printf("\a"); // generate sound
                    gameOver = 1;
                    break;
                }

            }//ends players loop
        } while(gameOver == -1);

        printf("\nPress any key to continue...");
        setConsoleBuffered(0);
        getchar();
        getchar();
        setConsoleBuffered(1);

        do {
            system("cls");
            printf("\nWould you like to replay? (1.Yes 2.No): ");
            scanf("%d",&replay);
        } while(replay < 1 || replay > 2);

        if(replay ==1) gameOver = -1;
        system("cls");
    } while(replay == 1); //play another game

}

// Get coordinates of players choice
void getCoordinates(int *line, int *row) {
    do {
        printf("Give coordinates [0 to 4] (e.x. 4,2): ");
        scanf("%d,%d",row,line);
    } while( *row < 0 || *row > 4 || *line < 0 || *line > 4 );
}

// Create players
void createPlayers(char paiktes[2][20]) {
    for(int i=0; i<2; i++) {
        printf("Give Player %d name: ",i+1);
        scanf("%s", paiktes[i]);
    }
}

// Generate game board, initialize array
void createSymbolBoard(char array[R][L]) {
    for ( int i = 0 ; i<R ; i++ )
        for ( int j = 0 ; j<L ; j++ )
            array[i][j] = ' ';
}

// Print Players
void printPlayers(char paiktes[2][20]) {
    system("cls");

    for(int i=0; i<2; i++) {
        printf("Player %d | Name: %s | Has symbol: %c\n", i+1, paiktes[i], (i==0)? '+': '-' );
    }

    printf("\nPress any key to continue...");
    setConsoleBuffered(0);
    getchar();
    getchar();
    setConsoleBuffered(1);
    system("cls");
}

//methodos gia emfanisi tou game board
void printGameBoard(char array[R][L]) {
    printf("\n\n\n");
    printf("\n\t\t\t    0\t      1\t        2\t  3\t  4\t");
    for (int i=0; i<R; i++ ) {
        printf("\n \t\t\t----------------------------------------------");
        printf("\n \t\t\t|\t  |\t   |\t     |\t     |\t     |");
        printf("\n \t\t%d\t",i);

        for (int j=0; j<L; j++ ) {
            printf( "\t%c", array[i][j] );
        }

        printf("\n \t\t\t|\t  |\t   |\t     |\t     |\t     |");
    }
    printf("\n\t\t\t----------------------------------------------\n\n\n");
}


int gameWon(char board[R][L], int line, int row) {
    int i,j,
        emptyLeft = R*L,
        symbLine=0,
        symbRow=0,
        symbDiagonal1=0,
        symbDiagonal2=0;
    char playerSymbol = board[line][row];

    // Check for tic tac toe in the specific line or the last inserted symbol
    for(j=0; j<L; j++) {
        if(board[line][j] == playerSymbol) {
            symbLine++;
        }
    }


    // Check for tic tac toe in the specific row or the last inserted symbol
    for(i=0; i<R; i++) {
        if(board[i][row] == playerSymbol) {
            symbRow++;
        }
    }

    // Check for diagonal
    for(i=0; i<R; i++)
        for(j=0; j<L; j++)
            if(i==j && board[i][j] == playerSymbol) {
                symbDiagonal1++;
            }

    // Check for the other diagonal
    for(j=0; j<L; j++)
        if(board[j][L-j-1] == playerSymbol) {
            symbDiagonal2++;
        }

    // Count for free remaining slots
    for(i=0; i<R; i++)
        for(j=0; j<L; j++)
            if(board[i][j] != ' ')
                emptyLeft--;

    if(emptyLeft == 0 ) return 0;

    return (symbLine == 5 || symbRow == 5 || symbDiagonal1 == 5 || symbDiagonal2 == 5) ? 1 : -1;
}

// Basically, we reset the console to unbuffered
void setConsoleBuffered(int boolean) {
#ifdef  OS_WIN
    DWORD        mode;
    HANDLE       hstdin;
    hstdin = GetStdHandle( STD_INPUT_HANDLE );
    GetConsoleMode( hstdin, &mode );
    SetConsoleMode( hstdin, 0 );

    if(boolean)
        // Reset to normal input
        SetConsoleMode(hstdin, mode);
    else
        // Reset to unbuffered mode
        SetConsoleMode( hstdin, 0 );
#endif // OS_WIN
}

void gameIntro(char *result) {
    printf("*****************************************");
    printf("\n*\t\tMade by Rambou\t\t*");
    printf("\n*\tGoal of the game if to win\t*");
    printf("\n*\tthe know game of tic-tac-toe.\t*");
    printf("\n*\t\t\t\t\t*");
    printf("\n*****************************************");
    printf("\n\nPress S, to start game.\n");

    // set console to unbuffered to read
    setConsoleBuffered(0);
    *result = getchar();
    // set console to buffer to clear
    setConsoleBuffered(1);
    system("cls");
}
