/* Ian Kersz - Cartão UFRGS: 00338368
18.03.22
BattleInf Jogo de tanquezinho lá do NES
*/

#include "raylib.h"
#include <stdio.h>

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    int pospx = 360, pospy = 100;
    InitWindow(screenWidth, screenHeight, "Meu texto");
    
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        if (IsKeyDown(KEY_W))
            pospy += -5;
        if (IsKeyDown(KEY_S))
            pospy += 5;
        if (IsKeyDown(KEY_A))
            pospx += -5;
        if (IsKeyDown(KEY_D))
            pospx += 5;
        
        DrawText("Testing moving", 190, 200, 20, GRAY);
        
        DrawRectangle(pospx, pospy, 80, 80, RED);

        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}