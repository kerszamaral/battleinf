#include "core.h"
#include "shooting.h"

Obj moveplayer( Obj player , int ID)
{
    /********************** PLAYER MOVEMENT *******************************/
    //When pointers are implemente, should be moved to move.c file and reference the data through pointers
    //Movement logic 
    if ( (IsKeyDown(KEY_W) && !ID)  || (IsKeyDown(KEY_UP) && ID) )
    {   //Checks player is colliding up
        if( !player.colSide.x )
            player.pos.y -= player.speed;
        player.rot = 0; //Sets players rotation to up
    }
    else if ( (IsKeyDown(KEY_S) && !ID)  || (IsKeyDown(KEY_DOWN) && ID) )
    {   //Checks player is colliding down
        if( !player.colSide.z )
            player.pos.y += player.speed;
        player.rot = 180; //Sets player rotation to down
    }
    else if ( (IsKeyDown(KEY_A) && !ID)  || (IsKeyDown(KEY_LEFT) && ID) )
    {   //Checks player is colliding left
        if( !player.colSide.w )
            player.pos.x -= player.speed;
        player.rot = 270; //Sets player rotation to left
    }
    else if ( (IsKeyDown(KEY_D) && !ID)  || (IsKeyDown(KEY_RIGHT) && ID) )
    {   //Checks player is colliding right
        if( !player.colSide.y )
            player.pos.x += player.speed;
        player.rot = 90; //Sets player rotation to right
    }

    return player;
}

Obj playershoot( Obj player, Obj Bullet , int ID)
{
    if ((IsKeyPressed(KEY_J)&& !ID) || (IsKeyPressed(KEY_SPACE) && ID)) //Verify if player(it's store on bullet) has ammo
        if (Bullet.ammo)
            Bullet = shoot(player , Bullet);
        
    return Bullet;
}