#ifndef core_H_
#define core_H_

#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREENWIDTH 1010 //Screen size x 1000
#define SCREENHEIGHT 655 //Screen size y 600

//#define (50) 50 //Size for the terrain blocks

typedef struct Obj //struct for the interactable objects in the game
{
    int id; //id of the object
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
    bool dying; //For seeing if game should play death animation
    Vector2 deathpos; //For death animation position
    int deathtimer; //For death animation timing
} Obj;

typedef struct Setti //struct for the interchangable settings in the game
{
    int players;
    int level;
    int score;
    int select;
    bool won;
    bool quit;
    Color theme;
    Color lettercolor;
    int extended;
    bool pause;
    bool exitgame;
    int pauseselect;
    bool loadgame;
    int foundplayerposition;
    int enemiesremaining;
    bool filenamefound;
    char error[100];
    char loadgamename[100]; //For loading game name 
} Setti;

typedef struct Textus  //struct for the textures in the game
{
    Texture2D player;  //Texture for the player tank
    Texture2D enemy; //Texture for the enemy tank
    Texture2D bullet; //Texture for the bullet
    Texture2D explosion; //Texture for the explosion
    Texture2D wall; //Wall texture
    Texture2D smoke; //Smoke texture
    Texture2D health; //Health texture
    Texture2D energy; //Energy texture
    Texture2D explosionVehicles; //Texture for the explosion of vehicles
    //Texture2D terrain; //Terrain texture
} Textus;

typedef struct SFX //struct for the sounds in the game
{
    Sound gamestart;  //Sound for the start of the game
    Sound gameend; //Sound for the end of the game
    Sound explosion; //Sound for the explosion
    Sound shoot; //Sound for the shoot
    Sound playerpassive; //Sound for the player
    Sound bulletmiss; //Sound for the bullet miss
    Sound terrainhit; //Sound for the terrain hit
    //Sound hit; //Sound for the hit
    //Sound death; //Sound for the death
} SFX;

#endif