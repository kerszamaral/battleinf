#include "raylib.h"
#include "core.h"
#include <string.h>
#include "saveload.h"


void startscreen(Setti *settings)
{
    /***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false;
    settings->select = 0;
    double time = GetTime();
    Color lettercolor = BLACK;
    if (ColorToInt(settings->theme) == ColorToInt(BLACK))
        lettercolor = RAYWHITE;
    
    int optionsnumber = 6;
    char options[6][100] = {
        "Start\0",
        "Continue\0",
        "Load\0",
        "High Scores\0",
        "Settings\0",
        "Quit\0"
    };
    /***************** EFFECTS *********************************/
    Textus textures;
    textures.player = LoadTexture("assets/player.png");  //Texture for the player tank
    textures.bullet = LoadTexture("assets/bullet.png"); //Texture for the bullet
    textures.explosion = LoadTexture("assets/explosionBullets.png"); //Texture for the explosion for bullets
    textures.smoke = LoadTexture("assets/fire.png"); //Smoke texture
    SFX sounds;
    sounds.shoot =  LoadSound("assets/BulletShotSFX.wav"); //Sound for the shoot
    sounds.bulletmiss = LoadSound("assets/BulletMissSFX.wav"); //Sound for the bullet miss
    SetSoundVolume(sounds.shoot, 0.02);
    SetSoundVolume(sounds.bulletmiss, 0.02);
    bool bulletdying = false;
    Vector2 bulletexplosion = {0,0};
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0;

    while ( !settings->quit )
    {
        Rectangle Menu[4] = {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50 * (GetScreenHeight()*(1.0/655)) }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*(GetScreenHeight()*(1.0/655)) , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5 * (GetScreenWidth()*(1.0/1010)), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5 * (GetScreenWidth()*(1.0/1010)) , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };
        
        Rectangle playerdrawRec = { 
            GetScreenWidth() / 2 - MeasureText(&options[settings->select][0], GetFontDefault().baseSize) * (GetScreenHeight()*(1.0/655)) - 10, 
            GetScreenHeight() / 4 + 50 * settings->select * (GetScreenHeight()*(1.0/655)) - 5, 
            textures.player.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ), 
            ( textures.player.height * textures.player.width / textures.player.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ) 
        }, bulletdrawRec;

        if( !selected && !shoot && !bulletdying && GetTime() > time + 0.5)
        {
            if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->select < optionsnumber - 1)
                settings->select += 1;
            if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && settings->select > 0)
                settings->select -= 1;
            if ( IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
                selected = true;
        }

        if ( selected )
        {
            shoot = true;
            PlaySoundMulti(sounds.shoot);
            bulletdrawRec = (Rectangle){ 
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 100 ), 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 100 ), 
                    textures.bullet.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ), 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ) 
                };
            selected = false;
        }
        
        if ( shoot )
        {
            if (bulletsmoke > 20)
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + 10 + playerdrawRec.width / 2 + MeasureText(&options[settings->select][0], GetFontDefault().baseSize) * (GetScreenHeight()*(1.0/655)) * 2 )
        {
            bulletexplosion.x = bulletdrawRec.x;
            bulletexplosion.y = bulletdrawRec.y + 5;
            bulletdying = true;
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 };
            bulletdeathtimer = 0;
            shoot = false;
            PlaySoundMulti(sounds.bulletmiss);
        }
        if ( bulletdying )
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        if (bulletdeathtimer > 39)
        {
            bulletexplosion.x = GetScreenWidth()*2;
            bulletexplosion.y = 0;
            bulletdying = false;
            break;
        }
        
        BeginDrawing();

        ClearBackground( settings->theme );

        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars

        DrawText( "BATTLEINF", GetScreenWidth() / 2 - MeasureText("BATTLEINF", GetFontDefault().baseSize) * 2 * (GetScreenHeight()*(1.0/655)) , 10*(GetScreenHeight()*(1.0/655)) , 40*(GetScreenHeight()*(1.0/655)) , LIME );
        
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        
        for (int i = 0; i < optionsnumber; i++)
            DrawText( &options[i][0], GetScreenWidth() / 2 - MeasureText(&options[i][0], GetFontDefault().baseSize), GetScreenHeight() / 4 + 50 * i * (GetScreenHeight()*(1.0/655)), 20*(GetScreenHeight()*(1.0/655)), lettercolor );
        
        //This was a bug that made the smoke spin but I kept it as a feature becaue it looked nice doubled
        DrawTexturePro( textures.smoke , (Rectangle){ bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.smoke , (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        
        DrawTexturePro( textures.bullet , (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );

        if ( strcmp(settings->error, " ") )
        {
            DrawText(TextFormat("Error: %s", settings->error), GetScreenWidth() / 2 - MeasureText(TextFormat("Error: %s", settings->error), GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 300, 25, RED);
            if (GetTime() > time + 2)
                strcpy(settings->error, " ");
        }

        EndDrawing();

        if( WindowShouldClose() )
        {
            settings->select = 5;
            settings->quit = true;
        }
    }

    StopSoundMulti();
    UnloadTexture(textures.player);  //Texture for the player tank
    UnloadTexture(textures.bullet); //Texture for the bullet
    UnloadTexture(textures.explosion); //Texture for the explosion for bullets
    UnloadTexture(textures.smoke); //Smoke texture
    UnloadSound(sounds.shoot); //Sound for shooting
    UnloadSound(sounds.bulletmiss); //Sound for explosion
}

#define MAX_INPUT_CHARS 9

void nome(Setti *settings)
{
    char name[MAX_INPUT_CHARS + 1] = "\0"; //+1 de espaço para o \0
    int letterCount = 0;
    Rectangle textBox = { GetScreenWidth()/2.0f - 100, 320, 225, 50 };
    
    int ranking = 6;

    for (int i = 0; i < 5; i++)
    {
        if (settings->score > LoadStorageValue(0 + i*12))
        {
            SaveStorageValue(0 + i*12, LoadStorageValue(12 + i*12));
            SaveStorageValue(1 + i*12, LoadStorageValue(13 + i*12));
            for (int j = 0; j < 10; j++)
                SaveStorageValue(2 + i*12 + j, LoadStorageValue(14 + i*12 + j));
            ranking--;
        }
    }

    while (!WindowShouldClose())
    {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
            {
                name[letterCount] = (char)key;
                name[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                letterCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            name[letterCount] = '\0';
        }

        BeginDrawing();

            ClearBackground(BLACK);

            DrawText(TextFormat("Seu Ranking: %i", ranking), GetScreenWidth() / 2 - 
            MeasureText("Seu Ranking: 5", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4 - 100, 40, GRAY);

            DrawText("Digite seu nome para registrar sua Pontuação!", GetScreenWidth() / 2 - 
            MeasureText("Digite seu nome para registrar sua Pontuação!", GetFontDefault().baseSize), GetScreenHeight() / 4 - 50, 20, GRAY);

            DrawText(TextFormat("Seu Score foi: %i", settings->score), GetScreenWidth() / 2 - 
            MeasureText("Seu Score foi: 10000", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4, 40, GRAY);

            DrawText(TextFormat("Sua maior fase foi: %i", settings->level), GetScreenWidth() / 2 - 
            MeasureText("Sua maior fase foi: 100", GetFontDefault().baseSize) * 2, 220, 40, GRAY);

            DrawRectangleRec(textBox, LIGHTGRAY);

            DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

            DrawText(TextFormat("Caracteres restantes: %i/%i", letterCount, MAX_INPUT_CHARS), GetScreenWidth() / 2 - 
            MeasureText("Caracteres restantes: 9/9", GetFontDefault().baseSize), 400, 20, DARKGRAY);

            if (letterCount < MAX_INPUT_CHARS)
            {
                DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
            }
            else DrawText("Pressione BACKSPACE para deletar", 230, 400, 20, GRAY);
            if ((IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12)) && letterCount <= MAX_INPUT_CHARS)
                break;
            
        EndDrawing();
    }
    int id = 5 - ranking;

    SaveStorageValue(0 + id*12, settings->score);
    SaveStorageValue(1 + id*12, settings->level);
    for (int i = 0; i < 10; i++)
        SaveStorageValue(2 + id*12 + i, name[i]);
}

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}

void endscreen(Setti *settings)
{
    settings->select = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText("GAME OVER", GetScreenWidth() / 2 - MeasureText("GAME OVER", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4 - 75, 40, LIME);
        
        if ( (IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3) ) && settings->select < 1 )
            settings->select += 1;
        if ( (IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W) || IsGamepadButtonReleased(0, 1) ) && settings->select > 0 )
            settings->select -= 1;

        if ( settings->select == 0 )
        {
            DrawText("Restart", GetScreenWidth() / 2 - MeasureText("Restart", GetFontDefault().baseSize) * 1.25 , GetScreenHeight() / 4, 25, YELLOW);
            if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
            {
                settings->select = 0;
                break;
            }
        }
        else
            DrawText("Restart", GetScreenWidth() / 2 - MeasureText("Restart", GetFontDefault().baseSize) , GetScreenHeight() / 4, 20, RAYWHITE);

        if ( settings->select == 1 )
        {
            DrawText("Quit", GetScreenWidth() / 2 - MeasureText("Quit", GetFontDefault().baseSize) * 1.25 , GetScreenHeight() / 4 + 100, 20, YELLOW);
            if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
            {
                settings->quit = true;
                break;
            }
        }
        else
            DrawText("Quit", GetScreenWidth() / 2 - MeasureText("Quit", GetFontDefault().baseSize) , GetScreenHeight() / 4 + 100, 20, RAYWHITE);
        
        EndDrawing();
    }
}

void settingscreen(Setti *settings)
{
    settings->select = 0;
    int submenuselect = 0;
    bool submenu = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);
        
        DrawText("Settings", GetScreenWidth() / 2 - MeasureText("Settings", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4 - 75, 40, LIME);
        if (!submenu)
            submenuselect = 0;

        if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->select < 4 && !submenu)
            settings->select += 1;
        if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W) || IsGamepadButtonReleased(0, 1)  ) && settings->select > 0 && !submenu)
            settings->select -= 1;

        if (settings->select == 0)
        {
            DrawText("Resolution", GetScreenWidth() / 2 - MeasureText("Resolution", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4, 25, YELLOW);
            if ( IsKeyPressed(KEY_RIGHT) || IsGamepadButtonPressed(0, 2) )
            {
                submenu = true;
            }
            if ( submenu )
            {
                if ( IsKeyPressed(KEY_LEFT) || IsGamepadButtonPressed(0, 4))
                    submenu = false;
                if ((IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && submenuselect < 2)
                    submenuselect += 1;
                if ((IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W) || IsGamepadButtonReleased(0, 1)  ) && submenuselect > 0)
                    submenuselect -= 1;

                if (submenuselect == 0)
                {
                    DrawText("1000x600", GetScreenWidth() / 3 * 2 - MeasureText("1000x600", GetFontDefault().baseSize) * 1.25,  GetScreenHeight() / 4, 25, YELLOW);
                    if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
                    {
                        if (IsWindowFullscreen())
                            ToggleFullscreen();
                        SetWindowSize(1000,600);
                    }
                }
                else
                    DrawText("1000x600", GetScreenWidth() / 3 * 2 - MeasureText("1000x600", GetFontDefault().baseSize),  GetScreenHeight() / 4, 20, RAYWHITE);
                
                if (submenuselect == 1)
                {
                    DrawText("800x450", GetScreenWidth() / 3 * 2 - MeasureText("800x450", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50, 25, YELLOW);
                    if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
                    {
                        if (IsWindowFullscreen())
                            ToggleFullscreen();
                        SetWindowSize(800,450);
                    }
                }
                else
                    DrawText("800x450", GetScreenWidth() / 3 * 2 - MeasureText("800x450", GetFontDefault().baseSize), GetScreenHeight() / 4 + 50, 20, RAYWHITE);
                
                if (submenuselect == 2)
                {
                    DrawText("Fullscreen", GetScreenWidth() / 3 * 2 - MeasureText("Fullscreen", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 100, 25, YELLOW);
                    if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
                    {
                        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()),GetMonitorHeight(GetCurrentMonitor()));
                        ToggleFullscreen();
                    }
                }
                else
                    DrawText("Fullscreen", GetScreenWidth() / 3 * 2 - MeasureText("Fullscreen", GetFontDefault().baseSize), GetScreenHeight() / 4 + 100, 20, RAYWHITE);
            }
            
        }
        else
            DrawText("Resolution", GetScreenWidth() / 2 - MeasureText("Resolution", GetFontDefault().baseSize), GetScreenHeight() / 4, 20, RAYWHITE);

        if (settings->select == 1)
        {
            DrawText("Number of Players", GetScreenWidth() / 2 - MeasureText("Number of Players", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50, 25, YELLOW);
            if ( IsKeyPressed(KEY_RIGHT) || IsGamepadButtonPressed(0, 2))
                submenu = true;
            
            if ( submenu )
            {
                if ( IsKeyPressed(KEY_LEFT) || IsGamepadButtonPressed(0, 4))
                    submenu = false;

                if ((IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W) || IsGamepadButtonReleased(0, 1)  ) && settings->players < 5) //!INCREASE THIS NUMBER TO INCREASE MAXIMUM POSSIBLE PLAYERS
                {
                    DrawText("+", GetScreenWidth() / 3 * 2 - MeasureText("+", GetFontDefault().baseSize) * 1.25,  GetScreenHeight() / 4, 25, YELLOW);
                    settings->players++;
                }
                else
                    DrawText("+", GetScreenWidth() / 3 * 2 - MeasureText("+", GetFontDefault().baseSize),  GetScreenHeight() / 4, 20, RAYWHITE);
                
                DrawText(TextFormat("%d",settings->players), GetScreenWidth() / 3 * 2 - MeasureText(TextFormat("%d",settings->players), GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50, 25, YELLOW);
                
                if ((IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->players > 1)
                {
                    DrawText("-", GetScreenWidth() / 3 * 2 - MeasureText("-", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 100, 25, YELLOW);
                    settings->players--;
                }
                else
                    DrawText("-", GetScreenWidth() / 3 * 2 - MeasureText("-", GetFontDefault().baseSize), GetScreenHeight() / 4 + 100, 20, RAYWHITE);
            }
        }
        else
            DrawText("Number of Players", GetScreenWidth() / 2 - MeasureText("Number of Players", GetFontDefault().baseSize), GetScreenHeight() / 4 + 50, 20, RAYWHITE);

        if (settings->select == 2)
        {
            DrawText("Theme", GetScreenWidth() / 2 - MeasureText("Theme", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 100, 25, YELLOW);
            if ( IsKeyPressed(KEY_RIGHT) || IsGamepadButtonPressed(0, 2) )
            {
                submenu = true;
            }
            if ( submenu )
            {
                if (IsKeyPressed(KEY_LEFT)|| IsGamepadButtonPressed(0, 4))
                    submenu = false;
                if ((IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3) ) && submenuselect < 1)
                    submenuselect += 1;
                if ((IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W) || IsGamepadButtonReleased(0, 1)  ) && submenuselect > 0)
                    submenuselect -= 1;

                if (submenuselect == 0)
                {
                    DrawText("Night", GetScreenWidth() / 3 * 2 - MeasureText("Night", GetFontDefault().baseSize) * 1.25,  GetScreenHeight() / 4 + 100, 25, YELLOW);
                    if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
                    {
                        settings->theme = BLACK;
                    }
                }
                else
                    DrawText("Night", GetScreenWidth() / 3 * 2 - MeasureText("Night", GetFontDefault().baseSize),  GetScreenHeight() / 4 + 100, 20, RAYWHITE);
                
                if (submenuselect == 1)
                {
                    DrawText("Day", GetScreenWidth() / 3 * 2 - MeasureText("Day", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 150, 25, YELLOW);
                    if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
                    {
                        settings->theme = RAYWHITE;
                    }
                }
                else
                    DrawText("Day", GetScreenWidth() / 3 * 2 - MeasureText("Day", GetFontDefault().baseSize), GetScreenHeight() / 4 + 150, 20, RAYWHITE);
            }
        }
        else
            DrawText("Theme", GetScreenWidth() / 2 - MeasureText("Theme", GetFontDefault().baseSize), GetScreenHeight() / 4 + 100, 20, RAYWHITE);
        
        if (settings->select == 3)
        {
            DrawText("Extended Play", GetScreenWidth() / 2 - MeasureText("Extended Play", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 150, 25, YELLOW);
            if ( IsKeyPressed(KEY_RIGHT) || IsGamepadButtonPressed(0, 2))
                submenu = true;
            
            if ( submenu )
            {
                if ( IsKeyPressed(KEY_LEFT) || IsGamepadButtonPressed(0, 4))
                    submenu = false;

                if ((IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W) || IsGamepadButtonReleased(0, 1)  ) && settings->extended < 3) //!INCREASE THIS NUMBER TO INCREASE MAXIMUM POSSIBLE PLAYERS
                {
                    DrawText("+", GetScreenWidth() / 3 * 2 - MeasureText("+", GetFontDefault().baseSize) * 1.25,  GetScreenHeight() / 4, 25, YELLOW);
                    settings->extended++;
                }
                else
                    DrawText("+", GetScreenWidth() / 3 * 2 - MeasureText("+", GetFontDefault().baseSize),  GetScreenHeight() / 4, 20, RAYWHITE);
                
                DrawText(TextFormat("%d",settings->extended), GetScreenWidth() / 3 * 2 - MeasureText(TextFormat("%d",settings->extended), GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50, 25, YELLOW);
                
                if ((IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->extended > 0)
                {
                    DrawText("-", GetScreenWidth() / 3 * 2 - MeasureText("-", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 100, 25, YELLOW);
                    settings->extended--;
                }
                else
                    DrawText("-", GetScreenWidth() / 3 * 2 - MeasureText("-", GetFontDefault().baseSize), GetScreenHeight() / 4 + 100, 20, RAYWHITE);
            }
        }
        else
            DrawText("Extended Play", GetScreenWidth() / 2 - MeasureText("Extended Play", GetFontDefault().baseSize), GetScreenHeight() / 4 + 150, 20, RAYWHITE);

        if (settings->select == 4)
        {
            DrawText("Back", GetScreenWidth() / 2 - MeasureText("Back", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 200, 25, YELLOW);
            if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
            {
                settings->select = 0;
                break;
            } 
        }
        else
            DrawText("Back", GetScreenWidth() / 2 - MeasureText("Back", GetFontDefault().baseSize), GetScreenHeight() / 4 + 200, 20, RAYWHITE);
        
        EndDrawing();
    }
}

void pausescreen(Setti *settings, char terrainspace[], Obj player[], Obj enemy[] , Obj *energy, Obj bullet[], Rectangle terrainarray[] )
{
  
    DrawText("Paused", GetScreenWidth() / 2 - MeasureText("Paused", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4, 40, YELLOW);
    
    if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->pauseselect < 3 )
        settings->pauseselect += 1;
    if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W) || IsGamepadButtonReleased(0, 1)  ) && settings->pauseselect > 0)
        settings->pauseselect -= 1;
    
    if (settings->pauseselect == 0)
    {
        DrawText("Resume", GetScreenWidth() / 2 - MeasureText("Resume", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50, 25, YELLOW);
        if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
        {
            settings->pause = false;
        }
    }
    else
        DrawText("Resume", GetScreenWidth() / 2 - MeasureText("Resume", GetFontDefault().baseSize), GetScreenHeight() / 4 + 50, 20, RAYWHITE);
    
    if (settings->pauseselect == 1)
    {
        DrawText("Save", GetScreenWidth() / 2 - MeasureText("Save", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 100, 25, YELLOW);
        if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
        {
            saving(settings, terrainspace, player, enemy, energy, bullet, terrainarray);
        }
    }
    else
        DrawText("Save", GetScreenWidth() / 2 - MeasureText("Save", GetFontDefault().baseSize), GetScreenHeight() / 4 + 100, 20, RAYWHITE);
    
    if (settings->pauseselect == 2)
    {
        DrawText("Main Menu", GetScreenWidth() / 2 - MeasureText("Main Menu", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 150, 25, YELLOW);
        if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
        {
            settings->exitgame = true;
            settings->pause = false;
        }
    }
    else
        DrawText("Main Menu", GetScreenWidth() / 2 - MeasureText("Main Menu", GetFontDefault().baseSize), GetScreenHeight() / 4 + 150, 20, RAYWHITE);
    
    if (settings->pauseselect == 3)
    {
        DrawText("Quit Game", GetScreenWidth() / 2 - MeasureText("Quit Game", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 200, 25, YELLOW);
        if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
        {
            settings->quit = true;
            settings->exitgame = true;
            settings->pause = false;
        }
    }
    else
        DrawText("Quit Game", GetScreenWidth() / 2 - MeasureText("Quit Game", GetFontDefault().baseSize), GetScreenHeight() / 4 + 200, 20, RAYWHITE);
    
}

void highscorescreen(Setti *settings)
{
    Score pscores[5];

    for (int i = 0; i < 5; i++)
    {
        pscores[i].score = LoadStorageValue(48 - i*12);
        pscores[i].level = LoadStorageValue(49 - i*12);
        for (int j = 0; j < 10; j++)
            pscores[i].name[j] = LoadStorageValue(50 + j - i*12 );
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);
        DrawText("Highscores", GetScreenWidth() / 2 - MeasureText("Highscores", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4 - 100, 40, YELLOW);
        
        DrawText("Rº", GetScreenWidth() / 3 - MeasureText("Rº", GetFontDefault().baseSize) * 1.25 - 200, GetScreenHeight() / 4 - 50 , 25, GOLD);
        DrawText("Score", GetScreenWidth() / 3 - MeasureText("Score", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 - 50 , 25, GREEN);
        DrawText("Name", GetScreenWidth() / 2 - MeasureText("Name", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 - 50 , 25, YELLOW);
        DrawText("Level", (GetScreenWidth() - GetScreenWidth() / 4) - MeasureText("Level", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 - 50 , 25, BLUE);

        for (int i = 0; i < 5; i++)
        {
            DrawText(TextFormat("%d", i+1), GetScreenWidth() / 3 - MeasureText(TextFormat("%d", i+1), GetFontDefault().baseSize) * 1.25 - 200, GetScreenHeight() / 4 + 50 + i * 50, 25, GOLD);
            DrawText(TextFormat("%d", pscores[i].score), GetScreenWidth() / 3 - MeasureText(TextFormat("%d", pscores[i].score), GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50 + i * 50, 25, WHITE);
            DrawText(TextFormat("%s", pscores[i].name), GetScreenWidth() / 2 - MeasureText(TextFormat("%s", pscores[i].name), GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50 + i * 50, 25, WHITE);
            DrawText(TextFormat("%d", pscores[i].level), (GetScreenWidth() - GetScreenWidth() / 4) - MeasureText(TextFormat("%d", pscores[i].level), GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50 + i * 50, 25, WHITE);
        }

        DrawText("Back", GetScreenWidth() / 2 - MeasureText("Back", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 400, 25, YELLOW);
        if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0,12) )
        {
            settings->select = 0;
            break;
        } 

        EndDrawing();
    }
}