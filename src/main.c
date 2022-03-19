/* Ian Kersz - Cartão UFRGS: 00338368
18.03.22
BattleInf Jogo de tanquezinho lá do NES
*/

#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 450;

int pospx = 360, pospy = 100; //Initial player position for testing

void move() //Function for moving the player
{
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) //For Tank like controls
    {
        if (IsKeyDown(KEY_W))
            pospy -= 5;
        if (IsKeyDown(KEY_S))
            pospy += 5;
    }
    if (!IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)) //For Tank like controls
    {
        if (IsKeyDown(KEY_A))
            pospx -= 5;
        if (IsKeyDown(KEY_D))
            pospx += 5;
    }
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Game");
    SetTargetFPS(60);

    Font fontTtf = LoadFontEx("resources/fonts/Roboto-Regular.ttf", 32, 0, 250); //Load da fonte roboto
    
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        move();

        DrawTextEx(fontTtf, "testing font", (Vector2){ screenWidth/2 - 40, screenHeight/2 }, (float)fontTtf.baseSize, 2, LIME);
        DrawRectangle(pospx, pospy, 80, 80, RED);

        EndDrawing();
    }

    UnloadFont(fontTtf);
    CloseWindow();
    return 0;
}