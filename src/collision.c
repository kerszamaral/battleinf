#include "core.h"
#include <stdio.h>

//Position given should be draw ones Size should be center ones
Obj collision( Obj object , Rectangle colRec )
{
    Vector2 Points[16] = {
        (Vector2){ object.draw.x - object.cen.x - object.speed , object.draw.y - object.cen.y - object.speed },   //0  U L      S: UP, LEFT

        (Vector2){ object.draw.x , object.draw.y - object.cen.y - object.speed },                                 //1   U       S: UP
        (Vector2){ object.draw.x - object.cen.x + object.speed , object.draw.y - object.cen.y - object.speed },   //2   U L R   S: UP
        (Vector2){ object.draw.x + object.cen.x - object.speed , object.draw.y - object.cen.y - object.speed },   //3   U R L   S: UP

        (Vector2){ object.draw.x + object.cen.x + object.speed , object.draw.y - object.cen.y - object.speed },   //4   U R     S: UP, RIGHT

        (Vector2){ object.draw.x + object.cen.x + object.speed , object.draw.y },                                 //5   R       S: RIGHT
        (Vector2){ object.draw.x + object.cen.x + object.speed , object.draw.y - object.cen.y + object.speed },   //6   U R D   S: RIGHT
        (Vector2){ object.draw.x + object.cen.x + object.speed , object.draw.y + object.cen.y - object.speed },   //7   D R U   S: RIGHT

        (Vector2){ object.draw.x + object.cen.x + object.speed , object.draw.y + object.cen.y + object.speed },   //8   D R     S: RIGHT, DOWN

        (Vector2){ object.draw.x , object.draw.y + object.cen.y + object.speed },                                 //9   D       S: DOWN
        (Vector2){ object.draw.x + object.cen.x - object.speed , object.draw.y + object.cen.y + object.speed },   //10  D R L   S: DOWN
        (Vector2){ object.draw.x - object.cen.x + object.speed , object.draw.y + object.cen.y + object.speed },   //11  D L R   S: DOWN

        (Vector2){ object.draw.x - object.cen.x - object.speed , object.draw.y + object.cen.y + object.speed },   //12  D L     S: DOWN, LEFT

        (Vector2){ object.draw.x - object.cen.x - object.speed , object.draw.y },                                 //13  L       S: LEFT
        (Vector2){ object.draw.x - object.cen.x - object.speed , object.draw.y + object.cen.y - object.speed },   //14  D L U   S: LEFT
        (Vector2){ object.draw.x - object.cen.x - object.speed , object.draw.y - object.cen.y + object.speed }    //15  U L D   S: LEFT
    };
    short up = 0 , right = 0 , down = 0 , left = 0;
    Color cor[16] = {
        BLACK,
        PURPLE,
        ORANGE,
        PINK,
        LIME,
        RED,
        BLUE,
        GREEN,
        YELLOW,
        MAROON,
        VIOLET,
        BROWN,
        MAGENTA,
        DARKBLUE,
        GOLD,
        DARKGRAY
    };

    for (int i = 0; i < 16; i++)
    {
        if (i <= 4)
            if(CheckCollisionPointRec(Points[i],colRec))
                up++;
        if (i >= 4 && i <= 8)
             if(CheckCollisionPointRec(Points[i],colRec))
                right++;
        if (i >= 8 && i <= 12)
             if(CheckCollisionPointRec(Points[i],colRec))
                down++;
        if ((i >= 12 && i <= 15)/* || i == 0 */)
             if(CheckCollisionPointRec(Points[i],colRec))
                left++;
    }
    if (up >= 2)
    {
        object.colSide.x = 1;
        printf("up: %f\n",object.colSide.x);
    }
    if (right >= 2)
    {
        object.colSide.y = 1;
        printf("right: %f\n",object.colSide.y);
    }
    if (down >= 2)
    {
        object.colSide.z = 1;
        printf("down: %f\n",object.colSide.z);
    }
    if (left >= 2)
    {
        object.colSide.w = 1;
        printf("left: %f\n",object.colSide.w);
    }
    for (int i = 0; i < 16; i++)
    {
        DrawPixelV(Points[i],cor[i]);
    }
    DrawRectangleRec(colRec, BLACK);

    return object;
    
}