#include "raylib.h"
#include "core.h"


int startscreen(void)
{
    int select = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);
        
        DrawText("BATTLEINF", SCREENWIDTH / 2 - MeasureText("BATTLEINF", GetFontDefault().baseSize) * 2, SCREENHEIGHT / 4 - 75, 40, LIME);
        
        if ((IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S)) && select < 3)
            select += 1;
        if ((IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)) && select > 0)
            select -= 1;

        if (select == 0)
        {
            DrawText("Start", SCREENWIDTH / 2 - MeasureText("Start", GetFontDefault().baseSize) * 1.25, SCREENHEIGHT / 4, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                break;
            }
        }
        else
            DrawText("Start", SCREENWIDTH / 2 - MeasureText("Start", GetFontDefault().baseSize), SCREENHEIGHT / 4, 20, RAYWHITE);

        if (select == 1)
        {
            DrawText("Continue", SCREENWIDTH / 2 - MeasureText("Continue", GetFontDefault().baseSize) * 1.25, SCREENHEIGHT / 4 + 50, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                /* code */
            }
        }
        else
            DrawText("Continue", SCREENWIDTH / 2 - MeasureText("Continue", GetFontDefault().baseSize), SCREENHEIGHT / 4 + 50, 20, RAYWHITE);

        if (select == 2)
        {
            DrawText("High Scores", SCREENWIDTH / 2 - MeasureText("High Scores", GetFontDefault().baseSize) * 1.25, SCREENHEIGHT / 4 + 100, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                /* code */
            }
        }
        else
            DrawText("High Scores", SCREENWIDTH / 2 - MeasureText("High Scores", GetFontDefault().baseSize), SCREENHEIGHT / 4 + 100, 20, RAYWHITE);

        if (select == 3)
        {
            DrawText("Quit", SCREENWIDTH / 2 - MeasureText("Quit", GetFontDefault().baseSize) * 1.25, SCREENHEIGHT / 4 + 150, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                select = 5;
                break;
            } 
        }
        else
            DrawText("Quit", SCREENWIDTH / 2 - MeasureText("Quit", GetFontDefault().baseSize), SCREENHEIGHT / 4 + 150, 20, RAYWHITE);
        
        EndDrawing();
    }
    return select;
}

#define MAX_INPUT_CHARS 9

void nome(int pscore)
{
    char name[MAX_INPUT_CHARS + 1] = "\0"; //+1 de espaço para o \0
    int letterCount = 0;
    Rectangle textBox = { SCREENWIDTH/2.0f - 100, 220, 225, 50 };


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

            DrawText("Digite seu nome para registrar sua Pontuação!", SCREENWIDTH / 2 - 
            MeasureText("Digite seu nome para registrar sua Pontuação!", GetFontDefault().baseSize), SCREENHEIGHT / 4 - 50, 20, GRAY);

            DrawText(TextFormat("Seu Score foi: %i", pscore), SCREENWIDTH / 2 - 
            MeasureText("Seu Score foi: 10000", GetFontDefault().baseSize) * 2, SCREENHEIGHT / 4, 40, GRAY);

            DrawRectangleRec(textBox, LIGHTGRAY);

            DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

            DrawText(TextFormat("Caracteres restantes: %i/%i", letterCount, MAX_INPUT_CHARS), SCREENWIDTH / 2 - 
            MeasureText("Caracteres restantes: 9/9", GetFontDefault().baseSize), 300, 20, DARKGRAY);

            if (letterCount < MAX_INPUT_CHARS)
            {
                DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
            }
            else DrawText("Pressione BACKSPACE para deletar", 230, 300, 20, GRAY);
            if (IsKeyReleased(KEY_ENTER) && letterCount <= MAX_INPUT_CHARS)
            {
                printf("%s\n", name);
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

int endscreen(void)
{
    int select = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText("GAME OVER", SCREENWIDTH / 2 - MeasureText("GAME OVER", GetFontDefault().baseSize) * 2, SCREENHEIGHT / 4 - 75, 40, LIME);
        
        if ( (IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S)) && select < 1 )
            select += 1;
        if ( (IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)) && select > 0 )
            select -= 1;

        if ( select == 0 )
        {
            DrawText("Restart", SCREENWIDTH / 2 - MeasureText("Restart", GetFontDefault().baseSize) * 1.25 , SCREENHEIGHT / 4, 25, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                select = 0;
                break;
            }
        }
        else
            DrawText("Restart", SCREENWIDTH / 2 - MeasureText("Restart", GetFontDefault().baseSize) , SCREENHEIGHT / 4, 20, RAYWHITE);

        if ( select == 1 )
        {
            DrawText("Quit", SCREENWIDTH / 2 - MeasureText("Quit", GetFontDefault().baseSize) * 1.25 , SCREENHEIGHT / 4 + 100, 20, YELLOW);
            if ( IsKeyPressed(KEY_ENTER) )
            {
                select = 5;
                break;
            }
        }
        else
            DrawText("Quit", SCREENWIDTH / 2 - MeasureText("Quit", GetFontDefault().baseSize) , SCREENHEIGHT / 4 + 100, 20, RAYWHITE);
        
        EndDrawing();
    }
    return select;
}