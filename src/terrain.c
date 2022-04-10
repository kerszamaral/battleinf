#include "terrain.h"

//Creates the random char array for the terrain
void terraincreate(char terrainspace[])
{
    for (int i = 0; i < MAPSIZE; i++)
    {
        if( !GetRandomValue(0,6) ) //If the random number between 0 and 6 is 0
            terrainspace[i] = '#'; //Sets the char to be a wall(#)
        else
            terrainspace[i] = '-'; //Sets the char to be a nothing(-)
        if ( (i+1) % 41  == 0) //If the terrain is at the end of the line
            terrainspace[i] = '\n'; //Sets the char to be a new line
        if ( i == 614 ) //If the terrain is at the end of the map
            terrainspace[i] = '\0'; //Sets the char to be a null terminator
    }
    printf("\n%s\n\n", terrainspace); //Prints map to console
}

//Places the rectangles for the terrain
void terrainplace(  Rectangle terrainarray[] , char terrainspace[] )
{
    float terrainx = 5*scaleX(), terrainy = 50*scaleY(); //Sets the starting x and y position of the terrain
    
    for (int i = 0; i < MAPSIZE; i++)
    {
        if ( terrainspace[i] == '#' ) //If the char is a wall add a rectangle
            terrainarray[i] = (Rectangle){ terrainx , terrainy , 25*scaleX() , 40*scaleY()};
        
        terrainx += 25*scaleX(); //Increments the x position of the terrain
        
        if (terrainspace[i] == '\n') //If the char is a new line
        {
            terrainx = 5*scaleX(); //Resets the x position of the terrain
            terrainy += 40*scaleY(); //Increments the y position of the terrain
        }
    }
}

//Function for resizing terrain on destruction
void terraindestruct( Obj bullet , Rectangle *terrain, SFX *sounds )
{
    bullet.colSide = (Vector4){ 0, 0, 0, 0 }; //Resets the collision side
    collision( &bullet , *terrain , 2); //Checks for collision with the terrain
    if ( bullet.colSide.x || bullet.colSide.y || bullet.colSide.z || bullet.colSide.w ) //If it collides
    {
        switch (bullet.rot) //Resizes the rectangle based on the bullet rotation
        {
        case 0: //If the bullet is facing up
            terrain->height -= 40*0.25* scaleY(); //Decreases the height of the terrain
            break;
        case 90: //If the bullet is facing right
            terrain->x += 25*0.25*scaleX(); //Increases the x position of the terrain
            terrain->width -= 25*0.25*scaleX(); //Decreases the width of the terrain
            break;
        case 180: //If the bullet is facing down
            terrain->y += 40*0.25* scaleY(); //Increases the y position of the terrain
            terrain->height -= 40*0.25* scaleY(); //Decreases the height of the terrain
            break;
        case 270: //If the bullet is facing left
            terrain->width -= 25*0.25*scaleX(); //Decreases the width of the terrain
            break;
        }
        if (terrain->height < 40*0.25* scaleY() || terrain->width < 25*0.25*scaleX()) //If the terrain is too small
        {
            PlaySoundMulti(sounds->terrainhit); //Plays the terrain destruction sound
            terrain->height = 0; //Sets the terrain height to 0
            terrain->width = 0; //Sets the terrain width to 0
            terrain->x = GetScreenWidth(); //Sets the terrain x position to the right of the screen
            terrain->y = 0; //Sets the terrain y position to the top of the screen
        }
    }
}