#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include <stdlib.h>
#include <deque>
using namespace std;

Color green = {200, 200, 150, 255};
Color darkGreen = {0, 0, 0, 255};

const int cellSize = 30;
const int cellCount = 25;
const int offset = 75;
const int maxBodySize = 100;

double lastUpdateTime = 0.0;
double lastUpdateTimePacman = 0.0;
double lastUpdateTimeGhost = 0.0;

int puzzle[4][4];

int DrawScreen(int board[cellCount][cellCount], int, int, int, int);
void MovePacman(int board[cellCount][cellCount], int *, int *, int, int *);
void MoveGhost(int board[cellCount][cellCount], int *, int *, int, int);
int eventTriggeredPacman(double); // Function to move the pacman every 0.2 second
int eventTriggeredGhost(double);  // Function to move the ghost every 0.4 second
void ResetGame(int board[cellCount][cellCount], int *, int *, int *, int *, int *);
int eventTriggered(double);
int ElementInDeque(Vector2 element, deque<Vector2> deque);
void drawPuzzle();
void shufflePuzzle();
void performMove(int move);
void winGame();

class Snake
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}}; // A deque<Vector2> which contains the initial cells of the snake
    Vector2 direction = {1, 0};                                          // Initial direction to move (right)
    int addSegment = 0;
    void Draw() // Function to draw the snake
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, float(cellSize), float(cellSize)};
            DrawRectangleRounded(segment, 0.5, 6, darkGreen);
        }
    }

    void Update() // Function to Update the snake every at frame
    {
        body.push_front(Vector2Add(body[0], direction)); // A cell is added to the snake's front at each update
        if (addSegment)                                  // If the snake ate the food then addSegment becomes equal to 1. Then the body's last cell is not popped (i.e, its size only increases by one)
            addSegment = 0;
        else
            body.pop_back(); // If the snake did not eat the food at current update, then bodys last cell should be popped
    }

    void Reset() // Function to reset snake to its initial position when GameOver() function is called
    {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class Food
{
public:
    Vector2 position;
    Texture2D texture;
    Rectangle sourceRect; // Added source rectangle

    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("images/apple.png"); // Loading image and Scaling it
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        // Set source rectangle to the full size of the image
        sourceRect = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
        position = GenerateRandomPos(snakeBody);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        // Scale the image by specifying a smaller destination rectangle
        Rectangle destRect = {offset + position.x * cellSize, offset + position.y * cellSize, cellSize, cellSize};
        DrawTexturePro(texture, sourceRect, destRect, Vector2{0, 0}, 0.0f, WHITE);
    }

    Vector2 GenerateRandomCell() // Function to generate a random cell for the food at each update
    {
        float x = rand() % 25;
        float y = rand() % 25;
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) // Function to generate random position at each update
    {
        Vector2 position = GenerateRandomCell();
        while (ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game // A game class
{
public:
    Snake snake = Snake(); // Initializing a snake and a food
    Food food = Food(snake.body);
    int running = 1;
    int score = 0;

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }
    void Update()
    {
        if (running)
        {
            snake.Update();
            checkCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }
    void checkCollisionWithFood() // Function that checks if the snake's head collided with the food
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = 1;
            score++;
        }
    }
    void CheckCollisionWithEdges() // Function to check if the snake's head collided with edges of game window
    {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1 || snake.body[0].y == cellCount || snake.body[0].y == -1)
            GameOver();
    }

    void GameOver() // Game Over function. It resets the snake's position and generates random position for food too. And resets the score to zero
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = 0;
        score = 0;
    }

    void CheckCollisionWithTail()
    {
        deque<Vector2> headlessBody = snake.body; // Creating a copy of the snake's body but without the first cell of it (pop_front() function)
        headlessBody.pop_front();

        if (ElementInDeque(snake.body[0], headlessBody))
            GameOver();
    }
};

