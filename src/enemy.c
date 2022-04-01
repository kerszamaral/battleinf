#include "collision.h"
#include "core.h"

//Random starting position
void spawn( Setti *settings , Obj *spawn , char terrainspace[], Rectangle terrainarray[] , Obj player[] , Obj enemy[])
{
    do
    {
        spawn->colSide = (Vector4){ 0 , 0 , 0 , 0 }; //Resets collision detection
        spawn->pos = (Vector2) //Tries to get a random position to spawn
        {
            GetRandomValue( GetScreenHeight()/90 , GetScreenWidth() - GetScreenHeight()/90*2 - spawn->cen.x*2 ), 
            GetRandomValue( GetScreenHeight()/12 , GetScreenHeight() - GetScreenHeight()/90 - spawn->cen.y*2 ) 
        };
        //Updates draw position
        spawn->draw = (Vector2){ spawn->pos.x + spawn->cen.x , spawn->pos.y + spawn->cen.y }; 

        //Checks if the position is valid
        for (int i = 0; i < (GetScreenHeight()/(GetScreenHeight()/12)) * (GetScreenWidth()/(GetScreenHeight()/12)); i++)
                if ( terrainspace[ i ] == '*' )
                    collision( spawn, terrainarray[i] , 2); //Tests if it collides with terrain
        
        for (int p = 0; p < settings->players; p++)
            collision( spawn, player[p].colRec , 2);
        for (int i = 0; i < settings->level; i++)
            collision( spawn , enemy[i].colRec , 2 );
    //Tests while it doesn't find a suitable match
    } while ( spawn->colSide.x || spawn->colSide.y || spawn->colSide.z || spawn->colSide.w );
    //If it does, returns to game with starting position
}

void enemyspawn( Setti *settings , Obj *enemy , char terrainspace[], Rectangle terrainarray[] , Obj player[] , Obj otherenemy[] )
{   
    if (!enemy->health && enemy->death < 100000)
    {
        enemy->death++;
    }
    if ( enemy->death > 60*5 && enemy->health == 0 && enemy->death != 100000 && !GetRandomValue(0,60)) //If enemy is dead and 5 seconds have passed spawns enemy at random position
    {
        if (ColorToInt(enemy->color) == ColorToInt(RED)) //Switches don't work here unfortunately
            enemy->health = 2; //Controls enemy health based on colors
        else
            enemy->health = 1;
        enemy->death = 100000;
        spawn( settings , enemy , terrainspace , terrainarray, player , otherenemy );
    }
}

void enemymove( Setti *settings , Obj *enemy, Obj player[] )
{   
    /********************** ENEMY MOVEMENT *******************************/
    //Checks for all players
    bool up = 0 , down = 0 , left = 0 , right = 0;
    for (int p = 0; p < settings->players; p++)
    {   //Casts a ray every direction from the enemy, if it hits a player, move towards them \ detection range size
        if ( CheckCollisionPointLine( player[p].pos , enemy->pos , (Vector2){enemy->pos.x , 0} , player[p].cen.x/2 ))
            up = true;
        if ( CheckCollisionPointLine( player[p].pos , enemy->pos , (Vector2){enemy->pos.x , GetScreenHeight()} , player[p].cen.x/2 ))
            down = true;
        if ( CheckCollisionPointLine( player[p].pos , enemy->pos , (Vector2){ 0, enemy->pos.y} , player[p].cen.y/2 ))
            left = true;
        if ( CheckCollisionPointLine( player[p].pos , enemy->pos , (Vector2){GetScreenWidth() , enemy->pos.y} , player[p].cen.y/2 ))
            right = true;
    }
    //Chase logic
    if ( up )
    {   
        if ( !enemy->colSide.x )
            enemy->pos.y -= enemy->speed;
        enemy->rot = 0; //Sets enemy rotation to up
    }
    else if ( down )
    {   
        if ( !enemy->colSide.z )
            enemy->pos.y += enemy->speed;
        enemy->rot = 180; //Sets enemy rotation to down
    }
    else if ( left )
    {   
        if ( !enemy->colSide.w )
            enemy->pos.x -= enemy->speed;
        enemy->rot = 270; //Sets enemy rotation to left
    }
    else if( right )
    {   
        if ( !enemy->colSide.y )
            enemy->pos.x += enemy->speed;
        enemy->rot = 90; //Sets enemy rotation to right
    }
    else
    {   //Test if its not detecting a player or colliding with the player
        if( enemy->time % 30 == 0 )  
        {   //Only gets a new number every half a second
            enemy->score = GetRandomValue(0,4);
            enemy->time = 0;
        }  
        if (  enemy->score == 0 )
        {   //If the number is 0 and it's not going out of bounds, move in that direction
            if ( !enemy->colSide.x )
                enemy->pos.y -= enemy->speed;
            enemy->rot = 0; //Sets enemy rotation to up
        }
        else if ( enemy->score == 1 )
        {   //If the number is 1 and it's not going out of bounds, move in that direction
            if( !enemy->colSide.z )
                enemy->pos.y += enemy->speed;
            enemy->rot = 180; //Sets enemy rotation to down
        }
        if ( enemy->score == 2 )
        {   //If the number is 2 and it's not going out of bounds, move in that direction
            if( !enemy->colSide.w )
                enemy->pos.x -= enemy->speed;
            enemy->rot = 270; //Sets enemy rotation to left
        }
        if ( enemy->score == 3 )
        {   //If the number is 3 and it's not going out of bounds, move in that direction
            if( !enemy->colSide.y )
                enemy->pos.x += enemy->speed;
            enemy->rot = 90; //Sets enemy rotation to right
        }  
        if ( enemy->score == 4) //stay still
        {   //If the number is 4 and it's not going out of bounds, stay still
            enemy->pos.x = enemy->pos.x;
            enemy->pos.y = enemy->pos.y;
            enemy->rot = enemy->rot;
        }
    }
    enemy->time++;
}