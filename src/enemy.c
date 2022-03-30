#include "collision.h"
#include "core.h"

//Random starting position
void spawn( Obj *spawn , int level , char terrainspace[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)] , Rectangle playerCol , Obj enemy[level])
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

        for (int i = 0; i < GetScreenHeight()/(GetScreenHeight()/12); i++)
            for (int j = 0; j < GetScreenWidth()/(GetScreenHeight()/12); j++)
                if ( terrainspace[ i ][ j ] == '*' )
                    collision( spawn, terrainarray[i][j] , 2); //Tests if it collides with terrain
        collision( spawn, playerCol , 2);
        for (int i = 0; i < level; i++)
            collision( spawn , enemy[i].colRec , 2 );
    //Tests while it doesn't find a suitable match
    } while ( spawn->colSide.x || spawn->colSide.y || spawn->colSide.z || spawn->colSide.w );
    //If it does, returns to game with starting position
}

void enemyspawn( Obj *enemy , int level , char terrainspace[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)] , Rectangle playerCol , Obj otherenemy[level] )
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
        spawn( enemy , level, terrainspace , terrainarray, playerCol, otherenemy );
    }
}

void enemymove(Obj *enemy, Obj *player)
{   
    /********************** ENEMY MOVEMENT *******************************/
    //Chase logic
    if ( !enemy->colSide.x && CheckCollisionPointLine(player->pos,enemy->pos, (Vector2){enemy->pos.x , 0}, player->cen.x/2))
    {   //Casts a ray up from the enemy, if it hits the player, move towards it                     Detection Range Size
        enemy->pos.y -= enemy->speed;
        enemy->rot = 0; //Sets enemy rotation to up
    }
    else if ( !enemy->colSide.z && CheckCollisionPointLine(player->pos,enemy->pos, (Vector2){enemy->pos.x , SCREENHEIGHT}, player->cen.x/2))
    {   //Casts a ray down from the enemy, if it hits the player, move towards it                                   Detection Range Size
        enemy->pos.y += enemy->speed;
        enemy->rot = 180; //Sets enemy rotation to down
    }
    else if ( !enemy->colSide.w && CheckCollisionPointLine(player->pos,enemy->pos, (Vector2){ 0, enemy->pos.y}, player->cen.y/2) )
    {   //Casts a ray to the left from the enemy, if it hits the player, move towards it            Detection Range Size
        enemy->pos.x -= enemy->speed;
        enemy->rot = 270; //Sets enemy rotation to left
    }
    else if( !enemy->colSide.y && CheckCollisionPointLine(player->pos,enemy->pos, (Vector2){ SCREENWIDTH , enemy->pos.y }, player->cen.y/2) )
    {   //Casts a ray to the right from the enemy, if it hits the player, move towards it                           Detection Range Size
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
        if (  enemy->score == 0 && !enemy->colSide.x)
        {   //If the number is 0 and it's not going out of bounds, move in that direction
            enemy->pos.y -= enemy->speed;
            enemy->rot = 0; //Sets enemy rotation to up
        }
        else if ( enemy->score == 1 && !enemy->colSide.z)
        {   //If the number is 1 and it's not going out of bounds, move in that direction
            enemy->pos.y += enemy->speed;
            enemy->rot = 180; //Sets enemy rotation to down
        }
        if ( enemy->score == 2 && !enemy->colSide.w)
        {   //If the number is 2 and it's not going out of bounds, move in that direction
            enemy->pos.x -= enemy->speed;
            enemy->rot = 270; //Sets enemy rotation to left
        }
        if ( enemy->score == 3 && !enemy->colSide.y)
        {   //If the number is 3 and it's not going out of bounds, move in that direction
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