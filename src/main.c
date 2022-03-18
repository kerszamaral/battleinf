/* Ian Kersz - Cartão UFRGS: 00338368
00.03.22

in: 2 valores
out: 1 valor 
*/

#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Meu texto");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Texto", 190, 200, 20, GRAY);
        DrawRectangle(screenWidth/2 - 40, 100, 80, 80, RED);

        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}