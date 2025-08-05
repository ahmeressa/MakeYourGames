#include "raylib.h"

struct Ball {
    int posX;
    int posY;
    float radius;
    int speedX;
    int speedY;
    Color color;
};

struct Paddle {
    int posX;
    int posY;
    int width;
    int height;
    int speed;
    Color color;
};

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int winScore = 5;

    InitWindow(screenWidth, screenHeight, "Pong - 1P & 2P");

    int gameMode = 0; // 0 = not chosen, 1 = single, 2 = two player
    bool gameOver = false;
    bool showWinMessage = false;
    const char* winText = "";

    struct Ball ball = {screenWidth/2, screenHeight/2, 10, 5, 5, WHITE};
    struct Paddle player1 = {50, screenHeight/2 - 40, 10, 80, 6, BLUE};
    struct Paddle player2 = {screenWidth - 60, screenHeight/2 - 40, 10, 80, 6, RED};

    int player1_score = 0;
    int player2_score = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Game mode selection
        if (gameMode == 0) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Press 1 for Single Player", screenWidth/2 - 150, screenHeight/2 - 40, 20, WHITE);
            DrawText("Press 2 for Two Player", screenWidth/2 - 150, screenHeight/2, 20, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_ONE)) gameMode = 1;
            if (IsKeyPressed(KEY_TWO)) gameMode = 2;
            continue;
        }

        if (gameOver) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText(winText, screenWidth/2 - MeasureText(winText, 30)/2, screenHeight/2 - 30, 30, YELLOW);
            DrawText("Press ENTER to play again or ESC to exit", screenWidth/2 - 200, screenHeight/2 + 20, 20, GRAY);
            EndDrawing();

            if (IsKeyPressed(KEY_ENTER)) {
                // Reset everything
                gameOver = false;
                player1_score = 0;
                player2_score = 0;
                ball.posX = screenWidth/2;
                ball.posY = screenHeight/2;
                ball.speedX = 5;
                ball.speedY = 5;
            }
            continue;
        }

        // --- Input ---

        // Player 1
        if (IsKeyDown(KEY_W)) player1.posY -= player1.speed;
        if (IsKeyDown(KEY_S)) player1.posY += player1.speed;

        // Keep paddle in bounds
        if (player1.posY < 0) player1.posY = 0;
        if (player1.posY + player1.height > screenHeight) player1.posY = screenHeight - player1.height;

        // Player 2
        if (gameMode == 2) {
            if (IsKeyDown(KEY_UP)) player2.posY -= player2.speed;
            if (IsKeyDown(KEY_DOWN)) player2.posY += player2.speed;
        } else {
            // AI
            if (ball.posY < player2.posY + player2.height/2) player2.posY -= player2.speed;
            if (ball.posY > player2.posY + player2.height/2) player2.posY += player2.speed;
        }

        // Keep paddle 2 in bounds
        if (player2.posY < 0) player2.posY = 0;
        if (player2.posY + player2.height > screenHeight) player2.posY = screenHeight - player2.height;

        // --- Ball Movement ---
        ball.posX += ball.speedX;
        ball.posY += ball.speedY;

        // Wall collision
        if (ball.posY - ball.radius <= 0 || ball.posY + ball.radius >= screenHeight)
            ball.speedY *= -1;

        // Paddle collisions
        if (CheckCollisionCircleRec((Vector2){ball.posX, ball.posY}, ball.radius,
                                    (Rectangle){player1.posX, player1.posY, player1.width, player1.height}))
            ball.speedX *= -1;

        if (CheckCollisionCircleRec((Vector2){ball.posX, ball.posY}, ball.radius,
                                    (Rectangle){player2.posX, player2.posY, player2.width, player2.height}))
            ball.speedX *= -1;

        // --- Scoring ---
        if (ball.posX < 0) {
            player2_score++;
            ball.posX = screenWidth / 2;
            ball.posY = screenHeight / 2;
            ball.speedX = 5;
            ball.speedY = 5;
        }

        if (ball.posX > screenWidth) {
            player1_score++;
            ball.posX = screenWidth / 2;
            ball.posY = screenHeight / 2;
            ball.speedX = -5;
            ball.speedY = 5;
        }

        // --- Win Check ---
        if (player1_score >= winScore || player2_score >= winScore) {
            gameOver = true;
            if (gameMode == 1) {
                winText = (player1_score > player2_score) ? "You Win!" : "You Lose!";
            } else {
                winText = (player1_score > player2_score) ? "Player 1 Wins!" : "Player 2 Wins!";
            }
        }
        if (GetRandomValue(0, 100) < 90) { // 90% chance it moves
            if (ball.posY < player2.posY + player2.height / 2)
        player2.posY -= player2.speed - 2; // move slower
            if (ball.posY > player2.posY + player2.height / 2)
        player2.posY += player2.speed - 2;
        }
        

        // --- Drawing ---
        BeginDrawing();
        ClearBackground(BLACK);

        DrawCircle(ball.posX, ball.posY, ball.radius, ball.color);
        DrawRectangle(player1.posX, player1.posY, player1.width, player1.height, player1.color);
        DrawRectangle(player2.posX, player2.posY, player2.width, player2.height, player2.color);

        DrawText(TextFormat("%d", player1_score), screenWidth / 4, 20, 40, WHITE);
        DrawText(TextFormat("%d", player2_score), 3 * screenWidth / 4, 20, 40, WHITE);

        DrawText(gameMode == 1 ? "Single Player Mode" : "Two Player Mode", screenWidth/2 - 100, 10, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
