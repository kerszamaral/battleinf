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
    Setti settings = { 1 , 1 , 0 , 0 , false , false, BLACK, RAYWHITE, 0, false, 0, false, 0, 0 };
    strcpy(settings.error, " ");
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Game");
    InitAudioDevice();
    SetMasterVolume(1);
    SetWindowIcon(LoadImage("assets/player.png"));
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    if (fopen("assets/highscores.bin", "rb") == NULL)
    {
        FILE *noscores = fopen("assets/highscores.bin", "wb");
        char madeupscores[15][100] =
        {
            //score     name            level
            "4000\0",   "Marcelo\0",    "3\0",
            "8000\0",   "Pedro\0",      "5\0",
            "8800\0",   "Felipe\0",     "5\0",
            "19200\0",  "Artur\0",      "7\0",
            "46400\0",  "Ian\0",        "11\0"
        };
        fwrite(madeupscores, sizeof(madeupscores), 1, noscores);
        fclose(noscores);
    }
    FILE *highscorescheck = fopen("assets/highscores.bin", "rb");
    char scores[15][100] = {0};
    fread(scores, sizeof(scores), 1, highscorescheck);
    fclose(highscorescheck);
    int lowscore = atoi(scores[0]);

    while ( !settings.quit )
    {
        startscreen( &settings );
        switch ( settings.select )
        {
        case 0:
            settings.loadgame = false;

            do{
                jogo( &settings );
                settings.foundplayerposition = 0;
                settings.enemiesremaining = 0;
                settings.level++;
            }while ( settings.won );
            
            if (!IsWindowFullscreen())
                SetWindowState(FLAG_WINDOW_RESIZABLE);
            SetExitKey(KEY_ESCAPE);
            if (settings.score > lowscore && !settings.quit)
                nome( &settings );

            settings.exitgame = false;
            settings.level = 1;
            settings.score = 0;
            
            break;
        case 1:
            if ( FileExists("saves/savegame.txt") )
            {
                settings.loadgame = true;

                char levelsave[5];
                strncpy( levelsave, &LoadFileText("saves/savegame.txt")[615], 4 );
                settings.level = atoi(levelsave);

                do{
                    jogo( &settings );
                    settings.level++;
                    settings.foundplayerposition = 0;
                    settings.enemiesremaining = 0;
                    settings.loadgame = false;
                }while ( settings.won );
                
                if (!IsWindowFullscreen())
                    SetWindowState(FLAG_WINDOW_RESIZABLE);
                SetExitKey(KEY_ESCAPE);
                if (settings.score > lowscore && !settings.quit)
                    nome( &settings );
                
                settings.exitgame = false;
                settings.level = 1;
                settings.score = 0;    
            }
            else
                strcpy(settings.error, "Não há nenhum jogo salvo!");
            
            break;
        case 2:
            break;
        case 3:
            highscorescreen( &settings );
            break;
        case 4:
            settingscreen( &settings );
            break;
        case 5:
            settings.quit = true;
            break;
        default:
            break;
        }
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}