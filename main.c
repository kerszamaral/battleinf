/* Ian Kersz - Cartão UFRGS: 00338368
18.03.22
BattleInf Jogo de tanquezinho lá do NES
*/
#include "raylib.h"
#include "core.h"
#include "jogo.h"
#include "menu.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
    Setti settings = { 1 , 0 , 0 , 0 , false , false, BLACK, 0, false, 0 };
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Game");
    InitAudioDevice();
    SetMasterVolume(1);
    SetWindowIcon(LoadImage("assets/player.png"));
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
            
            if (settings.score > LoadStorageValue(0) && !settings.quit)
                nome( &settings );
            settings.exitgame = false;
            settings.level = 0;
            settings.score = 0;
            break;
        case 2:
            highscorescreen( &settings );
            break;
        case 3:
            settingscreen( &settings );
            break;
        default:
            break;
        }
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}