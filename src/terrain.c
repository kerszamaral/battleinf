#include "core.h"
#include "collision.h"
 
void terraincreate(char terrainspace[])
{
    //Creates the Rectangles in the place it finds * in the array to display it in the game
    //Varibles to help find the coordinates the triangles should be placed (might be a better way to do it idk)
    for (int i = 0; i < (GetScreenHeight()/(GetScreenHeight()/12)) * (GetScreenWidth()/(GetScreenHeight()/12)); i++)
    {
            switch (GetRandomValue(0,4))
            {
            case 0:
                terrainspace[i] = '*';
                break;
            default:
                terrainspace[i] = '-';
                break;
            }
    }
    //Prints map to console to know if everything lines up, can be removed when changed
    printf("\n");
    for (int i = 0; i < (GetScreenHeight()/(GetScreenHeight()/12)) * (GetScreenWidth()/(GetScreenHeight()/12)); i++)
    {
        
        printf("%c",terrainspace[i]);

        if ((i+1) % (GetScreenWidth()/(GetScreenHeight()/12)) == 0)
            printf("\n");

    }
    printf("\n");
}

void terrainplace(  Rectangle terrainarray[] , char terrainspace[] )
{
    int terrainx = (GetScreenHeight()/90), terrainy = 0;
    //We use an array to create 128 rectangles, they are all set to size and position 0 
    //When it finds the * in sets the position and size for the rectangle on that place
    
    for (int i = 0; i < (GetScreenHeight()/(GetScreenHeight()/12)) * (GetScreenWidth()/(GetScreenHeight()/12)); i++)
    {

        if ( terrainspace[ i ] == '*' )
            terrainarray[ i ] = (Rectangle){ terrainx , terrainy + GetScreenHeight()/12 , (GetScreenHeight()/12) , (GetScreenHeight()/12) };
        terrainx += (GetScreenHeight()/12);
        
        if ((i+1) % (GetScreenWidth()/(GetScreenHeight()/12)) == 0)
        {
            terrainx = (GetScreenHeight()/90);
            terrainy += (GetScreenHeight()/12);
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
            terrain.height -= GetScreenHeight()/12*0.2;
            break;
        case 90:
            terrain.x += GetScreenHeight()/12*0.2;
            terrain.width -= GetScreenHeight()/12*0.2;
            break;
        case 180:
            terrain.y += GetScreenHeight()/12*0.2;
            terrain.height -= GetScreenHeight()/12*0.2;
            break;
        case 270:
            terrain.width -= GetScreenHeight()/12*0.2;
            break;
        }
        if (terrain.height < GetScreenHeight()/12*0.2 || terrain.width < GetScreenHeight()/12*0.2)
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