#include "jogo.h"
#include "core.h"
#include "raymath.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "shooting.h"

const int TOPBORDER = SCREENHEIGHT/12; //Menu border NEED TO CHANGE IT TO CORE WHEN CHANGED FIX IN ENEMY.C
const int BORDER = SCREENHEIGHT/90; //Border around playspace NEED TO CHANGE IT TO CORE WHEN CHANGED FIX IN ENEMY.C

int jogo(void)
{
    int k = 1; //variable for testing
    //Rectangle nullRec = { 0 , 0 , 0 , 0 };
    /********************** MENU VARIABELS *******************************/
    int level = 1; //Number of level on display
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
        0, //Score
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
    Obj enemy = 
    {
        (Vector2){ SCREENWIDTH , SCREENHEIGHT }, //Vector2 pos, x and y     Bullets spawn 0,0 conflict enemy spawn
        (float) tankenemy.width / tankenemy.height, //ratio
        (Vector2){ tankenemy.width / 20.0 , ( tankenemy.height * enemy.ratio ) / 20.0 }, //Vector2 center, x and y
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

    /********************** BULLET VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D bulletimg = LoadTexture( "resources/images/bullet.png" ); //Load playbullet image
    //Objects
    Obj bullet[2]; //Declares the number of bullets used for the level
    bullet->ratio = (float) bulletimg.width / bulletimg.height; //Fixes weird bug found by player that caused bullets to fill entire screen
    for (int i = 0; i < 2; i++) //Fills in the variables for all bullets
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
    ///Random Map Generator for testing, needs to be replaced by read file
    char terrainspace [ MAPY ][ MAPX ];
    for (int i = 0; i < MAPY; i++)
    {
        for (int j = 0; j < MAPX; j++)
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
    for (int i = 0; i < MAPY; i++)
    {
        for (int j = 0; j < MAPX; j++)
            printf("%c",terrainspace[i][j]);
        printf("\n");
    }
    //Creates the Rectangles in the place it finds * in the array to display it in the game
    //Varibles to help find the coordinates the triangles should be placed (might be a better way to do it idk)
    int terrainx = 0, terrainy = 0;
    //We use an array to create 128 rectangles, they are all set to size and position 0 
    //When it finds the * in sets the position and size for the rectangle on that place
    Rectangle terrainarray[ MAPY ][ MAPX ] = { 0 };
    for ( int i = 0 ; i < MAPY ; i++ )
    {
        for ( int j = 0 ; j < MAPX ; j++ )
        {
            if ( terrainspace[ i ][ j ] == '*' )
                terrainarray[ i ][ j ] = (Rectangle){ terrainx , terrainy + TOPBORDER , 50 , 50 };
            terrainx += 50;
        }
        terrainx = 0;
        terrainy +=50;
    }
    //Random player starting position
    player = spawn( player , terrainspace , terrainarray );
    
    //Main game loop
    while( !WindowShouldClose() && player.health != 0 ) //End if you press esc or player.health gets to 0
    {
        BeginDrawing();

        ClearBackground( RAYWHITE );
        
        /********************** TERRAIN CREATION *******************************/
        for (int i = 0; i < MAPY; i++)
            for (int j = 0; j < MAPX; j++)
                if (terrainspace[i][j] == '*')
                {
                    for (int k = 0; k < 1; k++)
                    {
                        collision( bullet[k] , terrainarray[i][j] );
                        if (bullet[k].colSide.x && terrainarray[i][j].height > 0)
                        {
                            terrainarray[i][j].height -= 10;
                        }
                        if (bullet[k].colSide.y && terrainarray[i][j].width > 0)
                        {
                            terrainarray[i][j].x += 10;
                            terrainarray[i][j].width -= 10;
                        }
                        if (bullet[k].colSide.z && terrainarray[i][j].height > 0)
                        {
                            terrainarray[i][j].y += 10;
                            terrainarray[i][j].height -= 10;
                        }
                        if (bullet[k].colSide.w && terrainarray[i][j].width > 0)
                        {
                            terrainarray[i][j].width -= 10;
                        }
                        bullet[k].colSide.x = 0;
                        bullet[k].colSide.y = 0;
                        bullet[k].colSide.z = 0;
                        bullet[k].colSide.w = 0;
                    }
                    DrawTexturePro( wall , sourceWall , terrainarray[i][j] , (Vector2){ 0 , 0 } , 0 , WHITE );
                }

        /********************** MENU CREATION *******************************/
        //Draws solid color rectangles
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //Text
        DrawText( TextFormat( "Fase %d" , level ) , SCREENWIDTH/2 - 6*10 , 10 , 40 , YELLOW );
        DrawText( TextFormat( "Score: %i", player.score ), SCREENWIDTH / 2 + 185 , 13 , 32 , RED );
        //Draws player health for health remaining            spacing from image size x * scaling
        for ( int i = 0, healthx = 5 ; i < player.health ; i++ , healthx += 35 )//
            DrawTextureEx( healthimg , (Vector2){ healthx , 10 } , 0 , 0.025 , WHITE );
        //                                           This image is too big, scaling factor needs to be very small

        /********************** PLAYER DRAWING *******************************/
        //Draw position and draw rectangle update
        //Sets player.draw to be player.pos + offset
        player.draw = (Vector2){ player.pos.x + player.cen.x , player.pos.y + player.cen.y };
        //Rectangle resized and offset for player drawing
        player.drawRec = (Rectangle){ player.draw.x, player.draw.y, player.cen.x*2 , player.cen.y*2 }; 
        //Player collision rectangle update
        player.colRec = (Rectangle){ player.pos.x , player.pos.y , player.cen.x*2 , player.cen.y*2 };
        //Because player cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
        DrawTexturePro( tankplayer , player.sourceRec , player.drawRec , player.cen , player.rot , WHITE ); //Draws player tank

        /********************** PLAYER COLLISION/MOVEMENT *******************************/
        //Resets collision detection
        player.colSide = (Vector4){ 0 , 0 , 0 , 0 };
        //Tests collision with sides
        for (int i = 0; i < 4; i++)
            player = collision( player , Menu[i]);
        //Tests collision with enemy
        player = collision( player , enemy.colRec );
        //Tests collision with each rectangle of terrain
        for (int i = 0; i < MAPY; i++)
            for (int j = 0; j < MAPX; j++)
                if (terrainspace[i][j] == '*')
                    player = collision( player, terrainarray[i][j]);
        
        player = moveplayer(player);

        /********************** PLAYER BULLET SHOOTING *******************************/
        bullet[0] = playershoot( player, bullet[0] );
        bullet[0] = shooting( bullet[0] , bullet[1], Menu , terrainspace, terrainarray );
        if ( !bullet[0].ammo )
        {   //Draws bullet[0]
            DrawTexturePro( bulletimg , bullet[0].sourceRec , bullet[0].drawRec , bullet[0].cen , bullet[0].rot , WHITE );
        }

        /********************** ENEMY HITBOX *******************************/
        //Draw position and draw rectangle update
        //Sets enemy.draw to be enemy.pos + offset
        enemy.draw = (Vector2){ enemy.pos.x + enemy.cen.x , enemy.pos.y + enemy.cen.y }; 
        //Rectangle resized and offset for enemy drawing
        enemy.drawRec = (Rectangle){ enemy.draw.x, enemy.draw.y, enemy.cen.x*2 , enemy.cen.y*2 };
        //Enemy collision rectangle
        enemy.colRec = (Rectangle){ enemy.pos.x , enemy.pos.y , enemy.cen.x*2 , enemy.cen.y*2 };
        //Because enemy cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size

        /********************** ENEMY SPAWNING *******************************/
        //Spawn logic
        enemy = enemyspawn( enemy, terrainspace , terrainarray );
        //Drawing needs to be done here else it causes a major bug
        if (enemy.health != 0)
            DrawTexturePro( tankenemy , enemy.sourceRec , enemy.drawRec , enemy.cen , enemy.rot , WHITE ); //Draws Enemy tank
        //Will be removed when bullet.c is done
        if ( CheckCollisionRecs( bullet[0].colRec, enemy.colRec ) ) //if player bullet collides with enemy, kills enemy
        {   //Reverts the states change when enemy alive to neutral
            enemy.health--;
            bullet[0].health--;
            bullet[0].ammo = true;
            enemy.pos = (Vector2){ SCREENWIDTH , SCREENHEIGHT };
            bullet[0].pos = (Vector2){ 0 , SCREENHEIGHT };
            player.score += 800;
        }

        /********************** ENEMY COLLISION/MOVEMENT *******************************/
        //Resets collision detection
        enemy.colSide = (Vector4){ 0 , 0 , 0 , 0 };
        //Tests collision with sides
        for (int i = 0; i < 4; i++)
            enemy = collision( enemy , Menu[i] );
        //Tests collision with player
        enemy = collision( enemy , player.colRec );
        //Tests collision with each rectangle of terrain
        for (int i = 0; i < MAPY; i++)
            for (int j = 0; j < MAPX; j++)
                if (terrainspace[i][j] == '*')
                    enemy = collision( enemy , terrainarray[i][j]);
        //When map destruction is done need to lower enemy sight distance, very easy
        if ( enemy.health >= 1 )
            enemy = enemymove(enemy, player);

        /********************** ENEMY BULLET SHOOTING *******************************/
        if (GetRandomValue(0,15) == 0 && bullet[1].ammo == true && enemy.health >= 1) //Verify if enemy has ammo
            bullet[1] = shoot( enemy, bullet[1] );

        bullet[1] = shooting( bullet[1] , bullet[0] , Menu, terrainspace, terrainarray );
        
        if ( !bullet[1].ammo )
        {
            if (CheckCollisionRecs( player.colRec , bullet[1].colRec ))
            {
                bullet[1].ammo = true;
                bullet[1].health = 0;
                bullet[1].time = 0;
                bullet[1].pos = (Vector2){ 0 , SCREENHEIGHT };
                player.health--;
            }
            DrawTexturePro(bulletimg, bullet[1].sourceRec, bullet[1].drawRec, bullet[1].cen, bullet[1].rot, WHITE);
        }
        
        /********************** ENERGY DRAWING/COLLISION *******************************/
        //Energy Spawning
        if ( energy.time >= 60*3 && !energy.health && GetRandomValue( 0 , 63 ) == 0 )
        {
            energy = spawn( energy , terrainspace , terrainarray );
            energy.colRec = (Rectangle){ energy.pos.x , energy.pos.y , energy.cen.x*2 , energy.cen.y*2 };
            energy.health = 1;
        }
        else
            energy.time++;
        //Energy Drawing
        if ( energy.health >= 1 )
        {
            DrawTexturePro( energyimg , energy.sourceRec , energy.colRec , (Vector2){ 0 , 0 } , 0 , WHITE );
            energy = collision( energy , player.colRec );
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

        /********************** TESTING VARIABLES *******************************/
        if (IsKeyPressed(KEY_K))
            k*=-1;
        if (k==-1)
            player.health = 3;
        DrawText(TextFormat("%d",GetFPS()),SCREENWIDTH/2,SCREENHEIGHT/2,32,LIME);
        EndDrawing();
    }

    /********************** UNLOADING AREA *******************************/
    UnloadTexture(healthimg);
    UnloadTexture(tankplayer);
    UnloadTexture(bulletimg);
    UnloadTexture(tankenemy);
    UnloadTexture(energyimg);

    return player.score;
}