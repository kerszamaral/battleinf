#include "raylib.h"
#include "jogo.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 450
#define TAMPLAYERX 348 //Tamanho original da imagem Escalar para player em 10%!!!!
#define TAMPLAYERY 457 //Tamanho original da imagem Escalar para player em 10%!!!!
#define BORDER 15 //borda do jogo

int movey(int y) //Function for moving the player with WASD or Arrow keys
{
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) //For Tank like controls
    {
        if ( (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && y >= (TAMPLAYERY/20.0) + 45) //Escalar para o centro, portanto 20 e não 10
            y -= 5;
        if ( (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && y + (TAMPLAYERY/20.0) < SCREENHEIGHT - BORDER + 6) //Escalar para o centro, portanto 20 e não 10 (fator de correção)
            y += 5;
    }
    return y;
}

int movex(int x)
{
    if (!IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)&& !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) //For Tank like controls
    {
        if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && x >= (TAMPLAYERX/20.0) + BORDER) //Escalar para o centro, portanto 20 e não 10
            x -= 5;
        if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && x + (TAMPLAYERX/20.0) < SCREENWIDTH - BORDER) //Escalar para o centro, portanto 20 e não 10
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
    int pscore = 0; //Numero de score do player
    int pospx = 360, pospy = 100, rotp = 0; //Initial player position and rotation for testing
    int health = 3; //Player Health
    int fase = 1; //Numero da fase no display
    
    Texture2D healthimg = LoadTexture("resources/images/health.png"); //Load imagem da vida
    Texture2D tankplayer = LoadTexture("resources/images/player.png");//Load imagem do tanque do player
    Rectangle screenSizerec = {0, 0, SCREENWIDTH, SCREENHEIGHT}; //Retangulo para a borda do jogo
    Rectangle sourcePlayer = { 0, 0, TAMPLAYERX, TAMPLAYERY}; //Retangulo do tamanho da image com posição 00
    Vector2 centroPlayer = {TAMPLAYERX / 20.0, TAMPLAYERY / 20.0}; //Vetor para achar o centro da imagem do player escalada a 10%

    while (!WindowShouldClose() && health !=0)
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        //Interface text
        DrawRectangle(0, 0, SCREENWIDTH, 40, DARKGRAY);
        DrawText(TextFormat("Fase %d", fase), SCREENWIDTH/2 - 6*10 , 0, 40, YELLOW);
        DrawRectangleLinesEx(screenSizerec, 5, DARKGRAY);
        DrawText(TextFormat("Score: %i", pscore), SCREENWIDTH / 2 + 215, 5, 32, RED);

        pospx = movex(pospx);
        pospy = movey(pospy);
        rotp = rot(rotp);

        //Player drawing
        //Retangulo movel do player, precisa estar aqui pois caso contrario não conseguimos mover
        Rectangle destPlayer = {pospx, pospy, TAMPLAYERX / 10.0, TAMPLAYERY / 10.0 }; // tamanho da imagem escalado para 10%
        DrawTexturePro(tankplayer, sourcePlayer, destPlayer, centroPlayer, rotp, WHITE); //Draws player tank

        //Draws health for health number
        for (int i = 0, healthx = 5; i < health; i++, healthx += 30) 
            DrawTextureEx(healthimg,(Vector2){healthx,5} , 0 , 0.025, WHITE);//Essa imagem é mto grande, achar uma menor escala está ridicula
        
        //Variaveis de teste
        if (IsKeyPressed(KEY_H)) //Teste game over por vida
            health -= 1;
        if (IsKeyPressed(KEY_ENTER)) //Teste de score
            pscore += 1;

        EndDrawing();
    }
    return pscore;
}