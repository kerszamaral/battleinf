#include "core.h"

Obj moveplayer(Obj player)
{
    /********************** PLAYER MOVEMENT *******************************/
    //When pointers are implemente, should be moved to move.c file and reference the data through pointers
    //Movement logic 
    if ( (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) )
    {   //Checks player is colliding up
        if( !player.colSide.x )
            player.pos.y -= player.speed;
        player.rot = 0; //Sets players rotation to up
    }
    else if ( (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) )
    {   //Checks player is colliding down
        if( !player.colSide.z )
            player.pos.y += player.speed;
        player.rot = 180; //Sets player rotation to down
    }
    else if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) )
    {   //Checks player is colliding left
        if( !player.colSide.w )
            player.pos.x -= player.speed;
        player.rot = 270; //Sets player rotation to left
    }
    else if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) )
    {   //Checks player is colliding right
        if( !player.colSide.y )
            player.pos.x += player.speed;
        player.rot = 90; //Sets player rotation to right
    }

    if (IsKeyReleased(KEY_SPACE) && player.ammo == true) //Verify if player has ammo
        

    return player;
}
    