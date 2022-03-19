#include "raylib.h"
#include "jogo.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 450
#define TAMPLAYERX 348 //Tamanho original da imagem Escalar para player em 10%!!!!
#define TAMPLAYERY 457 //Tamanho original da imagem Escalar para player em 10%!!!!
#define BORDER 15 //borda do jogo

typedef struct
{
    int x;
    int y;
    int health;
    int rot;
    int score;
} Plr;

Plr player = {0, 0, 0, 0, 0};

void move(void) //Function for moving the player with WASD or Arrow keys
{
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) //For Tank like controls
    {
        if ( (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && player.y >= (TAMPLAYERY/20.0) + 45) //Escalar para o centro, portanto 20 e não 10
        {
            player.y -= 5;
            player.rot = 0;
        }
        if ( (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && player.y + (TAMPLAYERY/20.0) < SCREENHEIGHT - BORDER + 6) //Escalar para o centro, portanto 20 e não 10 (fator de correção)
        {
            player.y += 5;
            player.rot = 180;
        }
    }
    if (!IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)&& !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) //For Tank like controls
    {
        if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && player.x >= (TAMPLAYERX/20.0) + BORDER) //Escalar para o centro, portanto 20 e não 10
        {
            player.x -= 5;
            player.rot = 270;
        }
        if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && player.x + (TAMPLAYERX/20.0) < SCREENWIDTH - BORDER) //Escalar para o centro, portanto 20 e não 10
        {
            player.x += 5;
            player.rot = 90;
        }
    }
}

int jogo(void)
{
    int fase = 1; //Numero da fase no display
    player.health = 3; //To solve infinite loop bug caused by struct
    player.score = 0; //To solve score not reseting
    player.rot = 0; //To solve rotation not reseting
    player.x = GetRandomValue(BORDER + (TAMPLAYERX/20.0), SCREENWIDTH - BORDER - (TAMPLAYERX/20.0)); //To start player in random position inbounds
    player.y = GetRandomValue(40 + BORDER + (TAMPLAYERY/20.0), SCREENHEIGHT - BORDER - (TAMPLAYERY/20.0)); //To start player in random position inbounds

    Texture2D healthimg = LoadTexture("resources/images/health.png"); //Load imagem da vida
    Texture2D tankplayer = LoadTexture("resources/images/player.png");//Load imagem do tanque do player
    Rectangle screenSizerec = {0, 0, SCREENWIDTH, SCREENHEIGHT}; //Retangulo para a borda do jogo
    Rectangle sourcePlayer = { 0, 0, TAMPLAYERX, TAMPLAYERY}; //Retangulo do tamanho da image com posição 0
    Vector2 centroPlayer = {TAMPLAYERX / 20.0, TAMPLAYERY / 20.0}; //Vetor para achar o centro da imagem do player escalada a 10%
    
    while (!WindowShouldClose() && player.health !=0)
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        //Interface text
        DrawRectangle(0, 0, SCREENWIDTH, 40, DARKGRAY);
        DrawText(TextFormat("Fase %d", fase), SCREENWIDTH/2 - 6*10 , 0, 40, YELLOW);
        DrawRectangleLinesEx(screenSizerec, 5, DARKGRAY);
        DrawText(TextFormat("Score: %i", player.score), SCREENWIDTH / 2 + 215, 5, 32, RED);
        //Draws health for health number
        for (int i = 0, healthx = 5; i < player.health; i++, healthx += 30) 
            DrawTextureEx(healthimg,(Vector2){healthx,5} , 0 , 0.025, WHITE);//Essa imagem é mto grande, achar uma menor escala está ridicula

        move();

        //Player drawing
        //Retangulo movel do player, precisa estar aqui pois caso contrario não conseguimos mover
        Rectangle destPlayer = {player.x, player.y, TAMPLAYERX / 10.0, TAMPLAYERY / 10.0 }; // tamanho da imagem escalado para 10%
        DrawTexturePro(tankplayer, sourcePlayer, destPlayer, centroPlayer, player.rot, WHITE); //Draws player tank
        
        //Variaveis de teste
        if (IsKeyPressed(KEY_H)) //Teste game over por vida
            player.health -= 1;
        if (IsKeyPressed(KEY_K)) //Kill player
            player.health = 0;
        if (IsKeyPressed(KEY_ENTER)) //Teste de score
            player.score += 1;

        EndDrawing();
    }
    UnloadTexture(healthimg);
    UnloadTexture(tankplayer);
    return player.score;
}