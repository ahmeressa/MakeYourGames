#include "raylib.h"
#define CELL_SIZE 150
#define GRID_SIZE 3

typedef enum{EMPTY, X, O} Cell;


int main(void){
    const int screenWidth = CELL_SIZE * GRID_SIZE;
    const int screenHeight = CELL_SIZE * GRID_SIZE;
    int board[GRID_SIZE][CELL_SIZE];
    Cell grid[GRID_SIZE][CELL_SIZE] = {EMPTY};
    Vector2mousePosition;
    int currentPlayer = X;
    int winner = 0;
}

    InitWindow(screenWidth, screenHeight, "Tic Tac Toe");
    SetTargetFPS(60);
    
while (!WindowShouldClose()) 
{
    for (int i = 1; i < GRID_SIZE; i++) {
        DrawLine
        
        
        
        
        
    