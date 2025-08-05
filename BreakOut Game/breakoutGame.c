
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BALLS 10
#define MAX_POWERUPS 10

typedef struct {
    Rectangle box;
    Color color;
    int score;
    bool active;
} Box;

typedef struct {
    float radius;
    Vector2 center;
    Color color;
    int x_speed;
    int y_speed;
    bool active;
} Ball;

typedef struct {
    Rectangle rect;
    int type; // 0: Extra ball, 1: Resize paddle
    bool active;
    Color color;
} PowerUp;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int row = 5;
    const int column = 10;
    const int boxWidth = screenWidth / column;
    const int boxHeight = 20;

    InitWindow(screenWidth, screenHeight, "Breakout Game - Raylib");
    SetTargetFPS(60);
    srand(time(NULL));

    Box boxes[row][column];
    Ball balls[MAX_BALLS] = {0};
    PowerUp powerUps[MAX_POWERUPS] = {0};

    Rectangle paddle = {screenWidth / 2 - 60, screenHeight - 30, 120, 20};
    Color paddleColor = DARKGRAY;

    int score = 0;
    bool gameWon = false;

    // Initialize bricks
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < column; c++) {
            boxes[r][c].box = (Rectangle){ c * boxWidth + 1, 60 + r * boxHeight, boxWidth - 2, boxHeight - 2 };
            boxes[r][c].active = true;
            switch (r) {
                case 0: boxes[r][c].color = RED; boxes[r][c].score = 10; break;
                case 1: boxes[r][c].color = ORANGE; boxes[r][c].score = 8; break;
                case 2: boxes[r][c].color = YELLOW; boxes[r][c].score = 6; break;
                case 3: boxes[r][c].color = GREEN; boxes[r][c].score = 4; break;
                case 4: boxes[r][c].color = BLUE; boxes[r][c].score = 2; break;
            }
        }
    }

    balls[0] = (Ball){10.0, (Vector2){screenWidth/2, screenHeight/2}, BLACK, 5, -5, true};

    while (!WindowShouldClose())
    {
        if (gameWon) {
            if (IsKeyPressed(KEY_R)) {
                // Reset everything
                score = 0;
                gameWon = false;
                paddle = (Rectangle){screenWidth / 2 - 60, screenHeight - 30, 120, 20};

                for (int r = 0; r < row; r++) {
                    for (int c = 0; c < column; c++) {
                        boxes[r][c].active = true;
                    }
                }

                for (int b = 0; b < MAX_BALLS; b++) {
                    balls[b].active = false;
                }
                balls[0] = (Ball){10.0, (Vector2){screenWidth/2, screenHeight/2}, BLACK, 5, -5, true};

                for (int p = 0; p < MAX_POWERUPS; p++) {
                    powerUps[p].active = false;
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("CONGRATULATIONS! YOU WON!", screenWidth/2 - 170, screenHeight/2 - 20, 20, GREEN);
            DrawText("Press [R] to Restart", screenWidth/2 - 110, screenHeight/2 + 20, 20, DARKGRAY);
            EndDrawing();
            continue;
        }

        // Paddle movement
        if (IsKeyDown(KEY_RIGHT)) paddle.x += 6;
        if (IsKeyDown(KEY_LEFT)) paddle.x -= 6;
        if (paddle.x < 0) paddle.x = 0;
        if (paddle.x + paddle.width > screenWidth) paddle.x = screenWidth - paddle.width;

        // Update balls
        for (int i = 0; i < MAX_BALLS; i++) {
            if (!balls[i].active) continue;

            balls[i].center.x += balls[i].x_speed;
            balls[i].center.y += balls[i].y_speed;

            if (balls[i].center.x <= balls[i].radius || balls[i].center.x >= screenWidth - balls[i].radius)
                balls[i].x_speed *= -1;
            if (balls[i].center.y <= balls[i].radius)
                balls[i].y_speed *= -1;
            if (balls[i].center.y >= screenHeight)
                balls[i].active = false;

            if (CheckCollisionCircleRec(balls[i].center, balls[i].radius, paddle)) {
                balls[i].y_speed *= -1;
                balls[i].center.y = paddle.y - balls[i].radius;
            }

            for (int r = 0; r < row; r++) {
                for (int c = 0; c < column; c++) {
                    if (boxes[r][c].active && CheckCollisionCircleRec(balls[i].center, balls[i].radius, boxes[r][c].box)) {
                        boxes[r][c].active = false;
                        score += boxes[r][c].score;
                        balls[i].y_speed *= -1;

                        if (rand() % 10 < 2) {
                            for (int p = 0; p < MAX_POWERUPS; p++) {
                                if (!powerUps[p].active) {
                                    powerUps[p].active = true;
                                    powerUps[p].type = rand() % 2;
                                    powerUps[p].rect = (Rectangle){boxes[r][c].box.x + boxWidth/2 - 10, boxes[r][c].box.y, 20, 20};
                                    powerUps[p].color = (powerUps[p].type == 0) ? PURPLE : SKYBLUE;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Update powerups
        for (int p = 0; p < MAX_POWERUPS; p++) {
            if (!powerUps[p].active) continue;
            powerUps[p].rect.y += 3;
            if (CheckCollisionRecs(paddle, powerUps[p].rect)) {
                if (powerUps[p].type == 0) {
                    for (int b = 0; b < MAX_BALLS; b++) {
                        if (!balls[b].active) {
                            balls[b] = (Ball){10.0, (Vector2){paddle.x + paddle.width/2, paddle.y - 20}, BLACK, rand()%5-2, -5, true};
                            break;
                        }
                    }
                } else if (powerUps[p].type == 1) {
                    paddle.width = (paddle.width == 120) ? 80 : 120;
                }
                powerUps[p].active = false;
            }
        }

        // Check win condition
        bool anyBricksLeft = false;
        for (int r = 0; r < row && !anyBricksLeft; r++) {
            for (int c = 0; c < column; c++) {
                if (boxes[r][c].active) {
                    anyBricksLeft = true;
                    break;
                }
            }
        }
        if (!anyBricksLeft) {
            gameWon = true;
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int r = 0; r < row; r++) {
            for (int c = 0; c < column; c++) {
                if (boxes[r][c].active)
                    DrawRectangleRec(boxes[r][c].box, boxes[r][c].color);
            }
        }

        DrawRectangleRec(paddle, paddleColor);

        for (int i = 0; i < MAX_BALLS; i++) {
            if (balls[i].active)
                DrawCircleV(balls[i].center, balls[i].radius, balls[i].color);
        }

        for (int p = 0; p < MAX_POWERUPS; p++) {
            if (powerUps[p].active)
                DrawRectangleRec(powerUps[p].rect, powerUps[p].color);
        }

        DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);
        DrawText("Use arrow keys to move paddle", screenWidth - 260, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
