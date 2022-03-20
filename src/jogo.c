#include "jogo.h"
#include "core.h"
#include "raymath.h"
#include <stdio.h>

#define SCREENWIDTH 800 //Screen size x
#define SCREENHEIGHT 450 //Screen size y
const int TOPBORDER = SCREENHEIGHT/10; //Menu border
const int BORDER = SCREENHEIGHT/90;
//#define TOPBORDER 50 //Menu border
//#define BORDER 5 //Game border

int jogo(void)
{
    /********************** PLAYER VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D tankplayer = LoadTexture( "resources/images/player.png" );//Load player image
    //Object
    //              pos     x y                     ratio                        cen             IMG X/scale*2                    IMG Y*ratio/scale*2       draw   x y  health rot  score time  speed  ammo
    Obj player = {(Vector2){0,0}, (float)tankplayer.width/tankplayer.height ,(Vector2){ tankplayer.width/20.0 , (tankplayer.height*player.ratio)/20.0 }, (Vector2){0,0},  3  ,  0  ,  0  ,  0  ,  5  , true };
    //Random starting position
    player.pos.x = GetRandomValue( BORDER*2 + player.cen.x , SCREENWIDTH - BORDER*2 - player.cen.x ); //To start player in random position inbounds
    player.pos.y = GetRandomValue( 40 + BORDER*2 + player.cen.y , SCREENHEIGHT - BORDER*2 - player.cen.y ); //To start player in random position inbounds

    /********************** BULLET VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D bulletplayer = LoadTexture( "resources/images/bullet.png" ); //Load bullet image
    //Object
    //              pos     x y                       ratio                        cen             IMG X/scale*2                    IMG Y*ratio/scale*2               draw   x y  health rot  score time  speed  ammo
    Obj bullet = {(Vector2){0,0}, (float)bulletplayer.width/bulletplayer.height ,(Vector2){ bulletplayer.width/100.0 , (bulletplayer.height*player.ratio)/100.0 }, (Vector2){0,0},  0  ,  0  ,  0  ,  0  ,  6  , false };

    /********************** ENEMY VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D tankenemy = LoadTexture( "resources/images/enemy.png" ); //Load enemy image
    //Object
    //              pos     x y                                     ratio                        cen             IMG X/scale*2                    IMG Y*ratio/scale*2       draw   x y  health rot  score time  speed  ammo
    Obj enemy = {(Vector2){SCREENWIDTH,SCREENHEIGHT}, (float)tankenemy.width/tankenemy.height ,(Vector2){ tankenemy.width/20.0 , (tankenemy.height*enemy.ratio)/20.0 }, (Vector2){0,0},  0  ,  0  ,  0  ,  0  ,  1  , false };
    //          Bullets spawn 0,0 conflict enemy spawn

    /********************** MENU VARIABELS *******************************/
    int level = 1; //Number of level on display
    //Textures
    Texture2D healthimg = LoadTexture( "resources/images/health.png" ); //Load imagem da vida do player
    //Rectangles for collision and drawing (dest rectangles need to be in while loop)
    Rectangle topMenurec = { 0 , 0 , SCREENWIDTH , TOPBORDER }; //Rectangle for the ingame menu
    Rectangle bottomMenurec = { 0 , SCREENHEIGHT - BORDER , SCREENWIDTH, BORDER}; //Rectangle for bottom border
    Rectangle leftMenurec = { 0 , 0 , BORDER, SCREENHEIGHT }; //Rectangle for left border
    Rectangle rightMenurec = { SCREENWIDTH - BORDER , 0 , SCREENWIDTH , SCREENHEIGHT};   //Rectangle for right border

    //Main game loop
    while( !WindowShouldClose() && player.health != 0 ) //End if you press esc or player.health gets to 0
    {
        BeginDrawing();

        ClearBackground( RAYWHITE );

        /********************** MENU CREATION *******************************/
        //Draws solid color rectangles
        DrawRectangleRec( topMenurec , DARKGRAY ); //Creates the big grey bar for text
        DrawRectangleRec( rightMenurec , DARKGRAY );
        DrawRectangleRec( bottomMenurec , DARKGRAY );
        DrawRectangleRec( leftMenurec , DARKGRAY );
        //Text
        DrawText( TextFormat( "Fase %d" , level ) , SCREENWIDTH/2 - 6*10 , 0 , 40 , YELLOW );
        DrawText( TextFormat( "Score: %i", player.score ), SCREENWIDTH / 2 + 215 , 5 , 32 , RED );
        //Draws player health for health remaining            spacing from image size x * scaling
        for ( int i = 0, healthx = 5 ; i < player.health ; i++ , healthx += 35 )//
            DrawTextureEx( healthimg , (Vector2){ healthx , 5 } , 0 , 0.025 , WHITE );
        //                                           This image is too big, scaling factor needs to be very small

        /********************** PLAYER DRAWING *******************************/
        //Source rectangle, draw position and draw rectangle
        Rectangle sourcePlayer = { 0 , 0 , tankplayer.width , tankplayer.height }; //Rectangle with size of original image
        player.draw = (Vector2){ player.pos.x + player.cen.x , player.pos.y + player.cen.y }; //Sets player.draw to be player.pos + offset
        Rectangle drawPlayer = { player.draw.x, player.draw.y, player.cen.x*2 , player.cen.y*2 }; //Rectangle resized and offset for player drawing
        //Player collision rectangle
        Rectangle colPlayer = { player.pos.x , player.pos.y , player.cen.x*2 , player.cen.y*2 };
        //Because player cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
        DrawTexturePro( tankplayer , sourcePlayer , drawPlayer , player.cen , player.rot , WHITE ); //Draws player tank
        

        /********************** PLAYER MOVEMENT *******************************/
        //Movement logic 
        if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) 
        {   //For Tank like controls        
            if ( (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && !CheckCollisionRecs( colPlayer , topMenurec) ) 
            {                                                   //Checks player position agains top border + correction with the margin of player center
                player.pos.y -= player.speed;
                player.rot = 0; //Sets players rotation to up
            }
            if ( (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && !CheckCollisionRecs( colPlayer , bottomMenurec) )
            {                                                   //Checks player position agains bottom border + correction with the margin of player center
                player.pos.y += player.speed;
                player.rot = 180; //Sets player rotation to down
            }
        }
        if (!IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)&& !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) //For Tank like controls
        {
            if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && !CheckCollisionRecs( colPlayer , leftMenurec) )
            {                                                   //Checks player position agains left border + corretion with the margin of player center
                player.pos.x -= player.speed;
                player.rot = 270; //Sets player rotation to left
            }
            if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && !CheckCollisionRecs( colPlayer , rightMenurec) )
            {                                                   //Checks player position agains left border + corretion with the margin of player center
                player.pos.x += player.speed;
                player.rot = 90; //Sets player rotation to right
            }
        }
        
        /********************** PLAYER BULLET SHOOTING *******************************/
        if (IsKeyReleased(KEY_SPACE) && player.ammo == true) //Verify if player has ammo
        {
            //Shooting setup
            player.ammo = false; //Sets ammo to false
            bullet.health = 1; //Sets bullet health to 1

            bullet.rot = player.rot; //Stores the player rotation when bullet fires for next steps
            bullet.pos.x = player.draw.x; //Stores the player x postion when bullet fires for next steps
            bullet.pos.y = player.draw.y; //Stores the player y postion when bullet fires for next steps

            //Offsets for each rotation to fire from center and end of barrel
            if (bullet.rot == 0)
            {
                bullet.pos.y -= player.cen.y;
                bullet.pos.x -= bullet.cen.x;
            }
            if (bullet.rot == 90)
            {
                bullet.pos.x += player.cen.x;
                bullet.pos.y -= bullet.cen.y;
            }
            if (bullet.rot == 180)
            {
                bullet.pos.y += player.cen.y;
                bullet.pos.x -= bullet.cen.x;
            }
            if (bullet.rot == 270)
            {
                bullet.pos.x -= player.cen.x;
                bullet.pos.y -= bullet.cen.y;
            }
        }

        //Source rectangle, draw position and draw rectangle
        Rectangle sourceBulletplayer = { 0 , 0 , bulletplayer.width , bulletplayer.height }; //Rectangle with size of original image
        bullet.draw = (Vector2){ bullet.pos.x + bullet.cen.x , bullet.pos.y + bullet.cen.y }; //Sets player.draw to be player.pos + offset
        Rectangle drawBullet = { bullet.draw.x, bullet.draw.y, bullet.cen.x*2 , bullet.cen.y*2 }; //Rectangle resized and offset for player drawing
        //Bullet collision rectangle
        Rectangle colBullet = {bullet.pos.x, bullet.pos.y, bullet.cen.x*2, bullet.cen.y*2 };
        //Because bullet cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size

        if (bullet.health >= 1) //test to see if should draw bullet
        {
            if (bullet.time == 60*1 || CheckCollisionRecs( colBullet , topMenurec) || CheckCollisionRecs( colBullet , bottomMenurec) || CheckCollisionRecs( colBullet , leftMenurec) || CheckCollisionRecs( colBullet , rightMenurec) ) //Kills bullet if 1 sec passes or it collides with border
            {   //Reverts the states change when firing bullet to neutral
                player.ammo = true;
                bullet.health = 0;
                bullet.time = 0;
            }

            //Moves bullet based on position and speed
            if (bullet.rot == 0)
                bullet.pos.y -= bullet.speed;
            if (bullet.rot == 90)
                bullet.pos.x += bullet.speed;
            if (bullet.rot == 180)
                bullet.pos.y += bullet.speed;
            if (bullet.rot == 270)
                bullet.pos.x -= bullet.speed;

            //Draws bullet
            DrawTexturePro(bulletplayer, sourceBulletplayer, drawBullet, bullet.cen, bullet.rot, WHITE);
            //Stores the time the bullet is alive based on fps, 1 second = 60 frames
            bullet.time++;
        }

        /********************** ENEMY SPAWNING *******************************/
        //Source rectangle, draw position and draw rectangle
        Rectangle sourceEnemy = { 0 , 0 , tankenemy.width , tankenemy.height }; //Rectangle with size of original image
        enemy.draw = (Vector2){ enemy.pos.x + enemy.cen.x , enemy.pos.y + enemy.cen.y }; //Sets enemy.draw to be enemy.pos + offset
        Rectangle drawEnemy = { enemy.draw.x, enemy.draw.y, enemy.cen.x*2 , enemy.cen.y*2 };//Rectangle resized and offset for enemy drawing
        //Enemy collision rectangle
        Rectangle colEnemy = { enemy.pos.x , enemy.pos.y , enemy.cen.x*2 , enemy.cen.y*2 };
        //Because enemy cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
    
        if (enemy.health >= 1) //Test to see if enemy is alive
        {
            DrawTexturePro( tankenemy , sourceEnemy , drawEnemy , enemy.cen , enemy.rot , WHITE ); //Draws Enemy tank
            enemy.time = 0;
        }
        if (enemy.health == 0) //if not, starts couting
            enemy.time++;
        if (enemy.time > 60*5 && enemy.health == 0) //If enemy is dead and 5 seconds have passed spawns enemy at random position
        {
            enemy.health = 1;
            enemy.pos.x = GetRandomValue( BORDER*2 + enemy.cen.x , SCREENWIDTH - BORDER*2 - enemy.cen.x ); //To spawn enemy in random position inbounds
            enemy.pos.y = GetRandomValue( TOPBORDER + BORDER*2 + enemy.cen.y , SCREENHEIGHT - BORDER*2 - enemy.cen.y ); //To spawn enemy in random position inbounds
        }
        if (CheckCollisionRecs(colBullet,colEnemy)) //if Player bullet collides with enemy, kills enemy
        {   //Reverts the states change when enemy alive to neutral
            enemy.health--;
            bullet.health--;
            player.ammo = true;
            enemy.pos.x = SCREENWIDTH;
            enemy.pos.y = SCREENHEIGHT;
            player.score += 800;
        }
        /********************** ENEMY MOVEMENT *******************************/
        //Movement logic 
        //Chase logic
        if (enemy.score != 3 && !CheckCollisionRecs(colPlayer,colEnemy))
        {   //Test if its not doing another movement or colliding with the player
            if (CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){enemy.pos.x , 0}, player.cen.x*2))
            {   //Casts a ray up from the enemy, if it hits the player, move towards it
                enemy.pos.y -= enemy.speed;
                enemy.rot = 0; //Sets enemy rotation to up
                enemy.score = 1; //Sets enemy score to signal it's detecting a player in y
            }
            else if (CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){enemy.pos.x , SCREENHEIGHT}, player.cen.x*2))
            {   //Casts a ray down from the enemy, if it hits the player, move towards it
                enemy.pos.y += enemy.speed;
                enemy.rot = 180; //Sets enemy rotation to down
                enemy.score = 1; //Sets enemy score to signal it's detecting a player in y
            }
            else
            {
                enemy.score = 2; //Sets the enemy score to signal it's not detecting a player
            }
        }
        if(enemy.score != 1 && !CheckCollisionRecs(colPlayer,colEnemy))
        {   //Test if its not doing another movement or colliding with the player
            if (CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){ 0, enemy.pos.y}, player.cen.y*2))
            {   //Casts a ray to the left from the enemy, if it hits the player, move towards it
                enemy.pos.x -= enemy.speed;
                enemy.rot = 270; //Sets enemy rotation to left
                enemy.score = 3; //Sets enemy score to signal it's detecting a player in x
            }
            else if(CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){ SCREENWIDTH , enemy.pos.y }, player.cen.y*2))
            {   //Casts a ray to the right from the enemy, if it hits the player, move towards it
                enemy.pos.x += enemy.speed;
                enemy.rot = 90; //Sets enemy rotation to right
                enemy.score = 3; //Sets enemy score to signal it's detecting a player in x
            }
            else
            {
                enemy.score = 2; //Sets the enemy score to signal it's not detecting a player
            }
        } //Random movement
        if (enemy.score == 2 && !CheckCollisionRecs(colPlayer,colEnemy))
        {   //Test if its not detecting a player or colliding with the player
            int x; //Variable to store the random value from 0-4 !CheckCollisionRecs( colPlayer , topMenurec)
            if(player.time % 30 == 0 || CheckCollisionRecs( colEnemy , topMenurec) || CheckCollisionRecs( colEnemy , bottomMenurec) || CheckCollisionRecs( colEnemy , leftMenurec) || CheckCollisionRecs( colEnemy , rightMenurec) )  
            {   //Only gets a new number every half a second
                x = GetRandomValue(0,4);
                player.time = 0;
            } 
            if (x == 0 && !CheckCollisionRecs( colEnemy , topMenurec))
            {   //If the number is 0 and it's not going out of bounds, move in that direction
                enemy.pos.y -= enemy.speed;
                enemy.rot = 0; //Sets enemy rotation to up
            }
            if (x == 1 && !CheckCollisionRecs( colEnemy , bottomMenurec))
            {   //If the number is 1 and it's not going out of bounds, move in that direction
                enemy.pos.y += enemy.speed;
                enemy.rot = 180; //Sets enemy rotation to up
            }
            if (x == 2 && !CheckCollisionRecs( colEnemy , leftMenurec))
            {   //If the number is 2 and it's not going out of bounds, move in that direction
                enemy.pos.x -= enemy.speed;
                enemy.rot = 270; //Sets enemy rotation to up
            }
            if (x == 3 && !CheckCollisionRecs( colEnemy , rightMenurec))
            {   //If the number is 3 and it's not going out of bounds, move in that direction
                enemy.pos.x += enemy.speed;
                enemy.rot = 90; //Sets enemy rotation to up
            }  
            if (x == 4)//stay still
            {   //If the number is 4 and it's not going out of bounds, stay still
                enemy.pos.x = enemy.pos.x;
                enemy.pos.y = enemy.pos.y;
                enemy.rot = enemy.rot;
            }
        }
        player.time++;

        /********************** TESTING VARIABLES *******************************/
        if (IsKeyPressed(KEY_H)) //Test lives counter
            player.health -= 1;
        if (IsKeyPressed(KEY_K))
        {

        }
            
        EndDrawing();
    }

    /********************** UNLOADING AREA *******************************/
    UnloadTexture(healthimg);
    UnloadTexture(tankplayer);
    UnloadTexture(bulletplayer);
    UnloadTexture(tankenemy);
    return player.score;
}