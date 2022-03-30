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
    Setti settings = { 1 , 0 , 0 , 0 , false , false };
    int lscore = 1600;
    Image logo = LoadImage("resources/images/player.png");
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Game");
    SetWindowIcon(logo);
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    while ( !settings.quit )
    {
        startscreen( &settings );
        switch ( settings.select )
        {
        case 0:
            do{
                settings.level++;
                jogo( &settings );
            }while ( settings.won );

            if (!IsWindowFullscreen())
                SetWindowState(FLAG_WINDOW_RESIZABLE);
            
            if (settings.score > lscore) //Quando fizermos load no arquivo de scores passados ele vai ver se é maior que o ultimo
                nome( &settings );
            settings.level = 0;
            settings.score = 0;
            endscreen( &settings );
            break;
        case 3:
            settingscreen( &settings );
            break;
        default:
            break;
        }
    }

    UnloadImage(logo);
    CloseWindow();
    return 0;
}