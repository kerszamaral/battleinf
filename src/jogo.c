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

int rot(int rot)
{
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) //For Tank like controls
    {
        if (IsKeyDown(KEY_W)||IsKeyDown(KEY_UP))
            rot = 0;
        if (IsKeyDown(KEY_S)||IsKeyDown(KEY_DOWN))
            rot = 180;
    }
    if (!IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)&& !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) //For Tank like controls
    {
        if (IsKeyDown(KEY_A)||IsKeyDown(KEY_LEFT))
            rot = 270;
        if (IsKeyDown(KEY_D)||IsKeyDown(KEY_RIGHT))
            rot = 90;
    }
    return rot;
}

int jogo(void)
{
    int pscore = 0;
    int pospx = 360, pospy = 100, rotp = 0; //Initial player position for testing
    int health = 3; //Player Health

    Font fontTtf = LoadFontEx("resources/fonts/Roboto-Regular.ttf", 32, 0, 250); //Load da fonte roboto
    Texture2D tankplayer = LoadTexture("resources/images/player.png");//Load imagem do tanque do player

    while (!WindowShouldClose() && health !=0)
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        pospx = movex(pospx);
        pospy = movey(pospy);
        rotp = rot(rotp);

        DrawTextEx(fontTtf, TextFormat("Score: %i", pscore), (Vector2){SCREENWIDTH / 2 - 150, 40 }, (float)fontTtf.baseSize, 2, GRAY);
        DrawTextureEx(tankplayer,(Vector2){pospx, pospy}, rotp, 0.1, WHITE);

        if (IsKeyPressed(KEY_ENTER))
        {
            pscore += 1;
        }

        EndDrawing();
    }
    UnloadFont(fontTtf);
    return pscore;
}