#include "raylib.h"
#include "jogo.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 450
#define TAMPLAYERX 348 //Tamanho original da imagem Escalar para player em 10%!!!!
#define TAMPLAYERY 457 //Tamanho original da imagem Escalar para player em 10%!!!!

int movey(int y) //Function for moving the player with WASD or Arrow keys
{
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) //For Tank like controls
    {
        if ( (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && y >= (TAMPLAYERY/20.0)) //Escalar para o centro, portanto 20 e não 10
            y -= 5;
        if ( (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && y + (TAMPLAYERY/20.0) <SCREENHEIGHT) //Escalar para o centro, portanto 20 e não 10
            y += 5;
    }
    return y;
}

int movex(int x)
{
    if (!IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)&& !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) //For Tank like controls
    {
        if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && x >= (TAMPLAYERX/20.0)) //Escalar para o centro, portanto 20 e não 10
            x -= 5;
        if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && x + (TAMPLAYERX/20.0) <SCREENWIDTH) //Escalar para o centro, portanto 20 e não 10
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
    Rectangle sourcePlayer = { 0, 0, TAMPLAYERX, TAMPLAYERY}; //Retangulo do tamanho da image com posição 00
    Vector2 centroPlayer = {TAMPLAYERX / 20.0, TAMPLAYERY / 20.0}; //Vetor para achar o centro da imagem do player escalada a 10%

    while (!WindowShouldClose() && health !=0)
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        //Retangulo movel do player, precisa estar aqui pois caso contrario não conseguimos mover
        Rectangle destPlayer = {pospx, pospy, TAMPLAYERX / 10.0, TAMPLAYERY / 10.0 }; // tamanho da imagem escalado para 10%

        pospx = movex(pospx);
        pospy = movey(pospy);
        rotp = rot(rotp);

        DrawTextEx(fontTtf, TextFormat("Score: %i", pscore), (Vector2){SCREENWIDTH / 2 - 150, 40 }, (float)fontTtf.baseSize, 2, GRAY);
        DrawTexturePro(tankplayer, sourcePlayer, destPlayer, centroPlayer, rotp, WHITE);
        if (IsKeyPressed(KEY_ENTER))
        {
            pscore += 1;
        }

        EndDrawing();
    }
    UnloadFont(fontTtf);
    return pscore;
}