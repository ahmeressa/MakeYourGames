#include "raylib.h"
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Recursive function to draw a fractal tree
void DrawBranch(float x, float y, float length, float angle, int depth, float angleDiff, float scale) {
    if (depth == 0) return;
    
    float x2 = x + cos(angle) * length;
    float y2 = y - sin(angle) * length;
    
    DrawLine((int)x, (int)y, (int)x2, (int)y2, DARKGREEN);
    
    DrawBranch(x2, y2, length * scale, angle - angleDiff, depth - 1, angleDiff, scale);
    DrawBranch(x2, y2, length * scale, angle + angleDiff, depth - 1, angleDiff, scale);
    
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Recursive Fractal Tree");
    SetTargetFPS(60);
    
    int depth = 6;
    float angleDiff = PI / 6;
    float scale = 0.7f;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP)) depth++;
        if (IsKeyPressed(KEY_DOWN) && depth > 1) depth--;
        if (IsKeyPressed(KEY_RIGHT)) angleDiff += 0.05f;
        if (IsKeyPressed(KEY_LEFT)) angleDiff -= 0.05f;
        if (IsKeyPressed(KEY_W)) scale += 0.05f;
        if (IsKeyPressed(KEY_S)) scale -= 0.05f;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawText("Use UP/DOWN to change to change recursion depth", 10, 10, 20, DARKGRAY);
        DrawText("Use LEFT/RIGHT to change angle", 10, 35, 20, DARKGRAY);
        DrawText("Use W/S to change scaling factor", 10, 60, 20, DARKGRAY);
        DrawText(TextFormat("Depth: %d", depth), 10, 100, 20, BLACK);
         DrawText(TextFormat("Scale: %.2f", scale), 10, 160, 20, BLACK);
          DrawText(TextFormat("Angle Diff: %.2f", angleDiff), 10, 130, 20, BLACK);
          DrawBranch(SCREEN_WIDTH/2, SCREEN_HEIGHT - 50, 100, PI/2, depth, angleDiff, scale);
          EndDrawing();
          
    }
    CloseWindow();
    return 0;
}
    