#ifndef end_H_
#define end_H_

#include "raylib.h"
#include <stdio.h>

#define SCREENWIDTH 1000 //Screen size x
#define SCREENHEIGHT 600 //Screen size y

#define TOPBORDER SCREENHEIGHT/12 //Menu border NEED TO CHANGE IT TO CORE WHEN CHANGED FIX IN ENEMY.C
#define BORDER SCREENHEIGHT/90 //Border around playspace NEED TO CHANGE IT TO CORE WHEN CHANGED FIX IN ENEMY.C

#define TERRAINSIZE 50
#define MAPX SCREENWIDTH/TERRAINSIZE
#define MAPY SCREENHEIGHT/TERRAINSIZE

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
} Obj;

#endif