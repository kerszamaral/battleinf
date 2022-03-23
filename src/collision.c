#include "core.h"

//Checks collision points for object against a rectangle
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
    //Used to see if it collides or is just passing
    short up = 0 , right = 0 , down = 0 , left = 0;
    //Tests against all the points in the array, adds +1 if it's hitting
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
        if ((i >= 12 && i <= 15) || i == 0 )
             if(CheckCollisionPointRec(Points[i],colRec))
                left++;
    }
    //If a side is hit on more than one point it is colliding with something
    if (up >= 2)
        object.colSide.x = 1;
    if (right >= 2)
        object.colSide.y = 1;
    if (down >= 2)
        object.colSide.z = 1;
    if (left >= 2)
        object.colSide.w = 1;

    return object;
    
}