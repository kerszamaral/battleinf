#include "raylib.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 450


void startscreen(void)
{
    int select = 0;
    Font fontTtf = LoadFontEx("resources/fonts/Roboto-Regular.ttf", 32, 0, 250); //Load da fonte roboto
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawTextEx(fontTtf, "BATTLEINF", (Vector2){ SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 - 50 }, 40, 2, LIME);
        
        if (IsKeyPressed(KEY_DOWN) && select < 2)
            select += 1;
        if (IsKeyPressed(KEY_UP) && select > 0)
            select -= 1;

        if (select == 0)
        {
            DrawTextEx(fontTtf, "Start", (Vector2){ SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 }, 20, 2, YELLOW);
            if (IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_ENTER))
            {
                break;
            }
        }
        else
            DrawTextEx(fontTtf, "Start", (Vector2){ SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 }, 20, 2, RAYWHITE);


        if (select == 1)
        {
            DrawTextEx(fontTtf, "Continue", (Vector2){ SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 + 50 }, 20, 2, YELLOW);
            if (IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_ENTER))
            {
                /* code */
            }
            
        }
        else
            DrawTextEx(fontTtf, "Continue", (Vector2){ SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 + 50 }, 20, 2, RAYWHITE);

        if (select == 2)
        {
            DrawTextEx(fontTtf, "Quit", (Vector2){ SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 + 100 }, 20, 2, YELLOW);
            if (IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_ENTER))
            {
                UnloadFont(fontTtf);
                CloseWindow();
            }
            
        }
        else
            DrawTextEx(fontTtf, "Quit", (Vector2){ SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 + 100 }, 20, 2, RAYWHITE);
        
        EndDrawing();
    }
    UnloadFont(fontTtf);
}