int main()
{
    srand(time(NULL));

    // Initialize Raylib window
    SetTargetFPS(60);
    InitWindow(700, 700, "Game Launcher");

    // Load images for buttons
    Image pacmanImg = LoadImage("images/pacmangame.png");
    Image numberPuzzleImg = LoadImage("images/numberpuzzlegame.png");
    Image snakeImg = LoadImage("images/snakegame.png");

    // Create textures from loaded images
    Texture2D pacmanTexture = LoadTextureFromImage(pacmanImg);
    Texture2D numberPuzzleTexture = LoadTextureFromImage(numberPuzzleImg);
    Texture2D snakeTexture = LoadTextureFromImage(snakeImg);

    // Unload the original images after creating textures
    UnloadImage(pacmanImg);
    UnloadImage(numberPuzzleImg);
    UnloadImage(snakeImg);

    bool pacmanWindowOpen = false;
    bool numberPuzzleWindowOpen = false;
    bool snakeWindowOpen = false;

    // Game loop
    while (!WindowShouldClose())
    {
        // Begin Drawing for the main window
        BeginDrawing();
        ClearBackground(GRAY);

        // Draw buttons with text
        Rectangle pacmanButton = {50, 150, 110, 110};
        Rectangle numberPuzzleButton = {300, 150, 110, 110};
        Rectangle snakeButton = {550, 150, 110, 110};

        DrawTextureRec(pacmanTexture, pacmanButton, {pacmanButton.x, pacmanButton.y}, WHITE);
        DrawText("Pacman", pacmanButton.x + 5, pacmanButton.y + pacmanButton.height + 10, 25, BLACK);

        DrawTextureRec(numberPuzzleTexture, numberPuzzleButton, {numberPuzzleButton.x, numberPuzzleButton.y}, WHITE);
        DrawText("Number Puzzle", numberPuzzleButton.x - 10, numberPuzzleButton.y + numberPuzzleButton.height + 10, 25, BLACK);

        DrawTextureRec(snakeTexture, snakeButton, {snakeButton.x, snakeButton.y}, WHITE);
        DrawText("Snake", snakeButton.x + 10, snakeButton.y + snakeButton.height + 10, 25, BLACK);

        DrawText("Choose a game to play!", 190, 460, 30, DARKGRAY);

        EndDrawing();

        // Opening the pacman game window and playing
        if (CheckCollisionPointRec(GetMousePosition(), pacmanButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int pacmanRow, pacmanCol;
            int ghostRow, ghostCol;
            int position;

            int board[cellCount][cellCount];
            int pelletsRemaining;

            // Initializing the game
            ResetGame(board, &pacmanRow, &pacmanCol, &ghostRow, &ghostCol, &pelletsRemaining);

            InitWindow(cellCount * cellSize, cellCount * cellSize, "Pacman");
            SetTargetFPS(60);
            pacmanWindowOpen = true;

            while (!WindowShouldClose() && pacmanWindowOpen)
            {
                BeginDrawing();

                pelletsRemaining = DrawScreen(board, pacmanRow, pacmanCol, ghostRow, ghostCol);

                // Check for key presses and update position accordingly
                if (IsKeyPressed(KEY_UP) && pacmanRow != 1)
                    position = 1;
                else if (IsKeyPressed(KEY_DOWN) && pacmanRow != cellCount - 2)
                    position = -1;
                else if (IsKeyPressed(KEY_LEFT) && pacmanCol != 1)
                    position = -2;
                else if (IsKeyPressed(KEY_RIGHT) && pacmanCol != cellCount - 2)
                    position = 2;

                // Check if the event is triggered before moving pacman
                if (eventTriggeredPacman(0.2))
                    MovePacman(board, &pacmanRow, &pacmanCol, position, &pelletsRemaining);

                if (eventTriggeredGhost(0.3))
                    MoveGhost(board, &ghostRow, &ghostCol, pacmanRow, pacmanCol);

                if (pelletsRemaining == 0)
                    ResetGame(board, &pacmanRow, &pacmanCol, &ghostRow, &ghostCol, &pelletsRemaining);

                if (pacmanRow == ghostRow && pacmanCol == ghostCol)
                    ResetGame(board, &pacmanRow, &pacmanCol, &ghostRow, &ghostCol, &pelletsRemaining);

                ClearBackground(BLACK);
                EndDrawing();
            }

            CloseWindow(); // Close the individual game window
        }

        // Opening the number puzzle game and playing
        else if (CheckCollisionPointRec(GetMousePosition(), numberPuzzleButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            InitWindow(4 * (cellSize * 2), 4 * (cellSize * 2) + 40, "Number Puzzle Game");
            SetTargetFPS(60);
            numberPuzzleWindowOpen = true;

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    puzzle[i][j] = i * 4 + j + 1;
                }
            }
            puzzle[3][3] = 0;

            shufflePuzzle();

            while (!WindowShouldClose() && numberPuzzleWindowOpen)
            {
                BeginDrawing();
                ClearBackground(RAYWHITE);

                // Draw the number puzzle
                drawPuzzle();

                // Check for arrow key press
                if (IsKeyPressed(KEY_UP))
                    performMove(1);
                else if (IsKeyPressed(KEY_DOWN))
                    performMove(2);
                else if (IsKeyPressed(KEY_LEFT))
                    performMove(3);
                else if (IsKeyPressed(KEY_RIGHT))
                    performMove(4);

                winGame();

                EndDrawing();
            }

            CloseWindow(); // Close the individual game window
        }

        // Opening the snake game window and playing
        else if (CheckCollisionPointRec(GetMousePosition(), snakeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake Game"); // Initialize window
            SetTargetFPS(60);                                                                               //  FPS = 60
            snakeWindowOpen = true;

            Game game = Game(); // Initializing a game (snake and food too)

            while (!WindowShouldClose() && snakeWindowOpen) // Game Loop
            {
                BeginDrawing();

                if (eventTriggered(0.2)) // Moves the snake every 0.2 second
                    game.Update();

                // Checks if any of the arrow keys are pressed
                if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
                {
                    game.snake.direction = {0, -1};
                    game.running = 1;
                }
                if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
                {
                    game.snake.direction = {0, 1};
                    game.running = 1;
                }
                if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
                {
                    game.snake.direction = {-1, 0};
                    game.running = 1;
                }
                if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
                {
                    game.snake.direction = {1, 0};
                    game.running = 1;
                }

                // Drawing
                ClearBackground(green);
                DrawRectangleLinesEx({offset - 5, offset - 5, cellSize * cellCount + 10, cellSize * cellCount + 10}, 5, darkGreen);
                DrawText("Mudassir's Snake Game", offset - 5, 20, 40, darkGreen);
                DrawText(TextFormat("Score: %d", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);

                game.Draw();

                EndDrawing();
            }

            CloseWindow(); // Close the individual game window
        }
        !WindowShouldClose();
    }

    CloseWindow(); // Close the main window

    return 0;
}

void ResetGame(int board[cellCount][cellCount], int *pacmanRow, int *pacmanCol, int *ghostRow, int *ghostCol, int *pelletsRemaining)
{
    // Initializing the board with walls and pellets
    for (int i = 0; i < cellCount; i++)
    {
        for (int j = 0; j < cellCount; j++)
        {
            if (
                // Borders
                (i == 0 || i == cellCount - 1 || j == 0 || j == cellCount - 1) ||
                // Two rectangles on top left and top right
                ((i == 3 || i == 4) && ((j >= 3 && j <= 8) || (j >= 16 && j <= 21))) ||
                // Vertical sticks of two 'T's on left and right
                ((j == 4 || j == 20) && (i >= 8 && i <= 16)) ||
                // Horizontal sticks of two 'T's on left and right
                (i == 12 && ((j >= 5 && j <= 9) || (j >= 15 && j <= 20))) ||
                // Box, inside which, the pacman will be at the start
                (i == 16 && ((j >= 8 && j <= 10) || (j >= 14 && j <= 16))) ||
                // Horizontal line of T in the center
                (i == 8 && (j >= 8 && j <= 16)) ||
                // Vertical line of T in the center
                (j == 12 && (i >= 9 && i <= 13)) ||
                // Additional walls
                ((j == 8 || j == 16) && (i >= 17 && i <= 20)) ||
                // Additional walls
                (i == 21 && (j >= 8 && j <= 16)))
            {
                // Walls
                board[i][j] = '#';
            }
            else
            {
                // Pellets
                board[i][j] = '.';
                ++(*pelletsRemaining);
            }
        }
    }
    *pacmanRow = 19, *pacmanCol = 12;
    *ghostRow = 3, *ghostCol = 12;
}

int eventTriggeredPacman(double interval) // Function to move the pacman every 0.2 second
{
    double currentTime = GetTime(); // This function gets the time elapsed since the window started
    if (currentTime - lastUpdateTimePacman >= interval)
    {
        lastUpdateTimePacman = currentTime;
        return 1;
    }
    return 0;
}

int eventTriggeredGhost(double interval) // Function to move the ghost every 0.4 second
{
    double currentTime = GetTime(); // This function gets the time elapsed since the window started
    if (currentTime - lastUpdateTimeGhost >= interval)
    {
        lastUpdateTimeGhost = currentTime;
        return 1;
    }
    return 0;
}

int DrawScreen(int board[cellCount][cellCount], int pacmanRow, int pacmanCol, int ghostRow, int ghostCol)
{
    int pelletsRemaining = 0;
    for (int c = 0; c < cellCount; c++) // Column
    {
        for (int r = 0; r < cellCount; r++) // Row
        {
            if (r == pacmanRow && c == pacmanCol)
            {
                DrawText("<", c * cellSize, r * cellSize, 40, YELLOW);
                continue;
            }
            else if (r == ghostRow && c == ghostCol)
            {
                DrawText("8", c * cellSize, r * cellSize, 40, RED);
                continue;
            }

            DrawText(TextFormat("%c", board[r][c]), c * cellSize, r * cellSize, 40, (board[r][c]=='.')? RAYWHITE : BLUE);
            if (board[r][c] == '.')
                pelletsRemaining++;
        }
    }

    return pelletsRemaining;
}

// 1 = row--, 2 = col++, -1 = row++, -2 = col--
void MovePacman(int board[cellCount][cellCount], int *pacmanRow, int *pacmanCol, int position, int *pelletsRemaining)
{
    int newRow = *pacmanRow;
    int newCol = *pacmanCol;
    // Calculate the new position based on the direction
    if (position == 1)
        newRow--;
    else if (position == -1)
        newRow++;
    else if (position == 2)
        newCol++;
    else if (position == -2)
        newCol--;

    // Check for collisions with walls and boundaries
    if (newRow > 0 && newRow < cellCount - 1 && newCol > 0 && newCol < cellCount - 1 && board[newRow][newCol] != '#')
    {
        if (board[newRow][newCol] == '.')
        {
            board[newRow][newCol] = 0;
            --(*pelletsRemaining);
        }
        // Update the position if there are no collisions
        *pacmanRow = newRow;
        *pacmanCol = newCol;
    }
}

void MoveGhost(int board[cellCount][cellCount], int *ghostRow, int *ghostCol, int pacmanRow, int pacmanCol)
{
    int newRow = *ghostRow;
    int newCol = *ghostCol;

    // Calculate the direction to move towards Pacman
    int rowDirection = (pacmanRow > *ghostRow) ? 1 : (pacmanRow < *ghostRow) ? -1
                                                                             : 0;
    int colDirection = (pacmanCol > *ghostCol) ? 1 : (pacmanCol < *ghostCol) ? -1
                                                                             : 0;

    // Attempt to move towards the target row or column
    if (rowDirection != 0 || colDirection != 0)
    {
        newRow += rowDirection;
        newCol += colDirection;
    }

    // Check for collisions with walls and boundaries
    if (board[newRow][*ghostCol] != '#')
        *ghostRow = newRow;
    if (board[*ghostRow][newCol] != '#')
        *ghostCol = newCol;
}

int eventTriggered(double interval) // Function to move the snake every 0.2 second
{
    double currentTime = GetTime(); // This functions gets the time elapsed since the window started
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return 1;
    }
    return 0;
}

int ElementInDeque(Vector2 element, deque<Vector2> deque) // Checks if the element in first arguement is a part of the deque<Vector2>
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
            return 1;
    }
    return 0;
}

