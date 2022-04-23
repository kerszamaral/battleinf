#ifndef core_H_
#define core_H_

/********** Shared Libraries ***********/
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********* Common Values *************/
#define SCREENWIDTH 1010 //Screen size x 1010
#define SCREENHEIGHT 655 //Screen size y 655
#define RATIOX (1.0 / SCREENWIDTH) //Ratio on x axis for object and text scaling
#define RATIOY (1.0 / SCREENHEIGHT) //Ratio on y axis for object and text scaling
#define MAPSIZE 15*41 //Map size
#define SHOOTINGDELAY 1.3 //Delay between shots

/******** Structs used in the game *********/
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
    Sound soundEffect; //For first soundeffect (fixes multichannel overflow bug)
    Sound soundEffect2; //For second soundeffect (fixes multichannel overflow bug)
    double realTime; //For real time
} Obj;

typedef struct Setti //struct for the interchangable settings in the game
{
    int players; //Used for multiplayer (know how many players are playing)
    int level; //Used for loading, saving, enemies number and display level
    int score; //Used for loading, saving, and display score
    int select; //Used for most select menus in the game
    bool won; //Boolean for if the player has won the game
    bool quit; //Boolean for if the player has pressed ESC or quit the game
    Color theme; //Color for the background of the game
    Color lettercolor; //Color for the letters in the game, matches the theme for easier reading
    int extended; //Used for extended play variable for more players in multiplayer
    bool pause; //Boolean for if the game is paused
    bool exitgame; //Boolean for if the player want to go to the main menu
    int pauseselect; //Used for pause menu selection
    bool loadgame; //Boolean to know if the game should load a savegame
    int foundplayerposition; //Used to know if the game has found the position of all players in the save, if not, it will choose at random
    int enemiesremaining; //Used to know how many enemies are left in the level
    bool filenamefound; //Used to know if the game has found the filename of the loadgame
    int lowscore; //Used to know if the game has found the lowest score in the highscores
    unsigned long errortime; //used for the time to wipe the error from the screen
    double realTime; //For keeping the time in seconds for GetTime()
    char error[100]; //Used to display error messages
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
} Textus;

typedef struct SFX //struct for the sounds in the game
{
    Sound gamestart;  //Sound for the start of the game
    Sound gameend; //Sound for the end of the game
    Sound shoot; //Sound for the shoot
    Sound bulletmiss; //Sound for the bullet miss
    Sound terrainhit; //Sound for the terrain hit
} SFX;

/********* Common Functions *************/
void moveUp( Obj *object );

void moveDown( Obj *object );

void moveLeft( Obj *object );

void moveRight( Obj *object );

double scaleX( void ); //for text and object scaling

double scaleY( void ); //for text and object scaling

#endif