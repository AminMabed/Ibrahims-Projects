#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void PrintBoard(int arr[4][4]);
int CheckWin(int arr[4][4]);

int main() {
    int board[4][4];
    int used[16] = {0};
    int input, temp, r, c;

    srand(time(NULL)); // Seed the random number generator

    // Initialize the board with unique random numbers
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            if (r == 3 && c == 3) {
                board[r][c] = 0;
                continue;
            }

            do {
                int r_num = rand() % 15 + 1;
                if (!used[r_num]) {
                    board[r][c] = r_num;
                    used[r_num] = 1;
                    break;
                }
            } while (1);
        }
    }

    PrintBoard(board);

    // Gameplay loop
    do {
        printf("\nEnter number to move: ");
        scanf("%d", &input);

        int i = -1, j = -1;
        // Find the position of the input number on the board
        for (r = 0; r < 4; r++) {
            for (c = 0; c < 4; c++) {
                if (board[r][c] == input) {
                    i = r;
                    j = c;
                    break;
                }
            }
            if (i != -1) {
                break;
            }
        }

        if (i == -1 || j == -1) {
            printf("Invalid input! Try again!");
            continue;
        }

        // Move the number in the desired direction
        if (i > 0 && board[i - 1][j] == 0) {
            board[i - 1][j] = board[i][j];
            board[i][j] = 0;
        } else if (i < 3 && board[i + 1][j] == 0) {
            board[i + 1][j] = board[i][j];
            board[i][j] = 0;
        } else if (j > 0 && board[i][j - 1] == 0) {
            board[i][j - 1] = board[i][j];
            board[i][j] = 0;
        } else if (j < 3 && board[i][j + 1] == 0) {
            board[i][j + 1] = board[i][j];
            board[i][j] = 0;
        } else {
            printf("Invalid input! Try again!");
            continue;
        }

        PrintBoard(board);
    } while (!CheckWin(board));

    printf("Congratulations! You've solved the puzzle!\n");
    return 0;
}

void PrintBoard(int arr[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (arr[r][c] != 0) {
                printf("%2d  ", arr[r][c]);
            } else {
                printf("   "); // Print empty space for 0
            }
        }
        puts("");
    }
}

int CheckWin(int arr[4][4]) {
    int count = 1;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (arr[r][c] != count) {
                if (r == 3 && c == 3 && arr[r][c] == 0) {
                    count++;
                    continue; // Skip the check for the last cell with 0
                } else {
                    return 0; // Return 0 immediately if there's a mismatch
                }
            }
            count++;
        }
    }
    return 1; // Return 1 if the loop completes without finding any mismatches
}
