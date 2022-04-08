#include "core.h"
#include "collision.h"
 
void terraincreate(char terrainspace[])
{
    //Creates the Rectangles in the place it finds * in the array to display it in the game
    //Varibles to help find the coordinates the triangles should be placed (might be a better way to do it idk)
    for (int i = 0; i < 15 * 41; i++)
    {
        if( !GetRandomValue(0,6) )
            terrainspace[i] = '#';
        else
            terrainspace[i] = '-';
        if ( (i+1) % 41  == 0)
            terrainspace[i] = '\n';
        if ( i == 614 )
            terrainspace[i] = '\0';
    }
    //Prints map to console to know if everything lines up, can be removed when changed
    printf("\n%s\n\n", terrainspace);
}

void terrainplace(  Rectangle terrainarray[] , char terrainspace[] )
{
    float terrainx = 5 * (GetScreenWidth()*(1.0/1010)), terrainy = 50 * (GetScreenHeight()*(1.0/655));
    //When it finds the * in sets the position and size for the rectangle on that place
    
    for (int i = 0; i < 15 * 41; i++)
    {
        if ( terrainspace[i] == '#' )
            terrainarray[i] = (Rectangle){ terrainx , terrainy , 25 * (GetScreenWidth()*(1.0/1010)) , 40 * (GetScreenHeight()*(1.0/655))};
        
        terrainx += 25 * (GetScreenWidth()*(1.0/1010));
        
        if (terrainspace[i] == '\n')
        {
            terrainx = 5 * (GetScreenWidth()*(1.0/1010));
            terrainy += 40 * (GetScreenHeight()*(1.0/655));
        }
    }
}

Rectangle terraindestruct( Obj bullet , Rectangle terrain, SFX *sounds )
{
    bullet.colSide.x = 0;
    bullet.colSide.y = 0;
    bullet.colSide.z = 0;
    bullet.colSide.w = 0;
    collision( &bullet , terrain , 2);
    if ( bullet.colSide.x || bullet.colSide.y || bullet.colSide.z || bullet.colSide.w )
    {
        switch (bullet.rot)
        {
        case 0:
            terrain.height -= 40*0.25* (GetScreenHeight()*(1.0/655));
            break;
        case 90:
            terrain.x += 25*0.25*(GetScreenWidth()*(1.0/1010));
            terrain.width -= 25*0.25*(GetScreenWidth()*(1.0/1010));
            break;
        case 180:
            terrain.y += 40*0.25* (GetScreenHeight()*(1.0/655));
            terrain.height -= 40*0.25* (GetScreenHeight()*(1.0/655));
            break;
        case 270:
            terrain.width -= 25*0.25*(GetScreenWidth()*(1.0/1010));
            break;
        }
        if (terrain.height < 40*0.25* (GetScreenHeight()*(1.0/655)) || terrain.width < 25*0.25*(GetScreenWidth()*(1.0/1010)))
        {
            PlaySoundMulti(sounds->terrainhit);
            terrain.height = 0;
            terrain.width = 0;
            terrain.x = GetScreenWidth();
            terrain.y = 0;
        }
    }    


    return terrain;
}