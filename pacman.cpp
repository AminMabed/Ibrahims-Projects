#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main() {
    srand(time(0)); // Seeding the rand() function

    const int ROWS = 25;
    const int COLS = 25;
    char maze[ROWS][COLS];
    int i, j;
    int pelletsRemaining = 0;
    char userInput;
    int valid, r_num, previously_a_dot;
    int pacmanRow = 19, pacmanCol = 12; // Initializing pacman's position
    int ghostRow = 9, ghostCol = 17;   // Initializing ghost's position

    // Initialize maze
    for (i = 0; i < ROWS; i++) 
        for (j = 0; j < COLS; j++) 
            if (i == 0 || i == ROWS - 1 || j == 0 || j == COLS - 1)
                maze[i][j] = '#'; // Walls
            else if ((i == 3 || i == 4) && ((j >= 3 && j <= 8) || (j >= 16 && j <= 21)))
                maze[i][j] = '#'; // Two rectangles on top left and top right
            else if ((j == 4 || j == 20) && (i >= 8 && i <= 16))
                maze[i][j] = '#'; // Vertical sticks of two 'T's on left and right
            else if (i == 12 && ((j >= 5 && j <= 9) || (j >= 15 && j <= 20)))
                maze[i][j] = '#'; // Horizontal sticks of two 'T's on left and right
            else if (i == 16 && ((j >= 8 && j <= 10) || (j >= 14 && j <= 16)))
                maze[i][j] = '#'; // Box, inside which, the pacman will be at the start
            else if (i == 8 && (j >= 8 && j <= 16))
                maze[i][j] = '#'; // Horizontal line of T in the center
            else if (j == 12 && (i >= 9 && i <= 13))
                maze[i][j] = '#'; // Vertical line of T in center
            else if ((j == 8 || j == 16) && (i >= 17 && i <= 20))
                maze[i][j] = '#';
            else if (i == 21 && (j >= 8 && j <= 16))
                maze[i][j] = '#';
            else if ((i == 19 && j == 12) || (i == 9 && j == 17))
                maze[i][j] = (i == 19) ? '>' : '8';
            else {
                maze[i][j] = '.'; // Pellets
                ++pelletsRemaining;
            }
        // Playing the main game now
    do {
        for (i = 0; i < ROWS; i++)  // Printing the whole board after each move
        {
            for (j = 0; j < COLS; j++)
                cout << maze[i][j] << " ";
            cout << endl;
        }

        while (1) // Keep taking input from user till they enter a valid input i.e. w,a,s,d
        {
            valid = 0;  
            cout << endl;
            userInput = _getche(); // Taking input without making the user press "enter" everytime
            cout << "\n\n";
            switch (userInput) {
            case 'w':
            case 'a':
            case 's':
            case 'd':
                valid = 1;
                break;
            }
            if (valid)
                break;
        }

        // Moving pac-man according to user's input
    switch(userInput)
    {
        case 'w': // Moving the pacman one step up
            if(maze[pacmanRow-1][pacmanCol] != '#')
            {
                pacmanRow--;
                maze[pacmanRow+1][pacmanCol] = ' '; // leaving a space on the pacman's previous position
                if(maze[pacmanRow][pacmanCol] == '.')
                    pelletsRemaining--;
            }
            maze[pacmanRow][pacmanCol] = 'V';
            break;

        case 'a': // Moving the pacman one step left
            if(maze[pacmanRow+1][pacmanCol] != '#')
            {
                pacmanCol--;
                maze[pacmanRow][pacmanCol+1] = ' ';
                if(maze[pacmanRow][pacmanCol] == '.')
                    pelletsRemaining--;
            }
            maze[pacmanRow][pacmanCol] = '>';
            break;

        case 's': // Moving the pacman one step down
            if(maze[pacmanRow+1][pacmanCol] != '#')
            {
                pacmanRow++;
                maze[pacmanRow-1][pacmanCol] = ' ';
                if(maze[pacmanRow][pacmanCol] == '.')
                    pelletsRemaining--;
            }
            maze[pacmanRow][pacmanCol] = '^';
            break;

        case 'd': // Moving the pacman one step right
            if(maze[pacmanRow][pacmanCol+1] != '#')
            {
                pacmanCol++;
                maze[pacmanRow][pacmanCol-1] = ' ';
                if(maze[pacmanRow][pacmanCol] == '.')
                    pelletsRemaining--;
            }
            maze[pacmanRow][pacmanCol] = '<';
            break;
    }

        r_num = rand() % 4 + 1; // Generating random number for ghost's movement
        // 1 is same as 'w', 2 as 'a', 3 as 's', and 4 as 'd'

        // Moving ghost based on the random number generated
        switch (r_num) {
        case 1: // Moving the ghost one step up
            if (maze[ghostRow - 1][ghostCol] != '#') {
                ghostRow--; 
                previously_a_dot = (maze[ghostRow][ghostCol] == '.') ? 1 : 0; // Making sure that if the ghost's current position was previously a dot, then the ghost doesnt eat the pellet
                maze[ghostRow + 1][ghostCol] = (previously_a_dot) ? '.' : ' ';
            }
            maze[ghostRow][ghostCol] = '8';
            break;

        case 2: // // Moving the ghost one step left
            if (maze[ghostRow][ghostCol - 1] != '#') {
                ghostCol--; 
                previously_a_dot = (maze[ghostRow][ghostCol] == '.') ? 1 : 0;
                maze[ghostRow][ghostCol + 1] = (previously_a_dot) ? '.' : ' ';
            }
            maze[ghostRow][ghostCol] = '8';
            break;

        case 3: // Moving the ghost one step down
            if (maze[ghostRow + 1][ghostCol] != '#') {
                ghostRow++;
                previously_a_dot = (maze[ghostRow][ghostCol] == '.') ? 1 : 0;
                maze[ghostRow - 1][ghostCol] = (previously_a_dot) ? '.' : ' ';
            }
            maze[ghostRow][ghostCol] = '8';
            break;

        case 4: // Moving the ghost one step right
            if (maze[ghostRow][ghostCol + 1] != '#') {
                ghostCol++;
                previously_a_dot = (maze[ghostRow][ghostCol] == '.') ? 1 : 0;
                maze[ghostRow][ghostCol - 1] = (previously_a_dot) ? '.' : ' ';
            }
            maze[ghostRow][ghostCol] = '8';
            break;
        }

        if (pelletsRemaining == 0)  // Win condition: Checking if all pellets have been collected
        {
            cout << "\n\nCongratulations! You won!";
            break;
        }
        if (maze[pacmanRow][pacmanCol] == maze[ghostRow][ghostCol]) // Losing condition: Checking if the pacman makes contact with the ghost
        {
            cout << "\n\nThe ghost caught the pacman!\nGame over!";
            break;
        }

    } while (1);
    return 0;
}