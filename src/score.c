#include "raylib.h"
#include "score.h"
#include <stdio.h>

#define MAX_INPUT_CHARS 9
#define SCREENWIDTH 800
#define SCREENHEIGHT 450

void nome(int pscore)
{
    char name[MAX_INPUT_CHARS + 1] = "\0"; //+1 de espaço para o \0
    int letterCount = 0;
    Rectangle textBox = { SCREENWIDTH/2.0f - 100, 180, 225, 50 };


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

            DrawText("Digite seu nome para registrar sua Pontuação!", 180, SCREENHEIGHT / 4 - 50, 20, GRAY);
            DrawText(TextFormat("Seu Score foi: %i", pscore), SCREENWIDTH / 2 - 150, SCREENHEIGHT / 4, 40, GRAY);

            DrawRectangleRec(textBox, LIGHTGRAY);

            DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

            DrawText(TextFormat("Caracteres restantes: %i/%i", letterCount, MAX_INPUT_CHARS), 275, 250, 20, DARKGRAY);

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