#include "collision.h"

#define SCREENWIDTH 800 //Screen size x
#define SCREENHEIGHT 450 //Screen size y

//Random starting position
Obj spawn( Obj spawn , char terrainspace[8][16], Rectangle terrainarray[8][16] )
{
    do
    {
        spawn.colSide = (Vector4){ 0 , 0 , 0 , 0 }; //Resets collision detection
        spawn.pos = (Vector2) //To start spawn in random position
        {
            GetRandomValue( 5 , SCREENWIDTH - 5*2 - spawn.cen.x*2 ), 
            GetRandomValue( 45 , SCREENHEIGHT - 5 - spawn.cen.y*2 ) 
        };

        spawn.draw = (Vector2){ spawn.pos.x + spawn.cen.x , spawn.pos.y + spawn.cen.y }; //Updates draw position

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 16; j++)
                if ( terrainspace[ i ][ j ] == '*' )
                    spawn = collision( spawn, terrainarray[i][j] ); //Tests if it collides with terrain

    } while ( spawn.colSide.x || spawn.colSide.y || spawn.colSide.z || spawn.colSide.w );
    
    return spawn;
}

Obj enemyspawn( Obj enemy , char terrainspace[8][16], Rectangle terrainarray[8][16] )
{   switch (enemy.health)//Test to see if enemy is alive
    {
    case 0: //if not, starts counting
        enemy.death++;
        break;
    default:
        enemy.death = 0;
        break;
    }
    if ( enemy.death > 60*5 && enemy.health == 0 ) //If enemy is dead and 5 seconds have passed spawns enemy at random position
    {
        enemy.health = 1;
        enemy = spawn( enemy , terrainspace , terrainarray );
    }
    
    return enemy;
}

Obj enemymove(Obj enemy, Obj player)
{   
    /********************** ENEMY MOVEMENT *******************************/
    //Chase logic
    if ( !enemy.colSide.x && CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){enemy.pos.x , 0}, player.cen.x/2))
    {   //Casts a ray up from the enemy, if it hits the player, move towards it                     Detection Range Size
        enemy.pos.y -= enemy.speed;
        enemy.rot = 0; //Sets enemy rotation to up
    }
    else if ( !enemy.colSide.z && CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){enemy.pos.x , SCREENHEIGHT}, player.cen.x/2))
    {   //Casts a ray down from the enemy, if it hits the player, move towards it                                   Detection Range Size
        enemy.pos.y += enemy.speed;
        enemy.rot = 180; //Sets enemy rotation to down
    }
    else if ( !enemy.colSide.w && CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){ 0, enemy.pos.y}, player.cen.y/2) )
    {   //Casts a ray to the left from the enemy, if it hits the player, move towards it            Detection Range Size
        enemy.pos.x -= enemy.speed;
        enemy.rot = 270; //Sets enemy rotation to left
    }
    else if( !enemy.colSide.y && CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){ SCREENWIDTH , enemy.pos.y }, player.cen.y/2) )
    {   //Casts a ray to the right from the enemy, if it hits the player, move towards it                           Detection Range Size
        enemy.pos.x += enemy.speed;
        enemy.rot = 90; //Sets enemy rotation to right
    }
    else
    {   //Test if its not detecting a player or colliding with the player
        if( enemy.time % 30 == 0 )  
        {   //Only gets a new number every half a second
            enemy.score = GetRandomValue(0,4);
            enemy.time = 0;
        }  
        if (  enemy.score == 0 && !enemy.colSide.x)
        {   //If the number is 0 and it's not going out of bounds, move in that direction
            enemy.pos.y -= enemy.speed;
            enemy.rot = 0; //Sets enemy rotation to up
        }
        else if ( enemy.score == 1 && !enemy.colSide.z)
        {   //If the number is 1 and it's not going out of bounds, move in that direction
            enemy.pos.y += enemy.speed;
            enemy.rot = 180; //Sets enemy rotation to down
        }
        if ( enemy.score == 2 && !enemy.colSide.w)
        {   //If the number is 2 and it's not going out of bounds, move in that direction
            enemy.pos.x -= enemy.speed;
            enemy.rot = 270; //Sets enemy rotation to left
        }
        if ( enemy.score == 3 && !enemy.colSide.y)
        {   //If the number is 3 and it's not going out of bounds, move in that direction
            enemy.pos.x += enemy.speed;
            enemy.rot = 90; //Sets enemy rotation to right
        }  
        if ( enemy.score == 4) //stay still
        {   //If the number is 4 and it's not going out of bounds, stay still
            enemy.pos.x = enemy.pos.x;
            enemy.pos.y = enemy.pos.y;
            enemy.rot = enemy.rot;
        }
    }

    enemy.time++;

    return enemy;
}