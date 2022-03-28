#ifndef core_H_
#define core_H_

#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define SCREENWIDTH 1000 //Screen size x 1000
#define SCREENHEIGHT 600 //Screen size y 600

//#define (GetScreenHeight()/12) 50 //Size for the terrain blocks

typedef struct  //struct for the interactable objects in the game
{
    Vector2 pos; //Vector2 for position, has x and y
    float ratio; //How much the height is more than the width
    Vector2 cen; //Vector2 for center position, has x and y
    Vector2 draw; //Vector 2 for drawing position, has x and y
    int health; //Int for health
    int rot; //Int for object rotation
    int score; //int for game score
    unsigned long time; //Unsinged long for object based time
    unsigned long death; //Unsinged long for object based time (normally for how long it's been dead)
    float speed; //int for speed, defines the speed objects move
    bool ammo; //bool for defining if the object can shoot
    Rectangle sourceRec; //Rectangle for creating the source for textures
    Rectangle colRec; //Rectangle for object collision
    Rectangle drawRec; //Rectangle for drawing and object rotation
    Vector4 colSide; //For collision detection algorithm, x = up, y = right, z = down, w = left
    Color color; //For enemy difficulty and mutliplayer
} Obj;

typedef struct  //struct for the interchangable settings in the game
{
    int players;
    int level;
    int score;
    int select;
    bool won;
    bool quit;
} Setti;

#endif