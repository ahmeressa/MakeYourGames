#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define WIN_SCORE 5

typedef struct {
    int posX, posY;
    float radius;
    int speedX, speedY;
    Color color;
} Ball;

typedef struct {
    int posX, posY;
    int width, height;
    int speed;
    Color color;
} Paddle;

void InitBall(Ball *ball) {
    ball->posX = SCREEN_WIDTH / 2;
    ball->posY = SCREEN_HEIGHT / 2;
    ball->radius = 10;
    ball->speedX = 5;
    ball->speedY = 5;
    ball->color = WHITE;
}

void InitPaddle(Paddle *paddle, int x, Color color) {
    paddle->posX = x;
    paddle->posY = SCREEN_HEIGHT / 2 - 40;
    paddle->width = 10;
    paddle->height = 80;
    paddle->speed = 6;
    paddle->color = color;
}

void ClampPaddle(Paddle *paddle) {
    if (paddle->posY < 0) paddle->posY = 0;
    if (paddle->posY + paddle->height > SCREEN_HEIGHT)
        paddle->posY = SCREEN_HEIGHT - paddle->height;
}

void MoveAI(Paddle *paddle, Ball *ball) {
    if (GetRandomValue(0, 100) < 90) { // 90% chance to react
        if (ball->posY < paddle->posY + paddle->height/2)
            paddle->posY -= paddle->speed - 2;
        else if (ball->posY > paddle->posY + paddle->height/2)
            paddle->posY += paddle->speed - 2;
    }
}

void MovePaddle(Paddle *paddle, int keyUp, int keyDown) {
    if (IsKeyDown(keyUp)) paddle->posY -= paddle->speed;
    if (IsKeyDown(keyDown)) paddle->posY += paddle->speed;
}

void ResetBall(Ball *ball, int direction) {
    ball->posX = SCREEN_WIDTH / 2;
    ball->posY = SCREEN_HEIGHT / 2;
    ball->speedX = 5 * direction;
    ball->speedY = 5;
}

bool BallCollides(Ball *ball, Paddle *paddle) {
    return CheckCollisionCircleRec(
        (Vector2){ball->posX, ball->posY}, ball->radius,
        (Rectangle){paddle->posX, paddle->posY, paddle->width, paddle->height}
    );
}

void DrawGame(Ball *ball, Paddle *p1, Paddle *p2, int s1, int s2, int mode) {
    ClearBackground(BLACK);
    DrawCircle(ball->posX, ball->posY, ball->radius, ball->color);
    DrawRectangle(p1->posX, p1->posY, p1->width, p1->height, p1->color);
    DrawRectangle(p2->posX, p2->posY, p2->width, p2->height, p2->color);
    DrawText(TextFormat("%d", s1), SCREEN_WIDTH/4, 20, 40, WHITE);
    DrawText(TextFormat("%d", s2), 3*SCREEN_WIDTH/4, 20, 40, WHITE);
    DrawText(mode == 1 ? "Single Player" : "Two Player", SCREEN_WIDTH/2 - 100, 10, 20, GRAY);
}

void ShowEndScreen(const char *msg) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(msg, SCREEN_WIDTH/2 - MeasureText(msg, 30)/2, SCREEN_HEIGHT/2 - 30, 30, YELLOW);
    DrawText("Press ENTER to play again or ESC to exit", SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 + 20, 20, GRAY);
    EndDrawing();
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong Refactored");

    Ball ball;
    Paddle player1, player2;
    InitBall(&ball);
    InitPaddle(&player1, 50, BLUE);
    InitPaddle(&player2, SCREEN_WIDTH - 60, RED);

    int gameMode = 0;
    int p1_score = 0, p2_score = 0;
    bool gameOver = false;
    const char *winText = "";

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (gameMode == 0) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Press 1 for Single Player", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 40, 20, WHITE);
            DrawText("Press 2 for Two Player", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2, 20, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_ONE)) gameMode = 1;
            if (IsKeyPressed(KEY_TWO)) gameMode = 2;
            continue;
        }

        if (gameOver) {
            ShowEndScreen(winText);
            if (IsKeyPressed(KEY_ENTER)) {
                p1_score = 0;
                p2_score = 0;
                gameOver = false;
                InitBall(&ball);
            }
            continue;
        }

        // Input
        MovePaddle(&player1, KEY_W, KEY_S);
        if (gameMode == 2)
            MovePaddle(&player2, KEY_UP, KEY_DOWN);
        else
            MoveAI(&player2, &ball);

        ClampPaddle(&player1);
        ClampPaddle(&player2);

        // Ball Movement
        ball.posX += ball.speedX;
        ball.posY += ball.speedY;

        if (ball.posY - ball.radius <= 0 || ball.posY + ball.radius >= SCREEN_HEIGHT)
            ball.speedY *= -1;

        if (BallCollides(&ball, &player1)) ball.speedX *= -1;
        if (BallCollides(&ball, &player2)) ball.speedX *= -1;

        if (ball.posX < 0) {
            p2_score++;
            ResetBall(&ball, 1);
        }
        if (ball.posX > SCREEN_WIDTH) {
            p1_score++;
            ResetBall(&ball, -1);
        }

        // Win Check
        if (p1_score >= WIN_SCORE || p2_score >= WIN_SCORE) {
            gameOver = true;
            if (gameMode == 1)
                winText = (p1_score > p2_score) ? "You Win!" : "You Lose!";
            else
                winText = (p1_score > p2_score) ? "Player 1 Wins!" : "Player 2 Wins!";
        }

        BeginDrawing();
        DrawGame(&ball, &player1, &player2, p1_score, p2_score, gameMode);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
