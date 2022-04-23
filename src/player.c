#include "player.h"

//Player Movement
void moveplayer( Obj *player , Setti *settings )
{
    //Movement logic 
    if ( player->health > 0) //Only move if the player is alive
    {
        if ( settings->players == 1 ) // makes the controlls work for any key if there's only one player
        {
            if ( IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) )
                moveUp( player ); //Moves the player up
            else if ( IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) )
                moveDown( player ); //Moves the player down
            else if ( IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) )
                moveLeft( player ); //Moves the player left
            else if ( IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT) )
                moveRight( player ); //Moves the player right
        }
        else if ( settings->players != 1 ) //If multiplayer is enabled
        {
            switch ( player->id ) //Tests the player id
            {
            case 0: //Player 0 is mapped to WASD
                if ( IsKeyDown(KEY_W) )
                    moveUp( player ); //Moves the player up
                else if ( IsKeyDown(KEY_S) )
                    moveDown( player ); //Moves the player down
                else if ( IsKeyDown(KEY_A) )
                    moveLeft( player ); //Moves the player left
                else if ( IsKeyDown(KEY_D) )
                    moveRight( player ); //Moves the player right
                break;
            case 1: //PLAYER 1 is mapped to arrow keys
                if ( IsKeyDown(KEY_UP) )
                    moveUp( player ); //Moves the player up
                else if ( IsKeyDown(KEY_DOWN) )
                    moveDown( player ); //Moves the player down
                else if ( IsKeyDown(KEY_LEFT) )
                    moveLeft( player ); //Moves the player left
                else if ( IsKeyDown(KEY_RIGHT) )
                    moveRight( player ); //Moves the player right
                break;
            }
            /*** Controller Support ***/
            //Moves the player ID equal to the controller ID, offset by the extended play variable
            if ( IsGamepadButtonDown( player->id - settings->extended, 1 ) || GetGamepadAxisMovement( player->id - settings->extended, 1 ) < -0.5 )
                moveUp( player ); //Moves the player up
            else if ( IsGamepadButtonDown( player->id - settings->extended, 3 ) || GetGamepadAxisMovement( player->id - settings->extended, 1 ) > 0.5 )
                moveDown( player ); //Moves the player down
            else if ( IsGamepadButtonDown( player->id - settings->extended, 4 ) || GetGamepadAxisMovement( player->id - settings->extended, 0 ) < -0.5 )
                moveLeft( player ); //Moves the player left
            else if ( IsGamepadButtonDown( player->id - settings->extended, 2 ) || GetGamepadAxisMovement( player->id - settings->extended, 0 ) > 0.5 )
                moveRight( player ); //Moves the player right
        }
    }
}

//Player Shooting
void playershoot( Obj *player, Obj *Bullet , Setti *settings )
{
    if ( player->health > 0 )
    {
        if ( settings->players == 1 ) // makes the controlls work for any key if there's only one player
        {
            if ( IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_J) )  
                if ( Bullet->ammo && GetTime() > Bullet->realTime + SHOOTINGDELAY)  //Verify if player(it's stored on bullet) has ammo
                    shoot( player, Bullet );
        }
        else if ( settings->players != 1 ) //If multiplayer is enabled
        {
            switch ( player->id )
            {
            case 0: //Player 0 is mapped to J
                if ( IsKeyPressed(KEY_J) )  
                    if ( Bullet->ammo && GetTime() > Bullet->realTime + SHOOTINGDELAY)  //Verify if player(it's stored on bullet) has ammo
                        shoot( player , Bullet );
                break;
            case 1: //Player 1 is mapped to SPACE
                if ( IsKeyPressed(KEY_SPACE) )  
                    if ( Bullet->ammo && GetTime() > Bullet->realTime + SHOOTINGDELAY)  //Verify if player(it's stored on bullet) has ammo
                        shoot( player , Bullet );
                break;
            }
            /*** Controller Support ***/
            //Moves the player ID equal to the controller ID, offset by the extended play variable
            if ( IsGamepadButtonPressed(player->id - settings->extended , 7) || IsGamepadButtonPressed(player->id - settings->extended , 12) )  
                if ( Bullet->ammo && GetTime() > Bullet->realTime + SHOOTINGDELAY)  //Verify if player(it's store on bullet) has ammo
                    shoot( player , Bullet );
        }
    }
}