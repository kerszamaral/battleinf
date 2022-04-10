#include "core.h"

void moveUp( Obj *object )
{
    //UP
    if ( !object->colSide.x ) //If the object is not colliding
        object->pos.y -= object->speed; //Move in that direction
    object->rot = 0; //Sets object rotation to up
}

void moveDown( Obj *object )
{
    //DOWN
    if ( !object->colSide.z ) //If the object is not colliding
        object->pos.y += object->speed; //Move in that direction
    object->rot = 180; //Sets object rotation to down
}

void moveLeft( Obj *object )
{
    //LEFT 
    if ( !object->colSide.w ) //If the object is not colliding
        object->pos.x -= object->speed; //Move in that direction
    object->rot = 270; //Sets object rotation to left
}

void moveRight( Obj *object )
{
    //RIGHT
    if ( !object->colSide.y ) //If the object is not colliding
        object->pos.x += object->speed; //Move in that direction
    object->rot = 90; //Sets object rotation to right
}

double scaleX( void ) //for text and object scaling
{
    double scale;
    scale = GetScreenWidth()*RATIOX;
    return scale;
}

double scaleY( void ) //for text and object scaling
{
    double scale;
    scale = GetScreenHeight()*RATIOY;
    return scale;
}