void drawPuzzle()
{
    // Calculate elapsed time in seconds
    double elapsedTime = GetTime();
    
    int minutes = static_cast<int>(elapsedTime) / 60;
    int seconds = static_cast<int>(elapsedTime) % 60;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            int value = puzzle[i][j];
            if (value > 0)
            {
                DrawRectangle(j * cellSize * 2, i * cellSize * 2, cellSize * 2, cellSize * 2, LIGHTGRAY);
                DrawText(TextFormat("%d", value), j * cellSize * 2 + cellSize, i * cellSize * 2 + cellSize, 30, BLACK);
            }
        }

    // Draw elapsed time in "MM:SS" format
    DrawText(TextFormat("%02d:%02d", minutes, seconds), 10, 4 * (cellSize * 2) + 5, 30, DARKGRAY);
}

void shufflePuzzle()
{
    srand(GetTime());
    for (int i = 0; i < 4 * 4 * 4; i++)
    {
        int move = GetRandomValue(1, 4);
        int emptyRow, emptyCol;

        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                if (puzzle[r][c] == 0)
                {
                    emptyRow = r;
                    emptyCol = c;
                }
            }
        }

        switch (move)
        {
        case 1: // Up
            if (emptyRow > 0)
            {
                puzzle[emptyRow][emptyCol] = puzzle[emptyRow - 1][emptyCol];
                puzzle[emptyRow - 1][emptyCol] = 0;
            }
            break;
        case 2: // Down
            if (emptyRow < 3)
            {
                puzzle[emptyRow][emptyCol] = puzzle[emptyRow + 1][emptyCol];
                puzzle[emptyRow + 1][emptyCol] = 0;
            }
            break;
        case 3: // Left
            if (emptyCol > 0)
            {
                puzzle[emptyRow][emptyCol] = puzzle[emptyRow][emptyCol - 1];
                puzzle[emptyRow][emptyCol - 1] = 0;
            }
            break;
        case 4: // Right
            if (emptyCol < 3)
            {
                puzzle[emptyRow][emptyCol] = puzzle[emptyRow][emptyCol + 1];
                puzzle[emptyRow][emptyCol + 1] = 0;
            }
            break;
        }
    }
}

