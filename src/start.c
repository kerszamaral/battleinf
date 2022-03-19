#include "raylib.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 450


int startscreen(void)
{
    int select = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText("BATTLEINF", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 - 50, 40, LIME);
        
        if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && select < 2)
            select += 1;
        if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && select > 0)
            select -= 1;

        if (select == 0)
        {
            DrawText("Start", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4, 20, YELLOW);
            if (IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_ENTER))
            {
                break;
            }
        }
        else
            DrawText("Start", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4, 20, RAYWHITE);


        if (select == 1)
        {
            DrawText("High Scores", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 + 50, 20, YELLOW);
            if (IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_ENTER))
            {
                /* code */
            }
        }
        else
            DrawText("High Scores", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 + 50, 20, RAYWHITE);

        if (select == 2)
        {
            DrawText("Quit", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 + 100, 20, YELLOW);
            if (IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_ENTER))
            {
                break;
            } 
        }
        else
            DrawText("Quit", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 + 100, 20, RAYWHITE);
        
        EndDrawing();
    }
    return select;
}