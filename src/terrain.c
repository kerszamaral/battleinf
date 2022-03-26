#include "core.h"
#include "collision.h"
 
void terraincreate(char terrainspace[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)])
{
    //Creates the Rectangles in the place it finds * in the array to display it in the game
    //Varibles to help find the coordinates the triangles should be placed (might be a better way to do it idk)
    for (int i = 0; i < GetScreenHeight()/(GetScreenHeight()/12); i++)
    {
        for (int j = 0; j < GetScreenWidth()/(GetScreenHeight()/12); j++)
        {
            switch (GetRandomValue(0,4))
            {
            case 0:
                terrainspace[i][j] = '*';
                break;
            default:
                terrainspace[i][j] = '-';
                break;
            }
        }
    }
    //Prints map to console to know if everything lines up, can be removed when changed
    for (int i = 0; i < GetScreenHeight()/(GetScreenHeight()/12); i++)
    {
        for (int j = 0; j < GetScreenWidth()/(GetScreenHeight()/12); j++)
            printf("%c",terrainspace[i][j]);
        printf("\n");
    }
}

void terrainplace(  Rectangle terrainarray[ GetScreenHeight()/(GetScreenHeight()/12) ][ GetScreenWidth()/(GetScreenHeight()/12) ] , char terrainspace[ GetScreenHeight()/(GetScreenHeight()/12) ][ GetScreenWidth()/(GetScreenHeight()/12) ] )
{
    int terrainx = 0, terrainy = 0;
    //We use an array to create 128 rectangles, they are all set to size and position 0 
    //When it finds the * in sets the position and size for the rectangle on that place
    
    for ( int i = 0 ; i < GetScreenHeight()/(GetScreenHeight()/12) ; i++ )
    {
        for ( int j = 0 ; j < GetScreenWidth()/(GetScreenHeight()/12) ; j++ )
        {
            if ( terrainspace[ i ][ j ] == '*' )
                terrainarray[ i ][ j ] = (Rectangle){ terrainx , terrainy + GetScreenHeight()/12 , (GetScreenHeight()/12) , (GetScreenHeight()/12) };
            terrainx += (GetScreenHeight()/12);
        }
        terrainx = 0;
        terrainy += (GetScreenHeight()/12);
    }
}

Rectangle terraindestruct( Obj bullet , Rectangle terrain )
{
    bullet.colSide.x = 0;
    bullet.colSide.y = 0;
    bullet.colSide.z = 0;
    bullet.colSide.w = 0;
    bullet = collision( bullet , terrain , 2);
    if ( bullet.colSide.x || bullet.colSide.y || bullet.colSide.z || bullet.colSide.w )
    {
        switch (bullet.rot)
        {
        case 0:
            terrain.height -= 10;
            break;
        case 90:
            terrain.x += 10;
            terrain.width -= 10;
            break;
        case 180:
            terrain.y += 10;
            terrain.height -= 10;
            break;
        case 270:
            terrain.width -= 10;
            break;
        }
    }    

    if (terrain.height < 10 || terrain.width < 10)
    {
        terrain.height = 0;
        terrain.width = 0;
        terrain.x = SCREENWIDTH;
        terrain.y = 0;
    }

    return terrain;
}