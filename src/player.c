#include "core.h"
#include "shooting.h"

Obj moveplayer( Obj player , int ID)
{
    /********************** PLAYER MOVEMENT *******************************/
    //When pointers are implemente, should be moved to move.c file and reference the data through pointers
    //Movement logic 
    if ( ( IsKeyDown(KEY_W) && ID == 0 )  || ( IsKeyDown(KEY_UP) && ID == 1 ) || ( ID == -1 && (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) ) )
    {   //Checks player is colliding up
        if( !player.colSide.x )
            player.pos.y -= player.speed;
        player.rot = 0; //Sets players rotation to up
    }
    else if ( ( IsKeyDown(KEY_S) && ID == 0 )  || (IsKeyDown(KEY_DOWN) && ID == 1 ) || ( ID == -1 && (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) ) )
    {   //Checks player is colliding down
        if( !player.colSide.z )
            player.pos.y += player.speed;
        player.rot = 180; //Sets player rotation to down
    }
    else if ( (IsKeyDown(KEY_A) && ID == 0 )  || (IsKeyDown(KEY_LEFT) && ID == 1 ) || ( ID == -1 && (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) ) )
    {   //Checks player is colliding left
        if( !player.colSide.w )
            player.pos.x -= player.speed;
        player.rot = 270; //Sets player rotation to left
    }
    else if ( (IsKeyDown(KEY_D) && ID == 0 )  || (IsKeyDown(KEY_RIGHT) && ID == 1 ) || ( ID == -1 && (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) ) )
    {   //Checks player is colliding right
        if( !player.colSide.y )
            player.pos.x += player.speed;
        player.rot = 90; //Sets player rotation to right
    }

    return player;
}

Obj playershoot( Obj player, Obj Bullet , int ID)
{
    if (( IsKeyPressed(KEY_J) && ID == 0 ) || ( IsKeyPressed(KEY_SPACE) && ID == 1 ) || ( ID == -1 && (IsKeyPressed(KEY_SPACE) ||IsKeyPressed(KEY_J)) ) ) //Verify if player(it's store on bullet) has ammo
        if (Bullet.ammo)
            Bullet = shoot(player , Bullet);
        
    return Bullet;
}