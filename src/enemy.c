#include "enemy.h"

//Function for spawning any object in a random valid location
void spawn( Setti *settings , Obj *spawn , char terrainspace[], Rectangle terrainarray[] , Obj player[] , Obj enemy[])
{
    do //Starts loop to find suitable location
    {
        spawn->colSide = (Vector4){ 0 , 0 , 0 , 0 }; //Resets collision detection of the object for the next position test
        spawn->pos = (Vector2) //Tries to get a random position to spawn
        {
            GetRandomValue( 5*scaleX() , GetScreenWidth() - 5*scaleX() - spawn->cen.x*2 ), //The X position needs to be inside the map
            GetRandomValue( 50*scaleY() , GetScreenHeight() - 5*scaleY() - spawn->cen.y*2 ) //The Y position needs to be inside the map
        };
        spawn->draw = (Vector2){ spawn->pos.x + spawn->cen.x , spawn->pos.y + spawn->cen.y }; //Updates object draw position
        
        /**** Collision Testing ****/
        for (int i = 0; i < MAPSIZE; i++) //Checks terrain array
                if ( terrainspace[ i ] == '#' ) //Only checks for collision against place where terrain is
                    collision( spawn, terrainarray[i] , 2); //Tests if the object collides with terrain
        for (int p = 0; p < settings->players; p++) //Tests if the object won't spawn inside the players
            collision( spawn, player[p].colRec , 2);
        for (int i = 0; i < settings->level; i++) //Tests if the object won't spawn inside the enemies
            collision( spawn , enemy[i].colRec , 2 );
    } while ( spawn->colSide.x || spawn->colSide.y || spawn->colSide.z || spawn->colSide.w ); //Tests until the object is not colliding with anything
}

//Function for spawning the enemies in the level
void enemyspawn( Setti *settings , Obj *enemy , char terrainspace[], Rectangle terrainarray[] , Obj player[] , Obj otherenemy[] )
{   
    if ( !enemy->health && settings->enemiesremaining < settings->level ) //If the enemy is dead and there is still enemies that should be spawned
        enemy->death++; //Increases the death counter

    if ( enemy->death > 60*5 && !enemy->health && !GetRandomValue(0,60) && GetTime() > settings->realTime + 1.5) //If deathcounter is 5 seconds, the enemy is dead and the random number is 0
    {
        if (ColorToInt(enemy->color) == ColorToInt(RED)) //Checks the enemy color, if it's red
            enemy->health = 2; //Sets the enemy health to 2 
        else //If the enemy is not red
            enemy->health = 1; //Sets the enemy health to 1
        settings->enemiesremaining++; //Increases the enemies remaining counter
        settings->realTime = GetTime(); //Sets the real time to the current time
        spawn( settings , enemy , terrainspace , terrainarray, player , otherenemy ); //Spawns the enemy in a random location
    }
}

//Logic for the enemy movement
void enemymove( Setti *settings , Obj *enemy, Obj player[] )
{   
    bool up = 0 , down = 0 , left = 0 , right = 0; //Variable to see which direction the player
    for (int p = 0; p < settings->players; p++) //Checks for all the players
    {   //Casts a ray every direction from the enemy, if it hits a player, move towards them \ detection range size
        if ( CheckCollisionPointLine( player[p].pos , enemy->pos , (Vector2){enemy->pos.x , 0} , player[p].cen.x/2 ))
            up = true; //Boolean for the player side
        if ( CheckCollisionPointLine( player[p].pos , enemy->pos , (Vector2){enemy->pos.x , GetScreenHeight()} , player[p].cen.x/2 ))
            down = true; //Boolean for the player side
        if ( CheckCollisionPointLine( player[p].pos , enemy->pos , (Vector2){ 0, enemy->pos.y} , player[p].cen.y/2 ))
            left = true; //Boolean for the player side
        if ( CheckCollisionPointLine( player[p].pos , enemy->pos , (Vector2){GetScreenWidth() , enemy->pos.y} , player[p].cen.y/2 ))
            right = true; //Boolean for the player side
    }

    //* Chase logic
    if ( up ) //Player direction in relation to the enemy
        moveUp( enemy ); //Moves the enemy up
    else if ( down ) //Player direction in relation to the enemy
        moveDown( enemy ); //Moves the enemy down
    else if ( left ) //Player direction in relation to the enemy
        moveLeft( enemy ); //Moves the enemy left
    else if( right ) //Player direction in relation to the enemy
        moveRight( enemy ); //Moves the enemy right
    else //* Random movement Logic, if no player is found
    {  
        if( enemy->time % 30 == 0 ) //Only gets a new number every half a second
        {   
            enemy->score = GetRandomValue(0,4); //gets a random number between 0 and 4
            enemy->time = 0; //Resets the time counter
        }  
        if (  enemy->score == 0 ) //If the number is 0
            moveUp( enemy ); //Moves the enemy up
        else if ( enemy->score == 1 ) //If the number is 1
            moveDown( enemy ); //Moves the enemy down
        if ( enemy->score == 2 ) //If the number is 2
            moveLeft( enemy ); //Moves the enemy left
        if ( enemy->score == 3 ) //If the number is 3
            moveRight( enemy ); //Moves the enemy right
        //If the number is 4, does nothing, makes the enemies seem more "alive"
    }
    enemy->time++; //Increases the time counter
}