void performMove(int move)
{
    int emptyRow, emptyCol;
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (puzzle[r][c] == 0)
            {
                emptyRow = r;
                emptyCol = c;
            }
        }
    }

    switch (move)
    {
    case 2: // Up
        if (emptyRow > 0)
        {
            puzzle[emptyRow][emptyCol] = puzzle[emptyRow - 1][emptyCol];
            puzzle[emptyRow - 1][emptyCol] = 0;
        }
        break;
    case 1: // Down
        if (emptyRow < 3)
        {
            puzzle[emptyRow][emptyCol] = puzzle[emptyRow + 1][emptyCol];
            puzzle[emptyRow + 1][emptyCol] = 0;
        }
        break;
    case 4: // Left
        if (emptyCol > 0)
        {
            puzzle[emptyRow][emptyCol] = puzzle[emptyRow][emptyCol - 1];
            puzzle[emptyRow][emptyCol - 1] = 0;
        }
        break;
    case 3: // Right
        if (emptyCol < 3)
        {
            puzzle[emptyRow][emptyCol] = puzzle[emptyRow][emptyCol + 1];
            puzzle[emptyRow][emptyCol + 1] = 0;
        }
        break;
    }
}

void winGame()
{
    // Check if the puzzle is solved
    int count = 1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (puzzle[i][j] != count % 16)
            {
                return; // Puzzle is not solved yet
            }
            count++;
        }
    }

    // Puzzle is solved, display "You solved it!"
    DrawText("You solved it!", GetScreenWidth() / 2 - MeasureText("You solved it!", 30) / 2, GetScreenHeight() / 2 - 15, 30, BLACK);
}