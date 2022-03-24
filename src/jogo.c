#include "jogo.h"
#include "core.h"
#include "raymath.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "shooting.h"
#include "terrain.h"



Vector4 jogo(Vector4 gamestate)
{
    /********************** MENU VARIABELS *******************************/
    int level = gamestate.z, score = gamestate.y;
    //Textures
    Texture2D healthimg = LoadTexture( "resources/images/health.png" ); //Load imagem da vida do player
    //Rectangles for collision and drawing (dest rectangles need to be in while loop)
    Rectangle Menu[4] = {
    (Rectangle){ 0 , 0 , SCREENWIDTH , TOPBORDER }, //Rectangle for the ingame menu
    (Rectangle){ 0 , SCREENHEIGHT - BORDER , SCREENWIDTH, BORDER}, //Rectangle for bottom border
    (Rectangle){ 0 , 0 , BORDER, SCREENHEIGHT }, //Rectangle for left border
    (Rectangle){ SCREENWIDTH - BORDER , 0 , SCREENWIDTH , SCREENHEIGHT}   //Rectangle for right border
    };

    /********************** PLAYER VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D tankplayer = LoadTexture( "resources/images/player.png" );//Load player image
    //Object
    Obj player = 
    {
        (Vector2){ 0 , 0 }, //Vector2 pos, x and y
        (float) tankplayer.width / tankplayer.height, //ratio
        (Vector2){ tankplayer.width / 20.0 , ( tankplayer.height * player.ratio ) / 20.0 }, //Vector2 center, x and y
        (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
        3, //Health
        0, //Object rotation
        gamestate.y, //Score
        0, //Time
        0, //Death
        2, //Speed
        false, //Ammo
        (Rectangle){ 0 , 0 , tankplayer.width , tankplayer.height }, //sourceRec
        (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
        (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
        (Vector4){ 0 , 0 , 0 , 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
    };

    /********************** ENEMY VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D tankenemy = LoadTexture( "resources/images/enemy.png" ); //Load enemy image
    //Object
    Obj enemy[ level ];
    enemy->ratio = (float) tankenemy.width / tankenemy.height; //Fixes weird bug found to fill entire screen
    for (int i = 0; i < level; i++)
    {
        enemy[i] = 
        (Obj){
            (Vector2){ SCREENWIDTH , SCREENHEIGHT }, //Vector2 pos, x and y     Bullets spawn 0,0 conflict enemy spawn
            (float) tankenemy.width / tankenemy.height, //ratio
            (Vector2){ tankenemy.width / 20.0 , ( tankenemy.height * enemy->ratio ) / 20.0 }, //Vector2 center, x and y
            (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
            0, //Health
            0, //Object rotation
            0, //Score
            0, //Time
            0, //Death
            1, //Speed
            false, //Ammo
            (Rectangle){ 0 , 0 , tankenemy.width , tankenemy.height }, //sourceRec
            (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
            (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
            (Vector4){ 0 , 0 , 0 , 0 } //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
        };
    }

    /********************** BULLET VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D bulletimg = LoadTexture( "resources/images/bullet.png" ); //Load playbullet image
    //Objects
    Obj bullet[ 1 + level ]; //Declares the number of bullets used for the level
    bullet->ratio = (float) bulletimg.width / bulletimg.height; //Fixes weird bug found by player that caused bullets to fill entire screen
    for (int i = 0; i < 1 + level ; i++) //Fills in the variables for all bullets
    {
        bullet[i] =
        (Obj){
        (Vector2){ 0 , 0 }, //Vector2 pos, x and y
        (float) bulletimg.width / bulletimg.height, //ratio
        (Vector2){ bulletimg.width / 100.0 , ( bulletimg.height * bullet->ratio ) / 100.0 }, //Vector2 center, x and y
        (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
        0, //Health
        0, //Object rotation
        0, //Score
        0, //Time
        0, //Death
        3, //Speed
        true, //Ammo
        (Rectangle){ 0 , 0 , bulletimg.width , bulletimg.height }, //sourceRec
        (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
        (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
        (Vector4){ 0 , 0 , 0 , 0 } //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
        };
    };

    /********************** ENERGY VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D energyimg = LoadTexture( "resources/images/energy.png" );//Load energy image
    //Object
    Obj energy = 
    {
        (Vector2){ 0 , 0 }, //Vector2 pos, x and y
        (float) energyimg.width / energyimg.height, //ratio
        (Vector2){ energyimg.width / 20.0 , ( energyimg.height * energy.ratio ) / 20.0 }, //Vector2 center, x and y
        (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
        0, //Health
        0, //Object rotation
        0, //Score
        0, //Time
        0, //Death
        0, //Speed
        false, //Ammo
        (Rectangle){ 0 , 0 , energyimg.width , energyimg.height }, //sourceRec
        (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
        (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
        (Vector4){ 0 , 0 , 0 , 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
    };
    
    /********************** TERRAIN VARIABELS *******************************/
    //Playspace is 1000x600 / We devide those in 50 by 50 cubes
    //Textures
    Texture2D wall = LoadTexture( "resources/images/wall.png" );
    Rectangle sourceWall = { 0 , 0 , wall.width , wall.height }; //Rectangle with size of original image
    //Random Map Generator for testing, needs to be replaced by read file
    char terrainspace [ MAPY ][ MAPX ];
    terraincreate(terrainspace);
    //Creates the actual rectangles in the right place
    Rectangle terrainarray[ MAPY ][ MAPX ] = { 0 };
    terrainplace( terrainarray , terrainspace );
    //Random player starting position
    player = spawn( player , level , terrainspace , terrainarray , player.colRec , enemy);
    
    //Main game loop
    while( !WindowShouldClose() ) //End if you press esc or player.health gets to 0
    {
        BeginDrawing();

        ClearBackground( RAYWHITE );
        
        /********************** TERRAIN CREATION *******************************/
        for (int i = 0; i < MAPY; i++)
            for (int j = 0; j < MAPX; j++)
                if (terrainspace[i][j] == '*')
                {
                    for (int k = 0; k < level; k++) //Sometimes this creates ghost blocks, but it's not consistent and i don't know why
                        terrainarray[i][j] = terraindestruct(bullet[k],terrainarray[i][j]);
                    DrawTexturePro( wall , sourceWall , terrainarray[i][j] , (Vector2){ 0 , 0 } , 0 , WHITE );
                }

        /********************** MENU CREATION *******************************/
        //Draws solid color rectangles
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //Text 
        DrawText( TextFormat( "Fase %d" , level ) , SCREENWIDTH / 2 - MeasureText("Fase 10", GetFontDefault().baseSize) , 10 , 40 , YELLOW );
        DrawText( TextFormat( "Pontuação: %i", player.score ), SCREENWIDTH - MeasureText("Pontuação: 100000", GetFontDefault().baseSize) * 3.2 , 13 , 32 , RED );
        DrawText( TextFormat( "Inimigos restantes: %d/%d", level - (player.score - score) / 800 , level ),
        MeasureText("Inimigos restantes: 10/10", GetFontDefault().baseSize) + 10 , 15 , 24 , BLUE );
        //Draws player health for health remaining            spacing from image size x * scaling
        for ( int i = 0, healthx = 5 ; i < player.health ; i++ , healthx += 35 )//
            DrawTextureEx( healthimg , (Vector2){ healthx , 10 } , 0 , 0.025 , WHITE );
        //                                           This image is too big, scaling factor needs to be very small

        /********************** ENERGY DRAWING/COLLISION *******************************/
        //Energy Spawning
        if ( energy.time >= 60*3 && !energy.health && GetRandomValue( 0 , 63 ) == 0 )
        {
            energy = spawn( energy , level , terrainspace , terrainarray , player.colRec , enemy);
            energy.colRec = (Rectangle){ energy.pos.x , energy.pos.y , energy.cen.x*2 , energy.cen.y*2 };
            energy.health = 1;
        }
        else
            energy.time++;
        //Energy Drawing
        if ( energy.health >= 1 )
        {
            DrawTexturePro( energyimg , energy.sourceRec , energy.colRec , (Vector2){ 0 , 0 } , 0 , WHITE );
            energy = collision( energy , player.colRec , 2 );
            energy.time = 0;
            if ( energy.colSide.x || energy.colSide.y || energy.colSide.z || energy.colSide.w )
            {   //Energy pickup
                energy.health--;
                player.speed *= 1.5;
                bullet[0].speed *= 1.5;
                energy.ammo = true;
            }
        }
        //Energy falloff
        if ( energy.time >= 60*3 && energy.ammo )
        {
            player.speed /= 1.5;
            bullet[0].speed /= 1.5;
            energy.ammo = false;
        }

        /********************** PLAYER DRAWING *******************************/
        //Draw position and draw rectangle update
        //Sets player.draw to be player.pos + offset
        player.draw = (Vector2){ player.pos.x + player.cen.x , player.pos.y + player.cen.y };
        //Rectangle resized and offset for player drawing
        player.drawRec = (Rectangle){ player.draw.x, player.draw.y, player.cen.x*2 , player.cen.y*2 }; 
        //Player collision rectangle update
        player.colRec = (Rectangle){ player.pos.x , player.pos.y , player.cen.x*2 , player.cen.y*2 };
        //Because player cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
        if (player.health > 0)
            DrawTexturePro( tankplayer , player.sourceRec , player.drawRec , player.cen , player.rot , WHITE ); //Draws player tank

        /********************** PLAYER COLLISION/MOVEMENT *******************************/
        //Resets collision detection
        player.colSide = (Vector4){ 0 , 0 , 0 , 0 };
        //Tests collision with sides
        for (int i = 0; i < 4; i++)
            player = collision( player , Menu[i], 2 );
        //Tests collision with enemy
        for (int i = 0; i < level; i++)
            player = collision( player , enemy[i].colRec , 2 );
        //Tests collision with each rectangle of terrain
        for (int i = 0; i < MAPY; i++)
            for (int j = 0; j < MAPX; j++)
                if (terrainspace[i][j] == '*')
                    player = collision( player, terrainarray[i][j] , 2 );
        
        player = moveplayer(player);

        /********************** PLAYER BULLET SHOOTING *******************************/
        bullet[0] = playershoot( player, bullet[0] );
        bullet[0] = shooting( bullet[0] , bullet[1], Menu , terrainspace, terrainarray );
        if ( !bullet[0].ammo )
        {   //Draws bullet[0]
            DrawTexturePro( bulletimg , bullet[0].sourceRec , bullet[0].drawRec , bullet[0].cen , bullet[0].rot , WHITE );
        }

        /********************** ENEMY HITBOX *******************************/
        for (int k = 0; k < level; k++)
        {
            //Draw position and draw rectangle update
            //Sets enemy.draw to be enemy.pos + offset
            enemy[k].draw = (Vector2){ enemy[k].pos.x + enemy[k].cen.x , enemy[k].pos.y + enemy[k].cen.y }; 
            //Rectangle resized and offset for enemy[k] drawing
            enemy[k].drawRec = (Rectangle){ enemy[k].draw.x, enemy[k].draw.y, enemy[k].cen.x*2 , enemy[k].cen.y*2 };
            //Enemy collision rectangle
            enemy[k].colRec = (Rectangle){ enemy[k].pos.x , enemy[k].pos.y , enemy[k].cen.x*2 , enemy[k].cen.y*2 };
            //Because enemy cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
        }

        /********************** ENEMY SPAWNING *******************************/
        for (int k = 0; k < level; k++)
        {
            //Spawn logic
            enemy[k] = enemyspawn( enemy[k], level , terrainspace , terrainarray , player.colRec , enemy );
            //Drawing needs to be done here else it causes a major bug
            if (enemy[k].health != 0)
                DrawTexturePro( tankenemy , enemy[k].sourceRec , enemy[k].drawRec , enemy[k].cen , enemy[k].rot , WHITE ); //Draws Enemy tank
            //Will be removed when bullet.c is done
            if ( CheckCollisionRecs( bullet[0].colRec, enemy[k].colRec ) ) //if player bullet collides with enemy[k], kills enemy[k]
            {   //Reverts the states change when enemy[k] alive to neutral
                enemy[k].health--;
                bullet[0].health--;
                bullet[0].ammo = true;
                enemy[k].pos = (Vector2){ SCREENWIDTH , SCREENHEIGHT };
                bullet[0].pos = (Vector2){ 0 , SCREENHEIGHT };
                player.score += 800;
            }
        }

        /********************** ENEMY COLLISION/MOVEMENT *******************************/
        for (int k = 0; k < level; k++)
        {
            //Resets collision detection
            enemy[k].colSide = (Vector4){ 0 , 0 , 0 , 0 };
            //Tests collision with sides
            for (int i = 0; i < 4; i++)
                enemy[k] = collision( enemy[k] , Menu[i] , 2);
            //Tests collision with player
            enemy[k] = collision( enemy[k] , player.colRec , 2);
            //Tests collision with each rectangle of terrain
            for (int i = 0; i < MAPY; i++)
                for (int j = 0; j < MAPX; j++)
                    if (terrainspace[i][j] == '*')
                        enemy[k] = collision( enemy[k] , terrainarray[i][j] , 2 );
            //Tests collision against other enemys
            for (int i = 0; i < level; i++)
                enemy[k] = collision( enemy[k] , enemy[i].colRec , 2 );
            //When map destruction is done need to lower enemy[i] sight distance, very easy
            if ( enemy[k].health >= 1 )
                enemy[k] = enemymove(enemy[k], player);
        }
        

        /********************** ENEMY BULLET SHOOTING *******************************/
        for (int k = 0; k < level; k++)
        {
            if (GetRandomValue(0,15) == 0 && bullet[1 + k].ammo == true && enemy[k].health >= 1) //Verify if enemy[k] has ammo
                bullet[1 + k] = shoot( enemy[k], bullet[1 + k] );

            bullet[1 + k] = shooting( bullet[1 + k] , bullet[0] , Menu, terrainspace, terrainarray );
            
            if ( !bullet[1 + k].ammo )
            {
                if (CheckCollisionRecs( player.colRec , bullet[1 + k].colRec ))
                {
                    bullet[1 + k].ammo = true;
                    bullet[1 + k].health = 0;
                    bullet[1 + k].time = 0;
                    bullet[1 + k].pos = (Vector2){ 0 , SCREENHEIGHT };
                    player.health--;
                }
                DrawTexturePro(bulletimg, bullet[1 + k].sourceRec, bullet[1 + k].drawRec, bullet[1 + k].cen, bullet[1 + k].rot, WHITE);
            }
        }
        

        /********************** WINNING VARIABLES *******************************/
        if ( player.score >= gamestate.y + 800 * level )
        {
            player.time++;
            player.health = 3;
            DrawText( "LEVEL COMPLETE", SCREENWIDTH / 2 - MeasureText("LEVEL COMPLETE", GetFontDefault().baseSize) * 2 , SCREENHEIGHT / 2  , 40 , GOLD );
            if ( player.time == 60 * 2 )
            {
                gamestate.w = 1;
                break;
            }
        }else
            gamestate.w = 0;
        if(player.health <= 0)
        {
            player.time++;
            DrawText( "VOCE MORREU", SCREENWIDTH / 2 - MeasureText("VOCE MORREU", GetFontDefault().baseSize) * 2 , SCREENHEIGHT / 2  , 40 , RED );
            if ( player.time == 60 * 2 )
                break;
        }

        
        EndDrawing();
    }

    /********************** UNLOADING AREA *******************************/
    gamestate.y = player.score;

    UnloadTexture(healthimg);
    UnloadTexture(tankplayer);
    UnloadTexture(bulletimg);
    UnloadTexture(tankenemy);
    UnloadTexture(energyimg);

    return gamestate;
}