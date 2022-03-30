#include "core.h"

//Checks collision points for object against a rectangle
void collision( Obj *object , Rectangle colRec , int sens)
{
    Vector2 Points[ 36 ] = {
        (Vector2){ object->draw.x - object->cen.x - object->speed , object->draw.y - object->cen.y - object->speed },   //0  U L      S: UP, LEFT

        (Vector2){ object->draw.x - object->speed , object->draw.y - object->cen.y - object->speed },                  //1   U C R   S: UP
        (Vector2){ object->draw.x + object->speed , object->draw.y - object->cen.y - object->speed },                  //2   U C L   S: UP
        (Vector2){ object->draw.x - object->cen.x / 2 - object->speed , object->draw.y - object->cen.y - object->speed },//3   U L M   S: UP
        (Vector2){ object->draw.x - object->cen.x / 2 + object->speed , object->draw.y - object->cen.y - object->speed },//4   U L M   S: UP
        (Vector2){ object->draw.x + object->cen.x / 2 - object->speed , object->draw.y - object->cen.y - object->speed },//5   U R M   S: UP
        (Vector2){ object->draw.x + object->cen.x / 2 + object->speed , object->draw.y - object->cen.y - object->speed },//6   U R M   S: UP
        (Vector2){ object->draw.x - object->cen.x + object->speed , object->draw.y - object->cen.y - object->speed },    //7   U L R   S: UP
        (Vector2){ object->draw.x + object->cen.x - object->speed , object->draw.y - object->cen.y - object->speed },    //8   U R L   S: UP

        (Vector2){ object->draw.x + object->cen.x + object->speed , object->draw.y - object->cen.y - object->speed },    //9   U R     S: UP, RIGHT

        (Vector2){ object->draw.x + object->cen.x + object->speed , object->draw.y - object->speed },                   //10   R C U   S: RIGHT
        (Vector2){ object->draw.x + object->cen.x + object->speed , object->draw.y + object->speed },                   //11   R C D   S: RIGHT
        (Vector2){ object->draw.x + object->cen.x + object->speed , object->draw.y - object->cen.y / 2 - object->speed },//12  R U M   S: RIGHT
        (Vector2){ object->draw.x + object->cen.x + object->speed , object->draw.y - object->cen.y / 2 + object->speed },//13  R U M   S: RIGHT
        (Vector2){ object->draw.x + object->cen.x + object->speed , object->draw.y + object->cen.y / 2 - object->speed },//14  R D M   S: RIGHT
        (Vector2){ object->draw.x + object->cen.x + object->speed , object->draw.y + object->cen.y / 2 + object->speed },//15  R D M   S: RIGHT
        (Vector2){ object->draw.x + object->cen.x + object->speed , object->draw.y - object->cen.y + object->speed },    //16  U R D   S: RIGHT
        (Vector2){ object->draw.x + object->cen.x + object->speed , object->draw.y + object->cen.y - object->speed },    //17  D R U   S: RIGHT

        (Vector2){ object->draw.x + object->cen.x + object->speed , object->draw.y + object->cen.y + object->speed },    //18  D R     S: RIGHT, DOWN

        (Vector2){ object->draw.x + object->speed , object->draw.y + object->cen.y + object->speed },                   //19  D C R   S: DOWN
        (Vector2){ object->draw.x - object->speed , object->draw.y + object->cen.y + object->speed },                   //20  D C L   S: DOWN
        (Vector2){ object->draw.x - object->cen.x / 2 + object->speed , object->draw.y + object->cen.y + object->speed },//21  D L M   S: DOWN
        (Vector2){ object->draw.x - object->cen.x / 2 - object->speed , object->draw.y + object->cen.y + object->speed },//22  D L M   S: DOWN
        (Vector2){ object->draw.x + object->cen.x / 2 + object->speed , object->draw.y + object->cen.y + object->speed },//23  D R M   S: DOWN
        (Vector2){ object->draw.x + object->cen.x / 2 - object->speed , object->draw.y + object->cen.y + object->speed },//24  D R M   S: DOWN
        (Vector2){ object->draw.x + object->cen.x - object->speed , object->draw.y + object->cen.y + object->speed },    //25  D R L   S: DOWN
        (Vector2){ object->draw.x - object->cen.x + object->speed , object->draw.y + object->cen.y + object->speed },    //26  D L R   S: DOWN

        (Vector2){ object->draw.x - object->cen.x - object->speed , object->draw.y + object->cen.y + object->speed },    //27  D L     S: DOWN, LEFT

        (Vector2){ object->draw.x - object->cen.x - object->speed , object->draw.y - object->speed },                   //28  L C U   S: LEFT
        (Vector2){ object->draw.x - object->cen.x - object->speed , object->draw.y + object->speed },                   //29  L C D   S: LEFT
        (Vector2){ object->draw.x - object->cen.x - object->speed , object->draw.y - object->cen.y / 2 + object->speed },//30  L U M   S: LEFT
        (Vector2){ object->draw.x - object->cen.x - object->speed , object->draw.y - object->cen.y / 2 - object->speed },//31  L U M   S: LEFT
        (Vector2){ object->draw.x - object->cen.x - object->speed , object->draw.y + object->cen.y / 2 + object->speed },//32  L D M   S: LEFT
        (Vector2){ object->draw.x - object->cen.x - object->speed , object->draw.y + object->cen.y / 2 - object->speed },//33  L D M   S: LEFT
        (Vector2){ object->draw.x - object->cen.x - object->speed , object->draw.y + object->cen.y - object->speed },    //34  D L U   S: LEFT
        (Vector2){ object->draw.x - object->cen.x - object->speed , object->draw.y - object->cen.y + object->speed },    //35  U L D   S: LEFT
        
    };
    //Used to see if it collides or is just passing
    short up = 0 , right = 0 , down = 0 , left = 0;
    //Tests against all the points in the array, adds +1 if it's hitting
    for (int i = 0; i < 36; i++)
    {
        if (i <= 9)
            if(CheckCollisionPointRec(Points[i],colRec))
                up++;
        if (i >= 9 && i <= 18)
             if(CheckCollisionPointRec(Points[i],colRec))
                right++;
        if (i >= 18 && i <= 27)
             if(CheckCollisionPointRec(Points[i],colRec))
                down++;
        if ((i >= 27 && i <= 35) || i == 0 )
             if(CheckCollisionPointRec(Points[i],colRec))
                left++;
    }
    //If a side is hit on more than one point it is colliding with something
    if (up >= sens)
        object->colSide.x = 1;
    if (right >= sens)
        object->colSide.y = 1;
    if (down >= sens)
        object->colSide.z = 1;
    if (left >= sens)
        object->colSide.w = 1;
    
}