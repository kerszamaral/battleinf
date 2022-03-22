#ifndef end_H_
#define end_H_

#include "raylib.h"

typedef struct  //struct for the interactable objects in the game
{
    Vector2 pos; //Vector2 for position, has x and y
    float ratio; //How much the height is more than the widht
    Vector2 cen; //Vector2 for center position, has x and y
    Vector2 draw; //Vector 2 for drawing position, has x and y
    int health; //Int for health
    int rot; //Int for object rotation
    int score; //int for game score
    unsigned long time; //Unsinged long for object based time
    float speed; //int for speed, defines the speed objects move
    bool ammo; //bool for defining if the object can shoot
    Rectangle colRec; //Rectangle for object collision (Rectangle){0,0,0,0}
    Rectangle drawRec; //Rectangle for drawing and object rotation ()
    Vector4 colSide; //For collision detection algorithm
} Obj;

#endif