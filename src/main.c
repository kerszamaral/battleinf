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
    Vector4 gamestate = { 0 , 0 , 0 , 0};
    int lscore = 1600;
    Image logo = LoadImage("resources/images/player.png");
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Game");
    SetWindowIcon(logo);
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    while ( gamestate.x != 5 )
    {
        gamestate.x = startscreen();
        switch ( (int)gamestate.x )
        {
        case 0:
            do{
                gamestate.z++;
                gamestate = jogo(gamestate);
            }while ( gamestate.w == 1 );
            
            if (!IsWindowFullscreen())
                SetWindowState(FLAG_WINDOW_RESIZABLE);
            
            if (gamestate.y > lscore) //Quando fizermos load no arquivo de scores passados ele vai ver se é maior que o ultimo
                nome(gamestate.y, gamestate.z);
            gamestate.z = 0;
            gamestate.y = 0;
            gamestate.x = endscreen();
            break;
        case 3:
            gamestate.x = settingscreen();
            break;
        default:
            break;
        }
    }
    UnloadImage(logo);
    CloseWindow();
    return 0;
}