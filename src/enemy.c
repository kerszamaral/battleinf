#include "core.h"

#define SCREENWIDTH 800 //Screen size x
#define SCREENHEIGHT 450 //Screen size y

Obj enemyspawn(Obj enemy)
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
        enemy.pos.x = GetRandomValue( 5*2 + enemy.cen.x , SCREENWIDTH - 5*2 - enemy.cen.x ); //To spawn enemy in random position inbounds
        enemy.pos.y = GetRandomValue( 45 + 5*2 + enemy.cen.y , SCREENHEIGHT - 5*2 - enemy.cen.y ); //To spawn enemy in random position inbounds
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
    