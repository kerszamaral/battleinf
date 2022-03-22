#include "jogo.h"
#include "core.h"
#include "raymath.h"
#include "collision.h"
#include <stdio.h>

#define SCREENWIDTH 800 //Screen size x
#define SCREENHEIGHT 450 //Screen size y
const int TOPBORDER = SCREENHEIGHT/10; //Menu border
const int BORDER = SCREENHEIGHT/90; //Border around playspace

int jogo(void)
{
    int k = 1; //variable for testing
    //When pointers are implemented all "VARIABELS" should be moved to another file and referenced through pointers
    /********************** PLAYER VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D tankplayer = LoadTexture( "resources/images/player.png" );//Load player image
    //Object
    //              pos     x y                     ratio                        cen             IMG X/scale*2                    IMG Y*ratio/scale*2       draw   x y  health rot  score time  speed  ammo
    Obj player = {(Vector2){0,0}, (float)tankplayer.width/tankplayer.height ,(Vector2){ tankplayer.width/20.0 , (tankplayer.height*player.ratio)/20.0 }, (Vector2){0,0},  3  ,  0  ,  0  ,  0  ,  2  , true , (Rectangle){0,0,0,0}, (Rectangle){0,0,0,0} ,(Vector4){0,0,0,0}};
    //Random starting position
    player.pos.x = GetRandomValue( BORDER*2 + player.cen.x , SCREENWIDTH - BORDER*2 - player.cen.x ); //To start player in random position inbounds
    player.pos.y = GetRandomValue( 40 + BORDER*2 + player.cen.y , SCREENHEIGHT - BORDER*2 - player.cen.y ); //To start player in random position inbounds
    //Source and Collision Rectangles creation, changed in the while loop
    Rectangle sourcePlayer = { 0 , 0 , tankplayer.width , tankplayer.height }; //Rectangle with size of original image
    Rectangle drawPlayer;
    Rectangle colPlayer;
    //Collision Vectors
    Vector2 UP;
    Vector2 DP;
    Vector2 LP;
    Vector2 RP;

    /********************** BULLET VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D bullet = LoadTexture( "resources/images/bullet.png" ); //Load playbullet image
    //Objects
    //                  pos     x y                       ratio            cen             IMG X/scale*2                    IMG Y*ratio/scale*2       draw   x y  health rot  score time  speed  ammo
    Obj playbullet = {(Vector2){0,0}, (float)bullet.width/bullet.height ,(Vector2){ bullet.width/100.0 , (bullet.height*playbullet.ratio)/100.0 }, (Vector2){0,0},  0  ,  0  ,  0  ,  0  ,  3  , false, (Rectangle){0,0,0,0}, (Rectangle){0,0,0,0} ,(Vector4){0,0,0,0} };
    //                   pos     x y                       ratio           cen             IMG X/scale*2                    IMG Y*ratio/scale*2         draw   x y  health rot  score time  speed  ammo
    Obj enemybullet = {(Vector2){0,0}, (float)bullet.width/bullet.height ,(Vector2){ bullet.width/100.0 , (bullet.height*enemybullet.ratio)/100.0 }, (Vector2){0,0},  0  ,  0  ,  0  ,  0  ,  2  , false, (Rectangle){0,0,0,0}, (Rectangle){0,0,0,0} ,(Vector4){0,0,0,0} };
    //Source and Collision Rectangles creation, changed in the while loop
    Rectangle sourceBullet = { 0 , 0 , bullet.width , bullet.height }; //Rectangle with size of original image
    Rectangle drawBullet;
    Rectangle colBullet;
    Rectangle drawBulletenemy;
    Rectangle colBulletenemy;
    bool colBulletTerrain; //To see bullet collision with terrain
    bool colBulletETerrain;

    /********************** ENEMY VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D tankenemy = LoadTexture( "resources/images/enemy.png" ); //Load enemy image
    //Object
    //              pos     x y                                     ratio                        cen             IMG X/scale*2                    IMG Y*ratio/scale*2       draw   x y  health rot  score time  speed  ammo
    Obj enemy = {(Vector2){SCREENWIDTH,SCREENHEIGHT}, (float)tankenemy.width/tankenemy.height ,(Vector2){ tankenemy.width/20.0 , (tankenemy.height*enemy.ratio)/20.0 }, (Vector2){0,0},  0  ,  0  ,  0  ,  0  ,  1  , true, (Rectangle){0,0,0,0}, (Rectangle){0,0,0,0} ,(Vector4){0,0,0,0} };
    //          Bullets spawn 0,0 conflict enemy spawn
    //Source and Collision Rectangles creation, changed in the while loop
    Rectangle sourceEnemy = { 0 , 0 , tankenemy.width , tankenemy.height }; //Rectangle with size of original image
    Rectangle drawEnemy;
    Rectangle colEnemy;
    //Collision Vectors;
    Vector2 UE;
    Vector2 DE;
    Vector2 LE;
    Vector2 RE;
    Vector2 distPE;

    /********************** MENU VARIABELS *******************************/
    unsigned long globaltimeenemy = 0;
    int level = 1; //Number of level on display
    //Textures
    Texture2D healthimg = LoadTexture( "resources/images/health.png" ); //Load imagem da vida do player
    //Rectangles for collision and drawing (dest rectangles need to be in while loop)
    Rectangle topMenurec = { 0 , 0 , SCREENWIDTH , TOPBORDER }; //Rectangle for the ingame menu
    Rectangle bottomMenurec = { 0 , SCREENHEIGHT - BORDER , SCREENWIDTH, BORDER}; //Rectangle for bottom border
    Rectangle leftMenurec = { 0 , 0 , BORDER, SCREENHEIGHT }; //Rectangle for left border
    Rectangle rightMenurec = { SCREENWIDTH - BORDER , 0 , SCREENWIDTH , SCREENHEIGHT};   //Rectangle for right border
    
    /********************** TERRAIN VARIABELS *******************************/
    //Playspace is 800x400 / starts at 0x45 /ends 800x445 / We devide those in 50 by 50 cubes
    //Textures
    Texture2D wall = LoadTexture( "resources/images/wall.png" );
    Rectangle sourceWall = { 0 , 0 , wall.width , wall.height }; //Rectangle with size of original image
    int mapx = 16, mapy = 8;
    ///Random Map Generator for testing, needs to be replaced by read file
    char terrainspace [ mapy ][ mapx ];
    for (int i = 0; i < mapy; i++)
    {
        for (int j = 0; j < mapx; j++)
        {
            switch (GetRandomValue(0,4))
            {
            case 0:
                terrainspace[i][j] = '*';
                break;
            default:
                terrainspace[i][j] = '-';
                break;
            }
        }
    }
    //Prints map to console to know if everything lines up, can be removed when changed
    for (int i = 0; i < mapy; i++)
    {
        for (int j = 0; j < mapx; j++)
            printf("%c",terrainspace[i][j]);
        printf("\n");
    }
    //Creates the Rectangles in the place it finds * in the array to display it in the game
    //Varibles to help find the coordinates the triangles should be placed (might be a better way to do it idk)
    int terrainx = 0, terrainy = 0;
    //We use an array to create 128 rectangles, they are all set to size and position 0 
    //When it finds the * in sets the position and size for the rectangle on that place
    Rectangle terrainarray[ 8 ][ 16 ] = { 0 };
    for ( int i = 0 ; i < 8 ; i++ )
    {
        for ( int j = 0 ; j < 16 ; j++ )
        {
            if ( terrainspace[ i ][ j ] == '*' )
                terrainarray[ i ][ j ] = (Rectangle){ terrainx , terrainy + 45 , 50 , 50 };
            terrainx += 50;
        }
        terrainx = 0;
        terrainy +=50;
    }
    
    //Main game loop
    while( !WindowShouldClose() && player.health != 0 ) //End if you press esc or player.health gets to 0
    {
        BeginDrawing();

        ClearBackground( RAYWHITE );
        
        /********************** TERRAIN creation *******************************/
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                if (terrainspace[i][j] == '*')
                {   //All collision checks with terrain need to be done here.
                    if (colBulletTerrain == false)
                        colBulletTerrain = CheckCollisionRecs(colBullet, terrainarray[i][j]);
                    if (colBulletETerrain == false)
                        colBulletETerrain = CheckCollisionRecs(colBulletenemy,terrainarray[i][j]);
                    DrawTexturePro(wall,sourceWall,terrainarray[i][j],(Vector2){0,0},0,WHITE);
                }
                terrainx += 50;
            }
            terrainx = 0;
            terrainy +=50;
        }

        /********************** MENU CREATION *******************************/
        //Draws solid color rectangles
        DrawRectangleRec( topMenurec , DARKGRAY ); //Creates the big grey bar for text
        DrawRectangleRec( rightMenurec , DARKGRAY );
        DrawRectangleRec( bottomMenurec , DARKGRAY );
        DrawRectangleRec( leftMenurec , DARKGRAY );
        //Text
        DrawText( TextFormat( "Fase %d" , level ) , SCREENWIDTH/2 - 6*10 , 0 , 40 , YELLOW );
        DrawText( TextFormat( "Score: %i", player.score ), SCREENWIDTH / 2 + 185 , 5 , 32 , RED );
        //Draws player health for health remaining            spacing from image size x * scaling
        for ( int i = 0, healthx = 5 ; i < player.health ; i++ , healthx += 35 )//
            DrawTextureEx( healthimg , (Vector2){ healthx , 5 } , 0 , 0.025 , WHITE );
        //                                           This image is too big, scaling factor needs to be very small

        /********************** PLAYER COLLISION/DRAWING *******************************/
        //Draw position and draw rectangle update
        player.draw = (Vector2){ player.pos.x + player.cen.x , player.pos.y + player.cen.y }; //Sets player.draw to be player.pos + offset
        drawPlayer = (Rectangle){ player.draw.x, player.draw.y, player.cen.x*2 , player.cen.y*2 }; //Rectangle resized and offset for player drawing
        //For detecting player boundary distance (will be removed with new collision system)
        UP = (Vector2){ player.draw.x , TOPBORDER };
        DP = (Vector2){ player.draw.x , SCREENHEIGHT - BORDER };
        LP = (Vector2){ BORDER , player.draw.y };
        RP = (Vector2){ SCREENWIDTH - BORDER , player.draw.y };
        //Player collision rectangle update
        colPlayer = (Rectangle){ player.pos.x , player.pos.y , player.cen.x*2 , player.cen.y*2 };
        //Because player cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
        DrawTexturePro( tankplayer , sourcePlayer , drawPlayer , player.cen , player.rot , WHITE ); //Draws player tank
    
        /********************** ENEMY COLLISION *******************************/
        //Draw position and draw rectangle update
        enemy.draw = (Vector2){ enemy.pos.x + enemy.cen.x , enemy.pos.y + enemy.cen.y }; //Sets enemy.draw to be enemy.pos + offset
        drawEnemy = (Rectangle){ enemy.draw.x, enemy.draw.y, enemy.cen.x*2 , enemy.cen.y*2 };//Rectangle resized and offset for enemy drawing
        //For detecting enemy boundary distance (will be removed with new collision system)
        UE = (Vector2){ enemy.draw.x , TOPBORDER };
        DE = (Vector2){ enemy.draw.x , SCREENHEIGHT - BORDER };
        LE = (Vector2){ BORDER , enemy.draw.y };
        RE = (Vector2){ SCREENWIDTH - BORDER , enemy.draw.y };
        //Enemy collision rectangle
        colEnemy = (Rectangle){ enemy.pos.x , enemy.pos.y , enemy.cen.x*2 , enemy.cen.y*2 };
        //Because enemy cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
        
        /********************** PLAYER/ENEMY COLLISION *******************************/
        //Will be remove when new collision System is introduced
        //To know which side the collision is happening
        int colSidePE = 0;
        //Subtracts the center position of the enemy from the center position of the player
        distPE = Vector2Subtract(player.draw,enemy.draw);
        
        //If distance of x is betwen a margin and distance y is 36.45 (in range because exact of causes flicker)
        //The player is colliding vertically with the enemy, side is determined by output signal
        if ( (distPE.x >= -35 && distPE.x <= 35) && (distPE.y <= -34 && distPE.y >= -38) )
            colSidePE = 1; //If it's negative, the player is above the enemy
        if ( (distPE.x >= -35 && distPE.x <= 35) && (distPE.y >= 34 && distPE.y <= 38) )
            colSidePE = 2; //if it's positive, the player is below the enemy

        //If distance of x is 36.45 (in range because exact causes flicker) and distance y is betwen a margin
        //The player is colliding vertically with the enemy, side is determined by output signal
        if ( (distPE.x <= -34 && distPE.x >= -38) && (distPE.y >= -35 && distPE.y <= 35))
            colSidePE = 3; //If it's negative, the player is to the left of the enemy
        if ( (distPE.x >= 34 && distPE.x <= 38) && (distPE.y >= -35 && distPE.y <= 35))
            colSidePE = 4; //if it's positive, the player is to the right of the enemy

        /********************** PLAYER MOVEMENT *******************************/
        //When pointers are implemente, should be moved to move.c file and reference the data through pointers
        //Movement logic 
        if ( (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) )
        {   //Checks player distance agains top border + correction with the margin of player center and contact with enemy tank
            if( (Vector2Distance( player.draw , UP ) >= player.speed + player.cen.y))
                player.pos.y -= player.speed;
            player.rot = 0; //Sets players rotation to up
        }
        else if ( (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) )
        {   //Checks player distance agains bottom border + correction with the margin of player center and contact with enemy tank
            if( (Vector2Distance( player.draw , DP )>=player.speed + player.cen.y ))
                player.pos.y += player.speed;
            player.rot = 180; //Sets player rotation to down
        }
        else if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) )
        {   //Checks player distance agains left border + correction with the margin of player center and contact with enemy tank
            if( (Vector2Distance( player.draw , LP ) >= player.speed + player.cen.y ))
                player.pos.x -= player.speed;
            player.rot = 270; //Sets player rotation to left
        }
        else if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) )
        {   //Checks player distance agains right border + correction with the margin of player center and contact with enemy tank
            if( (Vector2Distance( player.draw , RP ) >= player.speed + player.cen.y ))
                player.pos.x += player.speed;
            player.rot = 90; //Sets player rotation to right
        }
        
        /********************** PLAYER BULLET SHOOTING *******************************/
        //When pointers are implemented should be moved to "Shooting.c" function and data referenced through pointers
        if (IsKeyReleased(KEY_SPACE) && player.ammo == true) //Verify if player has ammo
        {
            //Shooting setup
            player.ammo = false; //Sets ammo to false
            playbullet.health = 1; //Sets playbullet health to 1

            playbullet.rot = player.rot; //Stores the player rotation when playbullet fires for next steps
            playbullet.pos.x = player.draw.x; //Stores the player x postion when playbullet fires for next steps
            playbullet.pos.y = player.draw.y; //Stores the player y postion when playbullet fires for next steps

            //Offsets for each rotation to fire from center and end of barrel
            switch (playbullet.rot)
            {
            case 0:
                playbullet.pos.y -= player.cen.y;
                playbullet.pos.x -= playbullet.cen.x;
                break;
            case 90:
                playbullet.pos.x += player.cen.x;
                playbullet.pos.y -= playbullet.cen.y;
                break;
            case 180:
                playbullet.pos.y += player.cen.y;
                playbullet.pos.x -= playbullet.cen.x;
                break;
            case 270:
                playbullet.pos.x -= player.cen.x;
                playbullet.pos.y -= playbullet.cen.y;
                break;
            }
        }

        //Draw position and draw rectangle updates
        playbullet.draw = (Vector2){ playbullet.pos.x + playbullet.cen.x , playbullet.pos.y + playbullet.cen.y }; //Sets player.draw to be player.pos + offset
        drawBullet = (Rectangle){ playbullet.draw.x, playbullet.draw.y, playbullet.cen.x*2 , playbullet.cen.y*2 }; //Rectangle resized and offset for player drawing
        //Bullet collision rectangle
        colBullet = (Rectangle){playbullet.pos.x, playbullet.pos.y, playbullet.cen.x*2, playbullet.cen.y*2 };
        //Because playbullet cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size

        if (playbullet.health >= 1) //test to see if should draw playbullet
        {
            //Can be remove with new collision system, but this might be less CPU intensive because it doesn't need the direction it collides but idk
            if (playbullet.time == 60*1 || CheckCollisionRecs( colBullet , topMenurec) || CheckCollisionRecs( colBullet , bottomMenurec) || CheckCollisionRecs( colBullet , leftMenurec) || CheckCollisionRecs( colBullet , rightMenurec)|| colBulletTerrain == true) //Kills playbullet if 1 sec passes or it collides with border
            {   //Reverts the states change when firing playbullet to neutral
                player.ammo = true;
                playbullet.pos = (Vector2){0,0};
                playbullet.health = 0;
                playbullet.time = 0;
                colBulletTerrain = false;
            }

            //Moves playbullet based on position and speed
            switch (playbullet.rot)
            {
            case 0:
                playbullet.pos.y -= playbullet.speed;
                break;
            case 90:
                playbullet.pos.x += playbullet.speed;
                break;
            case 180:
                playbullet.pos.y += playbullet.speed;
                break;
            case 270:
                playbullet.pos.x -= playbullet.speed;
                break;
            }
            //Draws playbullet
            DrawTexturePro(bullet, sourceBullet, drawBullet, playbullet.cen, playbullet.rot, WHITE);
            //Stores the time the playbullet is alive based on fps, 1 second = 60 frames
            playbullet.time++;
        }

        /********************** ENEMY SPAWNING *******************************/
        //When pointers are implemented should be moved to "EnemyLogic.c" function and data referenced through pointers
        //Everything "enemy" based should be moved, enemys will be an array for multiple ones to spawn, each one with it's logic
        //This function needs an in value to determine which enemy the logic should move
        switch (enemy.health)//Test to see if enemy is alive
        {
        case 0: //if not, starts counting
            globaltimeenemy++;
            break;
        default:
            DrawTexturePro( tankenemy , sourceEnemy , drawEnemy , enemy.cen , enemy.rot , WHITE ); //Draws Enemy tank
            globaltimeenemy = 0;
            break;
        }
        if ( globaltimeenemy > 60*5 && enemy.health == 0 ) //If enemy is dead and 5 seconds have passed spawns enemy at random position
        {
            enemy.health = 1;
            enemy.pos.x = GetRandomValue( BORDER*2 + enemy.cen.x , SCREENWIDTH - BORDER*2 - enemy.cen.x ); //To spawn enemy in random position inbounds
            enemy.pos.y = GetRandomValue( TOPBORDER + BORDER*2 + enemy.cen.y , SCREENHEIGHT - BORDER*2 - enemy.cen.y ); //To spawn enemy in random position inbounds
        }
        if ( CheckCollisionRecs(colBullet,colEnemy) ) //if Player playbullet collides with enemy, kills enemy
        {   //Reverts the states change when enemy alive to neutral
            enemy.health--;
            playbullet.health--;
            player.ammo = true;
            enemy.pos = (Vector2){SCREENWIDTH,SCREENHEIGHT};
            playbullet.pos = (Vector2){0,0};
            player.score += 800;
        }
        /********************** ENEMY MOVEMENT *******************************/
        //Movement logic 
        //Chase logic
        if ( enemy.score != 3 )
        {   //Test if its not doing another movement or colliding with the player
            if ( colSidePE != 1 && CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){enemy.pos.x , 0}, player.cen.x/2))
            {   //Casts a ray up from the enemy, if it hits the player, move towards it                     Detection Range Size
                enemy.pos.y -= enemy.speed;
                enemy.rot = 0; //Sets enemy rotation to up
                enemy.score = 1; //Sets enemy score to signal it's detecting a player in y
            }
            else if ( colSidePE != 2 && CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){enemy.pos.x , SCREENHEIGHT}, player.cen.x/2))
            {   //Casts a ray down from the enemy, if it hits the player, move towards it                                   Detection Range Size
                enemy.pos.y += enemy.speed;
                enemy.rot = 180; //Sets enemy rotation to down
                enemy.score = 1; //Sets enemy score to signal it's detecting a player in y
            }
            else
            {
                enemy.score = 2; //Sets the enemy score to signal it's not detecting a player
            }
        }
        if( enemy.score != 1 )
        {   //Test if its not doing another movement or colliding with the player
            if ( colSidePE != 3 && CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){ 0, enemy.pos.y}, player.cen.y/2) )
            {   //Casts a ray to the left from the enemy, if it hits the player, move towards it            Detection Range Size
                enemy.pos.x -= enemy.speed;
                enemy.rot = 270; //Sets enemy rotation to left
                enemy.score = 3; //Sets enemy score to signal it's detecting a player in x
            }
            else if( colSidePE != 4 && CheckCollisionPointLine(player.pos,enemy.pos, (Vector2){ SCREENWIDTH , enemy.pos.y }, player.cen.y/2) )
            {   //Casts a ray to the right from the enemy, if it hits the player, move towards it                           Detection Range Size
                enemy.pos.x += enemy.speed;
                enemy.rot = 90; //Sets enemy rotation to right
                enemy.score = 3; //Sets enemy score to signal it's detecting a player in x
            }
            else
            {
                enemy.score = 2; //Sets the enemy score to signal it's not detecting a player
            }
        } //Random movement
        if (enemy.score == 2 && colSidePE == 0)
        {   //Test if its not detecting a player or colliding with the player
            int x, b = 0; //Variable to store the random value from 0-4 and the side it's colliding
            if (Vector2Distance( enemy.draw , UE ) <= enemy.speed + enemy.cen.y)
                b = 1;
            if (Vector2Distance( enemy.draw , DE ) <= enemy.speed + enemy.cen.y)
                b = 2;
            if (Vector2Distance( enemy.draw , LE ) <= enemy.speed + enemy.cen.y)
                b = 3;
            if (Vector2Distance( enemy.draw , RE ) <= enemy.speed + enemy.cen.y)
                b = 4;
            
            if( player.time % 30 == 0)  
            {   //Only gets a new number every half a second
                x = GetRandomValue(0,4);
                player.time = 0;
            }  
            if ( x == 0 && b != 1)
            {   //If the number is 0 and it's not going out of bounds, move in that direction
                enemy.pos.y -= enemy.speed;
                enemy.rot = 0; //Sets enemy rotation to up
            }
            if (x == 1 && b != 2)
            {   //If the number is 1 and it's not going out of bounds, move in that direction
                enemy.pos.y += enemy.speed;
                enemy.rot = 180; //Sets enemy rotation to up
            }
            if (x == 2 && b != 3)
            {   //If the number is 2 and it's not going out of bounds, move in that direction
                enemy.pos.x -= enemy.speed;
                enemy.rot = 270; //Sets enemy rotation to up
            }
            if (x == 3 && b != 4)
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

        /********************** ENEMY BULLET SHOOTING *******************************/
        if (GetRandomValue(0,15) == 0 && enemy.ammo == true && enemy.health >= 1) //Verify if player has ammo
        {
            //Shooting setup
            enemy.ammo = false; //Sets ammo to false
            enemybullet.health = 1; //Sets playbullet health to 1

            enemybullet.rot = enemy.rot; //Stores the enemy rotation when enemybullet fires for next steps
            enemybullet.pos.x = enemy.draw.x; //Stores the enemy x postion when enemybullet fires for next steps
            enemybullet.pos.y = enemy.draw.y; //Stores the enemy y postion when enemybullet fires for next steps

            //Offsets for each rotation to fire from center and end of barrel
            switch (enemybullet.rot)
            {
            case 0:
                enemybullet.pos.y -= enemy.cen.y;
                enemybullet.pos.x -= enemybullet.cen.x;
                break;
            case 90:
                enemybullet.pos.x += enemy.cen.x;
                enemybullet.pos.y -= enemybullet.cen.y;
                break;
            case 180:
                enemybullet.pos.y += enemy.cen.y;
                enemybullet.pos.x -= enemybullet.cen.x;
                break;
            case 270:
                enemybullet.pos.x -= enemy.cen.x;
                enemybullet.pos.y -= enemybullet.cen.y;
                break;
            }
        }

        //Draw position and draw rectangle updates
        enemybullet.draw = (Vector2){ enemybullet.pos.x + enemybullet.cen.x , enemybullet.pos.y + enemybullet.cen.y }; //Sets player.draw to be player.pos + offset
        drawBulletenemy = (Rectangle){ enemybullet.draw.x, enemybullet.draw.y, enemybullet.cen.x*2 , enemybullet.cen.y*2 }; //Rectangle resized and offset for player drawing
        //Bullet collision rectangle
        colBulletenemy = (Rectangle){ enemybullet.pos.x, enemybullet.pos.y, enemybullet.cen.x*2, enemybullet.cen.y*2 };
        //Because enemybullet cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size

        if (enemybullet.health >= 1) //test to see if should draw enemybullet
        {
            if (enemybullet.time == 60*1 || CheckCollisionRecs( colBulletenemy , topMenurec) || CheckCollisionRecs( colBulletenemy , bottomMenurec) || CheckCollisionRecs( colBulletenemy , leftMenurec) || CheckCollisionRecs( colBulletenemy , rightMenurec) || colBulletETerrain == true ) //Kills enemybullet if 1 sec passes or it collides with border
            {   //Reverts the states change when firing enemybullet to neutral
                enemy.ammo = true;
                enemybullet.health = 0;
                enemybullet.pos = (Vector2){SCREENWIDTH,SCREENHEIGHT};
                enemybullet.time = 0;
                colBulletETerrain = false;
            }

            //Moves enemybullet based on position and speed
            switch (enemybullet.rot)
            {
            case 0:
                enemybullet.pos.y -= enemybullet.speed;
                break;
            case 90:
                enemybullet.pos.x += enemybullet.speed;
                break;
            case 180:
                enemybullet.pos.y += enemybullet.speed;
                break;
            case 270:
                enemybullet.pos.x -= enemybullet.speed;
                break;
            }
            if (CheckCollisionRecs( colBulletenemy , colBullet ))
            {
                player.ammo = true;
                playbullet.health = 0;
                playbullet.time = 0;
                playbullet.pos = (Vector2){0,0};
                enemy.ammo = true;
                enemybullet.health = 0;
                enemybullet.time = 0;
                enemybullet.pos = (Vector2){SCREENWIDTH,SCREENHEIGHT};
            }
            
            if (CheckCollisionRecs( colPlayer , colBulletenemy ))
            {
                enemy.ammo = true;
                enemybullet.health = 0;
                enemybullet.time = 0;
                enemybullet.pos = (Vector2){SCREENWIDTH,SCREENHEIGHT};
                player.health--;
            }
            //Draws enemybullet
            DrawTexturePro(bullet, sourceBullet, drawBulletenemy, enemybullet.cen, enemybullet.rot, WHITE);
            //Stores the time the enemybullet is alive based on fps, 1 second = 60 frames
            enemybullet.time++;
        }
        player.time++;
        
        /********************** TESTING VARIABLES *******************************/
        if (IsKeyPressed(KEY_K))
            k*=-1;
        if (k==-1)
            player.health = 3;
        player = collision(player,colEnemy);
        EndDrawing();
    }
    /********************** UNLOADING AREA *******************************/
    UnloadTexture(healthimg);
    UnloadTexture(tankplayer);
    UnloadTexture(bullet);
    UnloadTexture(tankenemy);
    return player.score;
}