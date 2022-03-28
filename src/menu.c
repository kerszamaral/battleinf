#include "raylib.h"
#include "core.h"


Set startscreen(Set settings)
{
    settings.select = 0;
    
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);
        
        DrawText("BATTLEINF", GetScreenWidth() / 2 - MeasureText("BATTLEINF", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4 - 75, 40, LIME);
        
        if ((IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S)) && settings.select < 4)
            settings.select += 1;
        if ((IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)) && settings.select > 0)
            settings.select -= 1;

        if (settings.select == 0)
        {
            DrawText("Start", GetScreenWidth() / 2 - MeasureText("Start", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                break;
            }
        }
        else
            DrawText("Start", GetScreenWidth() / 2 - MeasureText("Start", GetFontDefault().baseSize), GetScreenHeight() / 4, 20, RAYWHITE);

        if (settings.select == 1)
        {
            DrawText("Load", GetScreenWidth() / 2 - MeasureText("Load", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                settings.select = 1;
                break;
            }
        }
        else
            DrawText("Load", GetScreenWidth() / 2 - MeasureText("Load", GetFontDefault().baseSize), GetScreenHeight() / 4 + 50, 20, RAYWHITE);

        if (settings.select == 2)
        {
            DrawText("High Scores", GetScreenWidth() / 2 - MeasureText("High Scores", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 100, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                settings.select = 2;
                break;
            }
        }
        else
            DrawText("High Scores", GetScreenWidth() / 2 - MeasureText("High Scores", GetFontDefault().baseSize), GetScreenHeight() / 4 + 100, 20, RAYWHITE);
        
        if (settings.select == 3)
        {
            DrawText("Settings", GetScreenWidth() / 2 - MeasureText("Settings", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 150, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                settings.select = 3;
                break;
            } 
        }
        else
            DrawText("Settings", GetScreenWidth() / 2 - MeasureText("Settings", GetFontDefault().baseSize), GetScreenHeight() / 4 + 150, 20, RAYWHITE);

        if (settings.select == 4)
        {
            DrawText("Quit", GetScreenWidth() / 2 - MeasureText("Quit", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 200, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                settings.quit = true;
                break;
            } 
        }
        else
            DrawText("Quit", GetScreenWidth() / 2 - MeasureText("Quit", GetFontDefault().baseSize), GetScreenHeight() / 4 + 200, 20, RAYWHITE);
        
        EndDrawing();
    }
    return settings;
}

#define MAX_INPUT_CHARS 9

void nome(Set settings)
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

            DrawText(TextFormat("Seu Score foi: %i", settings.score), GetScreenWidth() / 2 - 
            MeasureText("Seu Score foi: 10000", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4, 40, GRAY);

            DrawText(TextFormat("Sua maior fase foi: %i", settings.level), GetScreenWidth() / 2 - 
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
            if (IsKeyReleased(KEY_ENTER) && letterCount <= MAX_INPUT_CHARS)
            {
                printf( "\nNome: %s    Pontuacao: %d     fase: %d\n\n", name , settings.score , settings.level );
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

Set endscreen(Set settings)
{
    settings.select = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText("GAME OVER", GetScreenWidth() / 2 - MeasureText("GAME OVER", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4 - 75, 40, LIME);
        
        if ( (IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S)) && settings.select < 1 )
            settings.select += 1;
        if ( (IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)) && settings.select > 0 )
            settings.select -= 1;

        if ( settings.select == 0 )
        {
            DrawText("Restart", GetScreenWidth() / 2 - MeasureText("Restart", GetFontDefault().baseSize) * 1.25 , GetScreenHeight() / 4, 25, YELLOW);
            if ( IsKeyReleased(KEY_ENTER) )
            {
                settings.select = 0;
                break;
            }
        }
        else
            DrawText("Restart", GetScreenWidth() / 2 - MeasureText("Restart", GetFontDefault().baseSize) , GetScreenHeight() / 4, 20, RAYWHITE);

        if ( settings.select == 1 )
        {
            DrawText("Quit", GetScreenWidth() / 2 - MeasureText("Quit", GetFontDefault().baseSize) * 1.25 , GetScreenHeight() / 4 + 100, 20, YELLOW);
            if ( IsKeyReleased(KEY_ENTER) )
            {
                settings.quit = true;
                break;
            }
        }
        else
            DrawText("Quit", GetScreenWidth() / 2 - MeasureText("Quit", GetFontDefault().baseSize) , GetScreenHeight() / 4 + 100, 20, RAYWHITE);
        
        EndDrawing();
    }
    return settings;
}

Set settingscreen(Set settings)
{
    settings.select = 0;
    int resoselect = 0;
    bool resolu = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);
        
        DrawText("SETTINGS", GetScreenWidth() / 2 - MeasureText("SETTINGS", GetFontDefault().baseSize) * 2, GetScreenHeight() / 4 - 75, 40, LIME);
        
        if ((IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S)) && settings.select < 4 && !resolu)
            settings.select += 1;
        if ((IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)) && settings.select > 0 && !resolu)
            settings.select -= 1;

        if (settings.select == 0)
        {
            DrawText("Resolution", GetScreenWidth() / 2 - MeasureText("Resolution", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4, 25, YELLOW);
            if ( IsKeyPressed(KEY_RIGHT) )
            {
                resolu = true;
            }
            if ( resolu )
            {
                if (IsKeyPressed(KEY_LEFT))
                    resolu = false;
                if ((IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S)) && resoselect < 2)
                    resoselect += 1;
                if ((IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)) && resoselect > 0)
                    resoselect -= 1;

                if (resoselect == 0)
                {
                    DrawText("1000x600", GetScreenWidth() / 3 * 2 - MeasureText("1000x600", GetFontDefault().baseSize) * 1.25,  GetScreenHeight() / 4, 25, YELLOW);
                    if ( IsKeyPressed(KEY_ENTER) )
                    {
                        if (IsWindowFullscreen())
                            ToggleFullscreen();
                        SetWindowSize(1000,600);
                    }
                }
                else
                    DrawText("1000x600", GetScreenWidth() / 3 * 2 - MeasureText("1000x600", GetFontDefault().baseSize),  GetScreenHeight() / 4, 20, RAYWHITE);
                
                if (resoselect == 1)
                {
                    DrawText("800x450", GetScreenWidth() / 3 * 2 - MeasureText("800x450", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50, 25, YELLOW);
                    if ( IsKeyPressed(KEY_ENTER) )
                    {
                        if (IsWindowFullscreen())
                            ToggleFullscreen();
                        SetWindowSize(800,450);
                    }
                }
                else
                    DrawText("800x450", GetScreenWidth() / 3 * 2 - MeasureText("800x450", GetFontDefault().baseSize), GetScreenHeight() / 4 + 50, 20, RAYWHITE);
                
                if (resoselect == 2)
                {
                    DrawText("Fullscreen", GetScreenWidth() / 3 * 2 - MeasureText("Fullscreen", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 100, 25, YELLOW);
                    if ( IsKeyPressed(KEY_ENTER) )
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

        if (settings.select == 1)
        {
            DrawText("test", GetScreenWidth() / 2 - MeasureText("test", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 50, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                /* code */
            }
        }
        else
            DrawText("test", GetScreenWidth() / 2 - MeasureText("test", GetFontDefault().baseSize), GetScreenHeight() / 4 + 50, 20, RAYWHITE);

        if (settings.select == 2)
        {
            DrawText("test", GetScreenWidth() / 2 - MeasureText("test", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 100, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                /* code */
            }
        }
        else
            DrawText("test", GetScreenWidth() / 2 - MeasureText("test", GetFontDefault().baseSize), GetScreenHeight() / 4 + 100, 20, RAYWHITE);
        
        if (settings.select == 3)
        {
            DrawText("test", GetScreenWidth() / 2 - MeasureText("test", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 150, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                
            } 
        }
        else
            DrawText("test", GetScreenWidth() / 2 - MeasureText("test", GetFontDefault().baseSize), GetScreenHeight() / 4 + 150, 20, RAYWHITE);

        if (settings.select == 4)
        {
            DrawText("Back", GetScreenWidth() / 2 - MeasureText("Back", GetFontDefault().baseSize) * 1.25, GetScreenHeight() / 4 + 200, 25, YELLOW);
            if ( IsKeyReleased(KEY_ENTER) )
            {
                settings.select = 0;
                break;
            } 
        }
        else
            DrawText("Back", GetScreenWidth() / 2 - MeasureText("Back", GetFontDefault().baseSize), GetScreenHeight() / 4 + 200, 20, RAYWHITE);
        
        EndDrawing();
    }
    return settings;
}