#include "raylib.h"
#include "jogo.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 450

int movey(int y) //Function for moving the player with WASD or Arrow keys
{
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) //For Tank like controls
    {
        if (IsKeyDown(KEY_W)||IsKeyDown(KEY_UP))
            y -= 5;
        if (IsKeyDown(KEY_S)||IsKeyDown(KEY_DOWN))
            y += 5;
    }
    return y;
}

int movex(int x)
{
    if (!IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)&& !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) //For Tank like controls
    {
        if (IsKeyDown(KEY_A)||IsKeyDown(KEY_LEFT))
            x -= 5;
        if (IsKeyDown(KEY_D)||IsKeyDown(KEY_RIGHT))
            x += 5;
    }
    return x;
}

void jogo(void)
{

    int pospx = 360, pospy = 100; //Initial player position for testing
    int health = 3; //Player Health

    Font fontTtf = LoadFontEx("resources/fonts/Roboto-Regular.ttf", 32, 0, 250); //Load da fonte roboto
    while (!WindowShouldClose()&& health !=0)
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        pospx = movex(pospx);
        pospy = movey(pospy);

        DrawTextEx(fontTtf, "testing font", (Vector2){ SCREENWIDTH/2 - 40, SCREENHEIGHT/2 }, (float)fontTtf.baseSize, 2, LIME);
        DrawRectangle(pospx, pospy, 80, 80, RED);

        if (IsKeyDown(KEY_ENTER))
        {
            health -= 1;
        }

        EndDrawing();
    }
    UnloadFont(fontTtf);
}