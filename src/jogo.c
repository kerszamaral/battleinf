#include "jogo.h"
#include "core.h"
#include "raymath.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "shooting.h"
#include "terrain.h"



void jogo(Setti *settings)
{
    /********************** TEXTURES *******************************/
    Textus textures = {
        LoadTexture("resources/images/player.png"),  //Texture for the player tank
        LoadTexture("resources/images/enemy.png"), //Texture for the enemy tank
        LoadTexture("resources/images/bullet.png"), //Texture for the bullet
        LoadTexture("resources/images/explosionBullets.png"), //Texture for the explosion for bullets
        LoadTexture("resources/images/wall.png"), //Wall texture
        LoadTexture("resources/images/fire.png"), //Smoke texture
        LoadTexture("resources/images/health.png"), //Health texture
        LoadTexture("resources/images/energy.png") //Energy texture
        //LoadTexture("") //Terrain texture
    };
    /********************** MENU VARIABELS *******************************/
    ClearWindowState(FLAG_WINDOW_RESIZABLE);
    //Rectangles for collision and drawing (dest rectangles need to be in while loop)
    Rectangle Menu[4] = {
    (Rectangle){ 0 , 0 , GetScreenWidth() , GetScreenHeight()/12 }, //Rectangle for the ingame menu
    (Rectangle){ 0 , GetScreenHeight() - GetScreenHeight()/90 , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
    (Rectangle){ 0 , 0 , GetScreenHeight()/90, GetScreenHeight() }, //Rectangle for left border
    (Rectangle){ GetScreenWidth() - GetScreenHeight()/90 , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
    };

    /********************** PLAYER VARIABELS *******************************/
    //Object
    Obj player = 
    {
        0, //id of the object
        (Vector2){ 0 , 0 }, //Vector2 pos, x and y
        (float) textures.player.width / textures.player.height, //ratio
        (Vector2){ textures.player.width * (0.000083*GetScreenHeight()) , ( textures.player.height * player.ratio ) * (0.000083*GetScreenHeight()) }, //Vector2 center, x and y
        (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
        3, //Health
        0, //Object rotation
        0, //Score
        0, //Time
        0, //Death
        2*(GetScreenHeight()*(1.0/600)), //Speed
        false, //Ammo
        (Rectangle){ 0 , 0 , textures.player.width , textures.player.height }, //sourceRec
        (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
        (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
        (Vector4){ 0 , 0 , 0 , 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
        WHITE, //Color for enemy difficulty and mutliplayer
        false, //dying
        (Vector2){ 0 , 0 }, //deathpos
        0 //deathtimer
    };

    /********************** ENEMY VARIABELS *******************************/
    //Object
    Obj enemy[ settings->level ];
    enemy->ratio = (float) textures.enemy.width / textures.enemy.height; //Fixes weird bug found to fill entire screen
    for (int i = 0; i < settings->level; i++)
    {
        enemy[i] = 
        (Obj){
            i, //id of the object
            (Vector2){ GetScreenWidth() , GetScreenHeight() }, //Vector2 pos, x and y     Bullets spawn 0,0 conflict enemy spawn
            (float) textures.enemy.width / textures.enemy.height, //ratio
            (Vector2){ textures.enemy.width  * (0.000083*GetScreenHeight()) , ( textures.enemy.height * enemy->ratio )  * (0.000083*GetScreenHeight()) }, //Vector2 center, x and y
            (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
            0, //Health
            0, //Object rotation
            0, //Score
            0, //Time
            0, //Death
            1*(GetScreenHeight()*(1.0/600)), //Speed
            false, //Ammo
            (Rectangle){ 0 , 0 , textures.enemy.width , textures.enemy.height }, //sourceRec
            (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
            (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
            (Vector4){ 0 , 0 , 0 , 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
            WHITE, //Color for enemy difficulty and mutliplayer
            false, //dying
            (Vector2){ 0 , 0 }, //deathpos
            0 //deathtimer
        };
        if (!GetRandomValue( 0 , 15 )) //sets enemy difficulty and color
            enemy[i].color = RED;
        else
            enemy[i].color = WHITE;
    }

    /********************** BULLET VARIABELS *******************************/
    //Objects
    Obj bullet[ settings->players + settings->level ]; //Declares the number of bullets used for the level
    bullet->ratio = (float) textures.bullet.width / textures.bullet.height; //Fixes weird bug found by player that caused bullets to fill entire screen
    for (int i = 0; i < settings->players + settings->level ; i++) //Fills in the variables for all bullets
    {
        bullet[i] =
        (Obj){
            i, //id of the object
            (Vector2){ 0 , 0 }, //Vector2 pos, x and y
            (float) textures.bullet.width / textures.bullet.height, //ratio
            (Vector2){ textures.bullet.width * (0.000016*GetScreenHeight()) , ( textures.bullet.height * bullet->ratio ) * (0.000016*GetScreenHeight()) }, //Vector2 center, x and y
            (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
            0, //Health
            0, //Object rotation
            0, //Score
            0, //Time
            0, //Death
            3*(GetScreenHeight()*(1.0/600)), //Speed
            true, //Ammo
            (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height }, //sourceRec
            (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
            (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
            (Vector4){ 0 , 0 , 0 , 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
            WHITE, //Color for enemy difficulty and mutliplayer
            false, //dying
            (Vector2){ 0 , 0 }, //deathpos
            0 //deathtimer
        };
    };

    /********************** ENERGY VARIABELS *******************************/
    //Object
    Obj energy = 
    {
        0, //id of the object
        (Vector2){ 0 , 0 }, //Vector2 pos, x and y
        (float) textures.energy.width / textures.energy.height, //ratio
        (Vector2){ textures.energy.width  * (0.000083*GetScreenHeight()) , ( textures.energy.height * energy.ratio )  * (0.000083*GetScreenHeight()) }, //Vector2 center, x and y
        (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
        0, //Health
        0, //Object rotation
        0, //Score
        0, //Time
        0, //Death
        0, //Speed
        false, //Ammo
        (Rectangle){ 0 , 0 , textures.energy.width , textures.energy.height }, //sourceRec
        (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
        (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
        (Vector4){ 0 , 0 , 0 , 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
        WHITE, //Color for enemy difficulty and mutliplayer
        false, //dying
        (Vector2){ 0 , 0 }, //deathpos
        0 //deathtimer
    };
    
    /********************** TERRAIN VARIABELS *******************************/
    //Playspace is 1000x600 / We devide those in 50 by 50 cubes
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    //Random Map Generator for testing, needs to be replaced by read file
    char terrainspace [ GetScreenHeight()/(GetScreenHeight()/12) ][ (int)ceil(GetScreenWidth()/(GetScreenHeight()/12)) ];
    terraincreate(terrainspace);
    //Creates the actual rectangles in the right place
    Rectangle terrainarray[ GetScreenHeight()/(GetScreenHeight()/12) ][ (int)ceil(GetScreenWidth()/(GetScreenHeight()/12)) ];
    terrainplace( terrainarray , terrainspace );
    //Random player starting position
    spawn( &player , settings->level , terrainspace , terrainarray , player.colRec , enemy);
    
    //Main game loop
    while( !WindowShouldClose() ) //End if you press esc or player.health gets to 0
    {
        BeginDrawing();

        ClearBackground( BLACK );
        
        /********************** TERRAIN CREATION *******************************/
        for (int i = 0; i < GetScreenHeight()/(GetScreenHeight()/12); i++)
            for (int j = 0; j < (int)ceil(GetScreenWidth()/(GetScreenHeight()/12)); j++)
                if (terrainspace[i][j] == '*')
                {
                    for (int k = 0; k < settings->players + settings->level; k++) //Sometimes this creates ghost blocks, but it's not consistent and i don't know why
                        terrainarray[i][j] = terraindestruct(bullet[k],terrainarray[i][j]);
                    DrawTexturePro( textures.wall , sourceWall , terrainarray[i][j] , (Vector2){ 0 , 0 } , 0 , WHITE );
                }

        /********************** MENU CREATION *******************************/
        //Draws solid color rectangles
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //Text 
        DrawText( TextFormat( "Fase %d" , settings->level ) , GetScreenWidth() / 2 - MeasureText("Fase 10", GetFontDefault().baseSize) * (GetScreenHeight()*(1.0/600)) , 10*(GetScreenHeight()*(1.0/600)) , 40*(GetScreenHeight()*(1.0/600)) , YELLOW );
        DrawText( TextFormat( "Pontuação: %i", settings->score + player.score ), GetScreenWidth() - MeasureText("Pontuação: 100000", GetFontDefault().baseSize) * 3.2 * (GetScreenHeight()*(1.0/600)) , 13 * (GetScreenHeight()*(1.0/600)) , 32*(GetScreenHeight()*(1.0/600)) , RED );
        DrawText( TextFormat( "Inimigos restantes: %d/%d", settings->level - player.score / 800 , settings->level ),
        MeasureText("Inimigos restantes: 10/10", GetFontDefault().baseSize)*(GetScreenHeight()*(1.0/600)) + 10 , 15 * (GetScreenHeight()*(1.0/600)) , 24*(GetScreenHeight()*(1.0/600)) , BLUE );
        //Draws player health for health remaining            spacing from image size x * scaling
        for ( int i = 0, healthx = 5 ; i < player.health ; i++ , healthx += 35 * (GetScreenHeight()*(1.0/600)) )//
            DrawTextureEx( textures.health , (Vector2){ healthx , 10 } , 0 , 0.025 * (GetScreenHeight()*(1.0/600)), WHITE );
        //                                           This image is too big, scaling factor needs to be very small

        /********************** ENERGY DRAWING/COLLISION *******************************/
        //Energy Spawning
        if ( energy.time >= 60*3 && !energy.health && !GetRandomValue( 0 , 63 ) )
        {
            spawn( &energy , settings->level , terrainspace , terrainarray , player.colRec , enemy);
            energy.colRec = (Rectangle){ energy.pos.x , energy.pos.y , energy.cen.x*2 , energy.cen.y*2 };
            energy.health = 1;
        }
        else
            energy.time++;
        //Energy Drawing
        if ( energy.health >= 1 )
        {
            DrawTexturePro( textures.energy , energy.sourceRec , energy.colRec , (Vector2){ 0 , 0 } , 0 , WHITE );
            collision( &energy , player.colRec , 2 );
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
            DrawTexturePro( textures.player , player.sourceRec , player.drawRec , player.cen , player.rot , player.color ); //Draws player tank
        //ImageDraw()
        /********************** PLAYER COLLISION/MOVEMENT *******************************/
        //Resets collision detection
        player.colSide = (Vector4){ 0 , 0 , 0 , 0 };
        //Tests collision with sides
        for (int i = 0; i < 4; i++)
            collision( &player , Menu[i], 2 );
        //Tests collision with enemy
        for (int i = 0; i < settings->level; i++)
            collision( &player , enemy[i].colRec , 2 );
        //Tests collision with each rectangle of terrain
        for (int i = 0; i < GetScreenHeight()/(GetScreenHeight()/12); i++)
            for (int j = 0; j < (int)ceil(GetScreenWidth()/(GetScreenHeight()/12)); j++)
                if (terrainspace[i][j] == '*')
                    collision( &player, terrainarray[i][j] , 2 );
        
        moveplayer( &player , settings );

        /********************** PLAYER BULLET SHOOTING *******************************/
        playershoot( &player , &bullet[0] , settings );
        shooting( settings , &bullet[0] , bullet, &player , enemy , Menu , terrainspace, terrainarray , &textures );
        
        /********************** ENEMY HITBOX *******************************/
        for (int k = 0; k < settings->level; k++)
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
        for (int k = 0; k < settings->level; k++)
        {
            //Spawn logic
            enemyspawn( &enemy[k], settings->level , terrainspace , terrainarray , player.colRec , enemy );
            //Drawing needs to be done here else it causes a major bug
            if (enemy[k].health != 0)
                DrawTexturePro( textures.enemy , enemy[k].sourceRec , enemy[k].drawRec , enemy[k].cen , enemy[k].rot , enemy[k].color ); //Draws Enemy tank
            //Will be removed when bullet.c is done
        }

        /********************** ENEMY COLLISION/MOVEMENT *******************************/
        for (int k = 0; k < settings->level; k++)
        {
            //Resets collision detection
            enemy[k].colSide = (Vector4){ 0 , 0 , 0 , 0 };
            //Tests collision with sides
            for (int i = 0; i < 4; i++)
                collision( &enemy[k] , Menu[i] , 2);
            //Tests collision with player
            collision( &enemy[k] , player.colRec , 2);
            //Tests collision with each rectangle of terrain
            for (int i = 0; i < GetScreenHeight()/(GetScreenHeight()/12); i++)
                for (int j = 0; j < (int)ceil(GetScreenWidth()/(GetScreenHeight()/12)); j++)
                    if (terrainspace[i][j] == '*')
                        collision( &enemy[k] , terrainarray[i][j] , 2 );
            //Tests collision against other enemys
            for (int i = 0; i < settings->level; i++)
                collision( &enemy[k] , enemy[i].colRec , 2 );
            //When map destruction is done need to lower enemy[i] sight distance, very easy
            if ( enemy[k].health >= 1 )
                enemymove( &enemy[k], &player );
        }
        

        /********************** ENEMY BULLET SHOOTING *******************************/
        for (int k = 0; k < settings->level; k++)
        {
            if (!GetRandomValue(0,15) && bullet[settings->players + k].ammo == true && enemy[k].health >= 1) //Verify if enemy[k] has ammo
                shoot( &enemy[k], &bullet[settings->players + k] );

            shooting( settings , &bullet[settings->players + k] , bullet , &player , enemy , Menu, terrainspace, terrainarray , &textures);
        }

        /********************** WINNING VARIABLES *******************************/
        if ( player.score + settings->score >= settings->score + 800 * settings->level )
        {
            player.time++;
            player.health = 3;
            DrawText( "LEVEL COMPLETE", GetScreenWidth() / 2 - MeasureText("LEVEL COMPLETE", GetFontDefault().baseSize) * 2 , GetScreenHeight() / 2  , 40 , GOLD );
            if ( player.time == 60 * 2 )
            {
                settings->won = true;
                break;
            }
        }else
            settings->won = false;
        
        if(player.health <= 0)
        {
            player.time++;
            DrawText( "VOCE MORREU", GetScreenWidth() / 2 - MeasureText("VOCE MORREU", GetFontDefault().baseSize) * 2 , GetScreenHeight() / 2  , 40 , RED );
            if ( player.time == 60 * 2 )
                break;
        }
        
        EndDrawing();
    }

    /********************** UNLOADING AREA *******************************/
    settings->score += player.score;

    UnloadTexture(textures.player);  //Texture for the player tank
    UnloadTexture(textures.enemy); //Texture for the enemy tank
    UnloadTexture(textures.bullet); //Texture for the bullet
    UnloadTexture(textures.explosion); //Texture for the explosion for bullets
    UnloadTexture(textures.wall); //Wall texture
    UnloadTexture(textures.smoke); //Smoke texture
    UnloadTexture(textures.health); //Health texture
    UnloadTexture(textures.energy); //Energy texture
}