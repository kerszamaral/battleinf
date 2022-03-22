#include "raylib.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 450


int endscreen(void)
{
    int select = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText("GAME OVER", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 - 50, 40, LIME);
        
        if ( (IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S)) && select < 1 )
            select += 1;
        if ( (IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)) && select > 0 )
            select -= 1;

        if ( select == 0 )
        {
            DrawText("Restart", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4, 20, YELLOW);
            if ( IsKeyReleased(KEY_ENTER) )
                break;
        }
        else
            DrawText("Restart", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4, 20, RAYWHITE);

        if ( select == 1 )
        {
            DrawText("Quit", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 + 100, 20, YELLOW);
            if ( IsKeyReleased(KEY_ENTER) )
                break;
        }
        else
            DrawText("Quit", SCREENWIDTH / 4 + 120, SCREENHEIGHT / 4 + 100, 20, RAYWHITE);
        
        EndDrawing();
    }
    return select;
}