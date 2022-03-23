/* Ian Kersz - Cartão UFRGS: 00338368
18.03.22
BattleInf Jogo de tanquezinho lá do NES
*/
#include "raylib.h"
#include "core.h"
#include "jogo.h"
#include "menu.h"

int main(void)
{
    int score = 0, lscore = 1600, select = 0;
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Game");
    SetTargetFPS(60);

    
    while (select == 0)
    {
        select = startscreen();
        if (select == 0)
        {
            score = jogo();
            if (score > lscore) //Quando fizermos load no arquivo de scores passados ele vai ver se é maior que o ultimo
                nome(score);
            select = endscreen();
        }
    }
    
    CloseWindow();
    return 0;
}