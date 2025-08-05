#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 20
#define GRID_WIDTH 40
#define GRID_HEIGHT 30
#define SCREEN_WIDTH (CELL_SIZE * GRID_WIDTH)
#define SCREEN_HEIGHT (CELL_SIZE * GRID_HEIGHT)
#define MAX_SNAKE_LENGTH (GRID_WIDTH * GRID_HEIGHT)

typedef struct {
    int x, y;
} Vector2Int;

typedef enum {
    UP, DOWN, LEFT, RIGHT
} Direction;

Vector2Int snake[MAX_SNAKE_LENGTH];
int snakeLength = 3;
Direction dir = RIGHT;
Vector2Int food;
bool gameOver = false;

// Timing
float moveTimer = 0.01f;
float moveDelay = 0.2f;
void ResetGame() {
    srand((unsigned int)time(NULL));
    snakeLength = 3;
    snake[0].x = GRID_WIDTH / 2;
    snake[0].y = GRID_HEIGHT / 2;
    dir = RIGHT;
    food.x = rand() % GRID_WIDTH;
    food.y = rand() % GRID_HEIGHT;
    gameOver = false;
    moveDelay = 0.2f;
    moveTimer = 0.0f;
}

bool CheckCollision(Vector2Int a, Vector2Int b) {
    return a.x == b.x && a.y == b.y;
}

void UpdateSnakeLogic() {
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    switch (dir) {
        case UP:    snake[0].y--; break;
        case DOWN:  snake[0].y++; break;
        case LEFT:  snake[0].x--; break;
        case RIGHT: snake[0].x++; break;
    }

    if (snake[0].x < 0 || snake[0].x >= GRID_WIDTH || snake[0].y < 0 || snake[0].y >= GRID_HEIGHT) {
        gameOver = true;
        return;
    }

    for (int i = 1; i < snakeLength; i++) {
        if (CheckCollision(snake[0], snake[i])) {
            gameOver = true;
            return;
        }
    }

    if (CheckCollision(snake[0], food)) {
        if (snakeLength < MAX_SNAKE_LENGTH) snakeLength++;
        food.x = rand() % GRID_WIDTH;
        food.y = rand() % GRID_HEIGHT;

        // Speed up the snake slightly (lower delay)
        if (moveDelay > 0.0f) {
            moveDelay -= 0.005f;
        }
    }
}

void DrawGameGrid() {
    for (int x = 0; x <= SCREEN_WIDTH; x += CELL_SIZE)
        DrawLine(x, 0, x, SCREEN_HEIGHT, LIGHTGRAY);
    for (int y = 0; y <= SCREEN_HEIGHT; y += CELL_SIZE)
        DrawLine(0, y, SCREEN_WIDTH, y, LIGHTGRAY);
}

void DrawSnake() {
    for (int i = 0; i < snakeLength; i++) {
        Color color = (i == 0) ? GREEN : DARKGREEN;
        DrawRectangle(snake[i].x * CELL_SIZE, snake[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
    }
}

void DrawFood() {
    DrawRectangle(food.x * CELL_SIZE, food.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    SetTargetFPS(60);

    ResetGame();

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Input
        if (!gameOver) {
            if (IsKeyPressed(KEY_UP) && dir != DOWN) dir = UP;
            if (IsKeyPressed(KEY_DOWN) && dir != UP) dir = DOWN;
            if (IsKeyPressed(KEY_LEFT) && dir != RIGHT) dir = LEFT;
            if (IsKeyPressed(KEY_RIGHT) && dir != LEFT) dir = RIGHT;
        }

        if (IsKeyPressed(KEY_R)) ResetGame();

        // Update with delta time
        if (!gameOver) {
            moveTimer += deltaTime;
            if (moveTimer >= moveDelay) {
                UpdateSnakeLogic();
                moveTimer = 0.0f;
            }
        }
        BeginDrawing();
        ClearBackground(BLACK);

        DrawGameGrid();
        DrawSnake();
        DrawFood();

        if (gameOver) {
            DrawText("GAME OVER", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 40, 40, WHITE);
            DrawText("Press R to Restart", SCREEN_WIDTH/2 - 110, SCREEN_HEIGHT/2 + 10, 20, GRAY);
        }
        DrawText(TextFormat("Score: %d", snakeLength - 1), 10, 10, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
