#include "raylib.h"
#include "core.h"


void startscreen(Setti *settings)
{
    settings->select = 0;
    
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);
        
        DrawText("BATTLEINF", GetScreenWidth() / 2 - MeasureText("BATTLEINF", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4 - 75, 40, LIME);
        
        if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->select < 4)
            settings->select += 1;
        if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && settings->select > 0)
            settings->select -= 1;

        if (settings->select == 0)
        {
            DrawText("Start", GetScreenWidth() / 2 - MeasureText("Start", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4, 25, YELLOW);
            //if ( IsKeyUp(KEY_ENTER))
                if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
                    break;
        }
        else
            DrawText("Start", GetScreenWidth() / 2 - MeasureText("Start", GetFontDefault().baseSize), GetScreenHeight() / 4, 20, RAYWHITE);

        if (settings->select == 1)
        {
            DrawText("Load", GetScreenWidth() / 2 - MeasureText("Load", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
            {
                settings->select = 1;
                break;
            }
        }
        else
            DrawText("Load", GetScreenWidth() / 2 - MeasureText("Load", GetFontDefault().baseSize), GetScreenHeight() / 4 + 50, 20, RAYWHITE);

        if (settings->select == 2)
        {
            DrawText("High Scores", GetScreenWidth() / 2 - MeasureText("High Scores", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 100, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
            {
                settings->select = 2;
                break;
            }
        }
        else
            DrawText("High Scores", GetScreenWidth() / 2 - MeasureText("High Scores", GetFontDefault().baseSize), GetScreenHeight() / 4 + 100, 20, RAYWHITE);
        
        if (settings->select == 3)
        {
            DrawText("Settings", GetScreenWidth() / 2 - MeasureText("Settings", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 150, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
            {
                settings->select = 3;
                break;
            } 
        }
        else
            DrawText("Settings", GetScreenWidth() / 2 - MeasureText("Settings", GetFontDefault().baseSize), GetScreenHeight() / 4 + 150, 20, RAYWHITE);

        if (settings->select == 4)
        {
            DrawText("Quit", GetScreenWidth() / 2 - MeasureText("Quit", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 200, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, 7) || IsGamepadButtonPressed(0, 12) )
            {
                settings->quit = true;
                break;
            } 
        }
        else
            DrawText("Quit", GetScreenWidth() / 2 - MeasureText("Quit", GetFontDefault().baseSize), GetScreenHeight() / 4 + 200, 20, RAYWHITE);
        
        EndDrawing();
    }
}

#define MAX_INPUT_CHARS 9

void nome(Setti *settings)
{
    char name[MAX_INPUT_CHARS + 1] = "\0"; //+1 de espaço para o \0
    int letterCount = 0;
    Rectangle textBox = { GetScreenWidth()/2.0f - 100, 320, 225, 50 };


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
            {
                printf( "\nNome: %s    Pontuacao: %d     fase: %d\n\n", name , settings->score , settings->level );
                //write to file "name", remove last place
                break;
            }
            
        EndDrawing();
    }
    
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

void pausescreen(Setti *settings)
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
            settings->pauseselect = 1;
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