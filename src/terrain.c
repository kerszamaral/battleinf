#include "core.h"
#include "collision.h"
 
void terraincreate(char terrainspace[][GetScreenWidth()/(GetScreenHeight()/12)])
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
    printf("\n");
    for (int i = 0; i < GetScreenHeight()/(GetScreenHeight()/12); i++)
    {
        for (int j = 0; j < GetScreenWidth()/(GetScreenHeight()/12); j++)
            printf("%c",terrainspace[i][j]);
        printf("\n");
    }
    printf("\n");
}

void terrainplace(  Rectangle terrainarray[][ GetScreenWidth()/(GetScreenHeight()/12) ] , char terrainspace[][ GetScreenWidth()/(GetScreenHeight()/12) ] )
{
    int terrainx = (GetScreenHeight()/90), terrainy = 0;
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
        terrainx = (GetScreenHeight()/90);
        terrainy += (GetScreenHeight()/12);
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