#include "core.h"
#include "shooting.h"

void moveplayer( Obj *player , Setti *settings )
{
    /********************** PLAYER MOVEMENT *******************************/
    //When pointers are implemente, should be moved to move.c file and reference the data through pointers
    //Movement logic 
    if ( settings->players == 1 && player->health > 0 )
    {
        if ( IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) )
        {   //Checks player is colliding up
            if( !player->colSide.x )
                player->pos.y -= player->speed;
            player->rot = 0; //Sets players rotation to up
            if(player->pos.y - player->speed-1 < GetScreenHeight()/12)
                player->pos.y = GetScreenHeight() - player->cen.y*2 - GetScreenHeight()/90;
        }
        else if ( IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) )
        {   //Checks player is colliding down
            if( !player->colSide.z )
                player->pos.y += player->speed;
            player->rot = 180; //Sets player rotation to down
            if(player->pos.y + player->cen.y*2 + player->speed+1 > GetScreenHeight() - GetScreenHeight()/90)
                player->pos.y = GetScreenHeight()/12;
        }
        else if ( IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) )
        {   //Checks player is colliding left
            if( !player->colSide.w )
                player->pos.x -= player->speed;
            player->rot = 270; //Sets player rotation to left
            if(player->pos.x - player->speed-1 < GetScreenHeight()/90)
                player->pos.x = GetScreenWidth() - player->cen.x*2 - GetScreenHeight()/90;
        }
        else if ( IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT) )
        {   //Checks player is colliding right
            if( !player->colSide.y )
                player->pos.x += player->speed;
            player->rot = 90; //Sets player rotation to right
            if(player->pos.x + player->cen.x*2 + player->speed+1 > GetScreenWidth() - GetScreenHeight()/90)
                player->pos.x = GetScreenHeight()/90;
        }
    }
    else if ( player->id == 0 && player->health > 0 )
    {
        if ( IsKeyDown(KEY_W) )
        {   //Checks player is colliding up
            if( !player->colSide.x )
                player->pos.y -= player->speed;
            player->rot = 0; //Sets players rotation to up
        }
        else if ( IsKeyDown(KEY_S) )
        {   //Checks player is colliding down
            if( !player->colSide.z )
                player->pos.y += player->speed;
            player->rot = 180; //Sets player rotation to down
        }
        else if ( IsKeyDown(KEY_A) )
        {   //Checks player is colliding left
            if( !player->colSide.w )
                player->pos.x -= player->speed;
            player->rot = 270; //Sets player rotation to left
        }
        else if ( IsKeyDown(KEY_D) )
        {   //Checks player is colliding right
            if( !player->colSide.y )
                player->pos.x += player->speed;
            player->rot = 90; //Sets player rotation to right
        }
    }
    else if ( player->id == 1 && player->health > 0 )
    {
        if ( IsKeyDown(KEY_UP) )
        {   //Checks player is colliding up
            if( !player->colSide.x )
                player->pos.y -= player->speed;
            player->rot = 0; //Sets players rotation to up
        }
        else if ( IsKeyDown(KEY_DOWN) )
        {   //Checks player is colliding down
            if( !player->colSide.z )
                player->pos.y += player->speed;
            player->rot = 180; //Sets player rotation to down
        }
        else if ( IsKeyDown(KEY_LEFT) )
        {   //Checks player is colliding left
            if( !player->colSide.w )
                player->pos.x -= player->speed;
            player->rot = 270; //Sets player rotation to left
        }
        else if ( IsKeyDown(KEY_RIGHT) )
        {   //Checks player is colliding right
            if( !player->colSide.y )
                player->pos.x += player->speed;
            player->rot = 90; //Sets player rotation to right
        }
    }

    if (player->health > 0)
    {
        if ( IsGamepadButtonDown( player->id - settings->extended , 1 ) || GetGamepadAxisMovement(player->id - settings->extended  , 1) < -0.5 )
        {   //Checks player is colliding up
            if( !player->colSide.x )
                player->pos.y -= player->speed;
            player->rot = 0; //Sets players rotation to up
        }
        else if ( IsGamepadButtonDown(player->id - settings->extended , 3) || GetGamepadAxisMovement( player->id - settings->extended  , 1 ) > 0.5 )
        {   //Checks player is colliding down
            if( !player->colSide.z )
                player->pos.y += player->speed;
            player->rot = 180; //Sets player rotation to down
        }
        else if ( IsGamepadButtonDown(player->id - settings->extended , 4) || GetGamepadAxisMovement(player->id - settings->extended  , 0) < -0.5 )
        {   //Checks player is colliding left
            if( !player->colSide.w )
                player->pos.x -= player->speed;
            player->rot = 270; //Sets player rotation to left
        }
        else if ( IsGamepadButtonDown(player->id - settings->extended , 2) || GetGamepadAxisMovement(player->id - settings->extended  , 0) > 0.5 )
        {   //Checks player is colliding right
            if( !player->colSide.y )
                player->pos.x += player->speed;
            player->rot = 90; //Sets player rotation to right
        }
    }
    
}

void playershoot( Obj *player, Obj *Bullet , Setti *settings, SFX *Sounds)
{
    if (settings->players == 1 && player->health > 0 )
    {
        if ( IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_J) )  
            if (Bullet->ammo) //Verify if player(it's store on bullet) has ammo
                shoot( player , Bullet, Sounds);
    }
    else if ( player->id == 0 && player->health > 0 )
    {
        if ( IsKeyPressed(KEY_J) )  
            if (Bullet->ammo) //Verify if player(it's store on bullet) has ammo
                shoot( player , Bullet, Sounds);
    }
    else if ( player->id == 1 && player->health > 0 )
    {
        if ( IsKeyPressed(KEY_SPACE) )  
            if (Bullet->ammo) //Verify if player(it's store on bullet) has ammo
                shoot( player , Bullet, Sounds);
    }

    if ( player->health > 0 )
    {
         if ( IsGamepadButtonPressed(player->id - settings->extended , 7) || IsGamepadButtonPressed(player->id - settings->extended , 12) )  
            if (Bullet->ammo) //Verify if player(it's store on bullet) has ammo
                shoot( player , Bullet, Sounds);
    }
}