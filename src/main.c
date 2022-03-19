/* Ian Kersz - Cartão UFRGS: 00338368
18.03.22
BattleInf Jogo de tanquezinho lá do NES
*/
#include "raylib.h"
#include "jogo.h"
#include "score.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 450

int main(void)
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Game");
    SetTargetFPS(60);
    
    jogo();
    nome();
    
    CloseWindow();
    return 0;
}