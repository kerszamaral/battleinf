#ifndef end_H_
#define end_H_

#include "raylib.h"

typedef struct  //struct for the interactable objects in the game
{
    Vector2 pos; //Vector2 for position, has x and y
    float ratio; //How much the height is more than the widht
    Vector2 cen; //Vector2 for center position, has x and y
    Vector2 draw; //Vector 2 for drawing position, has x and y
    int health; //Int for health, for player
    int rot; //Int for object rotation
    int score; //int for game score, for player 
    int time; //Int for time, for bullets and enemys
    float speed; //int for speed, defines the speed objects move
    bool ammo; //bool for defining if the object can shoot or is alive
} Obj;

#endif