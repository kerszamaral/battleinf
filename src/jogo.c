#include "jogo.h"
#include "core.h"
#include "raymath.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "shooting.h"
#include "terrain.h"
#include "menu.h"
#include "saveload.h"



void jogo(Setti *settings)
{
    SetExitKey(0);
    /********************** TEXTURES *******************************/
    Textus textures = {
        LoadTexture("assets/player.png"),  //Texture for the player tank
        LoadTexture("assets/enemy.png"), //Texture for the enemy tank
        LoadTexture("assets/bullet.png"), //Texture for the bullet
        LoadTexture("assets/explosionBullets.png"), //Texture for the explosion for bullets
        LoadTexture("assets/wall.png"), //Wall texture
        LoadTexture("assets/fire.png"), //Smoke texture
        LoadTexture("assets/health.png"), //Health texture
        LoadTexture("assets/energy.png"), //Energy texture
        LoadTexture("assets/explosionVehicles.png") //Texture for the explosion of vehicles
        //LoadTexture("") //Terrain texture
    };
    /********************** SOUNDS *******************************/
    SFX sounds = {
        LoadSound("assets/gamestartSFX.wav"),  //Sound for the start of the game
        LoadSound("assets/gameendSFX.wav"), //Sound for the end of the game
        LoadSound("assets/explosionVehicleSFX.wav"), //Sound for the explosion
        LoadSound("assets/BulletShotSFX.wav"), //Sound for the shoot
        LoadSound("assets/playerpassiveSFX.wav"), //Sound for the player //! Disabled for now because it's too annoying
        LoadSound("assets/BulletMissSFX.wav"), //Sound for the bullet miss
        LoadSound("assets/TerrainHitSFX.wav") //Sound for the terrain hit
    };

    SetSoundVolume(sounds.playerpassive, 0.006);
    SetSoundVolume(sounds.gamestart, 0.03);
    SetSoundVolume(sounds.gameend, 0.05);
    SetSoundVolume(sounds.explosion, 0.02);
    SetSoundVolume(sounds.shoot, 0.02);
    SetSoundVolume(sounds.bulletmiss, 0.02);
    SetSoundVolume(sounds.terrainhit, 0.02);

    /********************** MENU VARIABELS *******************************/
    ClearWindowState(FLAG_WINDOW_RESIZABLE);
    //Rectangles for collision and drawing (dest rectangles need to be in while loop)
    Rectangle Menu[4] = {
    (Rectangle){ 0 , 0 , GetScreenWidth() , 50 * (GetScreenHeight()*(1.0/655)) }, //Rectangle for the ingame menu
    (Rectangle){ 0 , GetScreenHeight() - 5*(GetScreenHeight()*(1.0/655)) , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
    (Rectangle){ 0 , 0 , 5 * (GetScreenWidth()*(1.0/1010)), GetScreenHeight() }, //Rectangle for left border
    (Rectangle){ GetScreenWidth() - 5 * (GetScreenWidth()*(1.0/1010)) , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
    };

    /********************** PLAYER VARIABELS *******************************/
    //Object
    Obj player[settings->players]; 
    player->ratio = (float) textures.player.width / textures.player.height; //!Fixes weird bug found to fill entire screen
    for (int i = 0; i < settings->players; i++)
    {
        player[i] = 
        (Obj){
            i, //id of the object
            (Vector2){ 0 , 0 }, //Vector2 pos, x and y
            (float) textures.player.width / textures.player.height, //ratio
            (Vector2){ textures.player.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 20 ) , ( textures.player.height * player->ratio ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 20 ) }, //Vector2 center, x and y
            (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
            3, //Health
            0, //Object rotation
            0, //Score
            0, //Time
            0, //Death
            2*(GetScreenHeight()*(1.0/655)), //Speed
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
        switch (player[i].id)
        {
            case 1:
                player[i].color = RED;
                break;
            case 2:
                player[i].color = GREEN;
                break;
            case 3:
                player[i].color = ORANGE;
                break;
            case 4:
                player[i].color = MAGENTA;
                break;
            default:
                player[i].color = WHITE;
                break;
        }
    }
    

    /********************** ENEMY VARIABELS *******************************/
    //Object
    Obj enemy[ settings->level ];
    enemy->ratio = (float) textures.enemy.width / textures.enemy.height; //!Fixes weird bug found to fill entire screen
    for (int i = 0; i < settings->level; i++)
    {
        enemy[i] = 
        (Obj){
            i, //id of the object
            (Vector2){ GetScreenWidth() , GetScreenHeight() }, //Vector2 pos, x and y     Bullets spawn 0,0 conflict enemy spawn
            (float) textures.enemy.width / textures.enemy.height, //ratio
            (Vector2){ textures.enemy.width  * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 20 ) , ( textures.enemy.height * enemy->ratio )  * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 20 ) }, //Vector2 center, x and y
            (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
            0, //Health
            0, //Object rotation
            0, //Score
            0, //Time
            0, //Death
            1*(GetScreenHeight()*(1.0/655)), //Speed
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
    bullet->ratio = (float) textures.bullet.width / textures.bullet.height; //!Fixes weird bug found by player that caused bullets to fill entire screen
    for (int i = 0; i < settings->players + settings->level ; i++) //Fills in the variables for all bullets
    {
        bullet[i] =
        (Obj){
            i, //id of the object
            (Vector2){ 0 , 0 }, //Vector2 pos, x and y
            (float) textures.bullet.width / textures.bullet.height, //ratio
            (Vector2){ textures.bullet.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 100 ) , ( textures.bullet.height * bullet->ratio ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 100 ) }, //Vector2 center, x and y
            (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
            0, //Health
            0, //Object rotation
            0, //Score
            0, //Time
            0, //Death
            3*(GetScreenHeight()*(1.0/655)), //Speed
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
        (Vector2){ textures.energy.width  * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 30 ) , textures.energy.height  * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 30 ) }, //Vector2 center, x and y
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
    
    bool levelfilefound = false;
    char filename[100];
    if (!settings->loadgame)
        if ( FileExists( TextFormat( "saves/nivel%d.txt", settings->level ) ) )
        {
            levelfilefound = true;
            strcpy( filename , TextFormat( "saves/nivel%d", settings->level ) );
        }
    //Random Map Generator for testing, needs to be replaced by read file
    char terrainspace [ 15 * 41 ];   //15x41 terrain space 
    if ( !levelfilefound && !settings->loadgame )
            terraincreate(terrainspace);
        
    //Creates the actual rectangles in the right place
    Rectangle terrainarray[ 15 * 41 ];
    if ( !levelfilefound && !settings->loadgame )
        terrainplace( terrainarray , terrainspace );
    
    if (levelfilefound)
        loading( filename, settings, player, enemy, &energy, terrainarray, terrainspace, 1 );

    if (settings->loadgame)
    {
        strcpy( filename , TextFormat( "saves/%s", settings->loadgamename ) );
        loading( filename, settings, player, enemy, &energy, terrainarray, terrainspace, 1 );
    }
    //Random player starting position
    if ( settings->foundplayerposition <= settings->players )
        for (int p = settings->foundplayerposition; p < settings->players; p++)
            spawn( settings , &player[p] , terrainspace , terrainarray , player , enemy);
    
    PlaySound(sounds.gamestart);

    //!Main game loop
    while( !settings->exitgame && !WindowShouldClose() ) //End if you press esc or player.health gets to 0
    {
        BeginDrawing();

        ClearBackground( settings->theme ); //Background color
        
        if (IsKeyPressed(KEY_ESCAPE) || IsGamepadButtonPressed( 0 , 15) ) //Ends game if esc is pressed
            settings->pause = !settings->pause;
        

        /********************** TERRAIN CREATION *******************************/
        for (int i = 0; i < 15 * 41; i++)
                if (terrainspace[i] == '#')
                {
                    for (int k = 0; k < settings->players + settings->level; k++)
                        terrainarray[i] = terraindestruct( bullet[k], terrainarray[i], &sounds );
                    DrawTexturePro( textures.wall , sourceWall , terrainarray[i] , (Vector2){ 0 , 0 } , 0 , WHITE );
                }

        /********************** MENU CREATION *******************************/
        //Draws solid color rectangles
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        int score = settings->score;
        for (int p = 0; p < settings->players; p++)
            score += player[p].score;
        //Text 
        DrawText( TextFormat( "Fase %d" , settings->level ) , GetScreenWidth() / 2 - MeasureText(TextFormat( "Fase %d" , settings->level ), GetFontDefault().baseSize) * (GetScreenHeight()*(1.0/655)) , 10*(GetScreenHeight()*(1.0/655)) , 40*(GetScreenHeight()*(1.0/655)) , YELLOW );
        DrawText( TextFormat( "Pontuação: %d", score ), GetScreenWidth() - MeasureText(TextFormat( "Pontuação: %d", score ), GetFontDefault().baseSize) * 3.2 * (GetScreenHeight()*(1.0/655)) , 13 * (GetScreenHeight()*(1.0/655)) , 32*(GetScreenHeight()*(1.0/655)) , RED );
        DrawText( TextFormat( "Inimigos restantes: %d/%d", settings->level - (score - settings->score) / 800 , settings->level ),
        MeasureText(TextFormat( "Inimigos restantes: %d/%d", settings->level - (score - settings->score) / 800 , settings->level ), GetFontDefault().baseSize)*(GetScreenHeight()*(1.0/655)) + 10 , 15 * (GetScreenHeight()*(1.0/655)) , 24*(GetScreenHeight()*(1.0/655)) , BLUE );
        //Draws player health for health remaining            spacing from image size x * scaling
        for (int p = 0, healthy = 10; p < settings->players; p++, healthy += (45 * (GetScreenHeight()*(1.0/655)))/settings->players )
        {    
            for ( int i = 0, healthx = 5 ; i < player[p].health ; i++ , healthx += (35 * (GetScreenHeight()*(1.0/655)))/settings->players )
                DrawTextureEx( textures.health , (Vector2){ healthx , healthy } , 0 , (0.05 * (GetScreenHeight()*(1.0/655)))/settings->players, player[p].color );
        }

        /********************** ENERGY DRAWING/COLLISION *******************************/
        //Energy Spawning
        if ( energy.time >= 60*3 && !energy.health && !GetRandomValue( 0 , 63 ) && !settings->pause )
        {
            spawn( settings , &energy , terrainspace , terrainarray , player , enemy);
            energy.draw = (Vector2){ energy.pos.x + energy.cen.x , energy.pos.y + energy.cen.y };
            energy.colRec = (Rectangle){ energy.pos.x , energy.pos.y , energy.cen.x*2 , energy.cen.y*2 };
            energy.health = 1;
        }
        else if (!settings->pause)
            energy.time++;
        //Energy Drawing
        if ( energy.health >= 1 )
        {
            energy.draw = (Vector2){ energy.pos.x + energy.cen.x , energy.pos.y + energy.cen.y };
            energy.colRec = (Rectangle){ energy.pos.x , energy.pos.y , energy.cen.x*2 , energy.cen.y*2 };
            DrawTexturePro( textures.energy , energy.sourceRec , energy.colRec , (Vector2){ 0 , 0 } , 0 , WHITE );
            for (int p = 0; p < settings->players; p++)
                collision( &energy , player[p].colRec , 2 );
            
            energy.time = 0;
            if ( energy.colSide.x || energy.colSide.y || energy.colSide.z || energy.colSide.w )
            {   //Energy pickup
                energy.health--;
                for (int p = 0; p < settings->players; p++)
                {
                    player[p].speed *= 1.5;
                    bullet[p].speed *= 1.5;
                }
                energy.ammo = true;
            }
        }
        //Energy falloff
        if ( energy.time >= 60*3 && energy.ammo && !settings->pause )
        {
            for (int p = 0; p < settings->players; p++)
            {
                player[p].speed /= 1.5;
                bullet[p].speed /= 1.5;
            }
            energy.ammo = false;
        }
        
        /********************** For multiplayer*******************************/
        for (int p = 0; p < settings->players; p++)
        {
            /********************** PLAYER DRAWING *******************************/
            //Draw position and draw rectangle update
            //Sets player.draw to be player.pos + offset
            player[p].draw = (Vector2){ player[p].pos.x + player[p].cen.x , player[p].pos.y + player[p].cen.y };
            //Rectangle resized and offset for player[p] drawing
            player[p].drawRec = (Rectangle){ player[p].draw.x, player[p].draw.y, player[p].cen.x*2 , player[p].cen.y*2 }; 
            //player[p] collision rectangle update
            player[p].colRec = (Rectangle){ player[p].pos.x , player[p].pos.y , player[p].cen.x*2 , player[p].cen.y*2 };
            //Because player[p] cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
            if (player[p].health > 0)
            {
                DrawTexturePro( textures.player , player[p].sourceRec , player[p].drawRec , player[p].cen , player[p].rot , player[p].color ); //Draws player[p] tank
            }
            if (player[p].health <= 0)
            {
                if (!player[p].dying && player[p].health != -100)
                {                
                    PlaySoundMulti(sounds.explosion);
                    player[p].deathpos = player[p].draw;
                    player[p].dying = true;
                }
                player[p].pos = (Vector2){ 0 , 0 };
            }
            if (player[p].dying)
            {  //Player death
                DrawTexturePro( textures.explosionVehicles , (Rectangle){ textures.explosionVehicles.width/36 * player[p].deathtimer , 0 , textures.explosionVehicles.width/39 , textures.explosionVehicles.height } , (Rectangle){player[p].deathpos.x ,player[p].deathpos.y  , textures.explosionVehicles.width/180, textures.explosionVehicles.height/5 } , (Vector2){ (textures.explosionVehicles.width/180)/2 , (textures.explosionVehicles.height/5)/2 } ,player[p].rot , WHITE );
                if (!settings->pause)
                    player[p].deathtimer++;
                if (player[p].deathtimer > 36)
                {
                   player[p].deathtimer = 0;
                   player[p].dying = false;
                   player[p].health = -100;
                }
            }

            /********************** PLAYER COLLISION/MOVEMENT *******************************/
            //Resets collision detection
            player[p].colSide = (Vector4){ 0 , 0 , 0 , 0 };
            //Tests collision with sides
            for (int i = 0; i < 4; i++)
                collision( &player[p] , Menu[i], 2 );
            //Tests collision with enemy
            for (int i = 0; i < settings->level; i++)
                collision( &player[p] , enemy[i].colRec , 2 );
            //Tests collision with other players
            for (int i = 0; i < settings->players; i++)
                if (i != p)
                    collision( &player[p] , player[i].colRec , 2 );
            //Tests collision with each rectangle of terrain
            for (int i = 0; i < 15 * 41; i++)
                    if (terrainspace[i] == '#')
                        collision( &player[p], terrainarray[i] , 2 );
            if (!settings->pause)
                moveplayer( &player[p] , settings );
            /********************** PLAYER BULLET SHOOTING *******************************/
            if (!settings->pause)
                playershoot( &player[p] , &bullet[p] , settings , &sounds);
            shooting( settings , &bullet[p] , bullet, player , enemy , Menu , &sounds , terrainspace, terrainarray , &textures );
        }

        /********************** MULTIPLE ENEMIES *******************************/
        for (int k = 0; k < settings->level; k++)
        {
            /********************** ENEMY HITBOX *******************************/
            //Draw position and draw rectangle update
            //Sets enemy.draw to be enemy.pos + offset
            enemy[k].draw = (Vector2){ enemy[k].pos.x + enemy[k].cen.x , enemy[k].pos.y + enemy[k].cen.y }; 
            //Rectangle resized and offset for enemy[k] drawing
            enemy[k].drawRec = (Rectangle){ enemy[k].draw.x, enemy[k].draw.y, enemy[k].cen.x*2 , enemy[k].cen.y*2 };
            //Enemy collision rectangle
            enemy[k].colRec = (Rectangle){ enemy[k].pos.x , enemy[k].pos.y , enemy[k].cen.x*2 , enemy[k].cen.y*2 };
            //Because enemy cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
            /********************** ENEMY SPAWNING *******************************/
            //Spawn logic
            if (!settings->pause && settings->enemiesremaining < settings->level)
                enemyspawn( settings , &enemy[k], terrainspace , terrainarray , player , enemy );
            //Drawing needs to be done here else it causes a major bug
            if (enemy[k].health >= 1)
                DrawTexturePro( textures.enemy , enemy[k].sourceRec , enemy[k].drawRec , enemy[k].cen , enemy[k].rot , enemy[k].color ); //Draws Enemy tank
            
            /********************** ENEMY COLLISION/MOVEMENT *******************************/
            //Resets collision detection
            enemy[k].colSide = (Vector4){ 0 , 0 , 0 , 0 };
            //Tests collision with sides
            for (int i = 0; i < 4; i++)
                collision( &enemy[k] , Menu[i] , 2);
            //Tests collision with player
            for (int p = 0; p < settings->players; p++)
                collision( &enemy[k] , player[p].colRec , 2);
            //Tests collision with each rectangle of terrain
            for (int i = 0; i < 15 * 41; i++)
                    if (terrainspace[i] == '#')
                        collision( &enemy[k] , terrainarray[i] , 2 );
            //Tests collision against other enemys
            for (int i = 0; i < settings->level; i++)
                collision( &enemy[k] , enemy[i].colRec , 2 );
            //TODO When map destruction is done need to lower enemy[i] sight distance, very easy
            if ( enemy[k].health >= 1 && !settings->pause)
                enemymove( settings , &enemy[k] , player );
            if (enemy[k].dying)
            {  //Player death
                DrawTexturePro( textures.explosionVehicles , (Rectangle){ textures.explosionVehicles.width/36 * enemy[k].deathtimer , 0 , textures.explosionVehicles.width/39 , textures.explosionVehicles.height } , (Rectangle){enemy[k].deathpos.x ,enemy[k].deathpos.y  , textures.explosionVehicles.width/180, textures.explosionVehicles.height/5 } , (Vector2){ (textures.explosionVehicles.width/180)/2 , (textures.explosionVehicles.height/5)/2 } ,enemy[k].rot , WHITE );
                if (!settings->pause)
                    enemy[k].deathtimer++;
                if (enemy[k].deathtimer > 36)
                {
                   enemy[k].deathtimer = 0;
                   enemy[k].dying = false;
                }
            }
            /********************** ENEMY BULLET SHOOTING *******************************/
            if (!GetRandomValue(0,15) && bullet[settings->players + k].ammo == true && enemy[k].health >= 1 && !settings->pause) //Verify if enemy[k] has ammo
                shoot( &enemy[k], &bullet[settings->players + k] , &sounds );
            shooting( settings , &bullet[settings->players + k] , bullet , player , enemy , Menu, &sounds , terrainspace, terrainarray , &textures);
        }

        if (settings->pause)
            pausescreen( settings, terrainspace , player, enemy , &energy, bullet, terrainarray);
        else
            settings->pauseselect = 0;
            
        /********************** WINNING VARIABLES *******************************/
        if ( score >= settings->score + 800 * settings->level )
        {
            if (!settings->pause)
                player[0].time++;

            DrawText( "LEVEL COMPLETE", GetScreenWidth() / 2 - MeasureText("LEVEL COMPLETE", GetFontDefault().baseSize) * 2 , GetScreenHeight() / 2  , 40 , GOLD );
            if ( player[0].time == 60 * 2 )
            {
                PlaySound(sounds.gameend);
                settings->won = true;
                break;
            }
        }else
            settings->won = false;

        int deathcount = 0;
        for (int p = 0; p < settings->players; p++)
            if( player[p].health <= 0 )
                deathcount++;
        if( deathcount >= settings->players )
        {
            if (!settings->pause)
                player[0].time++;
            DrawText( "VOCE MORREU", GetScreenWidth() / 2 - MeasureText("VOCE MORREU", GetFontDefault().baseSize) * 2 , GetScreenHeight() / 2  , 40 , RED );
            if ( player[0].time == 60 * 2 )
            {
                PlaySound(sounds.gameend);
                break;
            }
        }
        
        EndDrawing();
    }
    /********************** END GAME *******************************/
    for (int p = 0; p < settings->players; p++)
        settings->score += player[p].score;
    memset( terrainspace, 0, sizeof(terrainspace) );
    memset( terrainarray, 0, sizeof(terrainarray) );
    memset( filename, 0, sizeof(filename) );

    StopSoundMulti(); 
    /********************** UNLOADING AREA *******************************/
    UnloadSound(sounds.gamestart);  //Sound for the start of the game
    UnloadSound(sounds.gameend); //Sound for the end of the game
    UnloadSound(sounds.explosion); //Sound for the explosion
    UnloadSound(sounds.shoot); //Sound for the shoot
    UnloadSound(sounds.playerpassive); //Sound for the player
    UnloadSound(sounds.bulletmiss); //Sound for the bullet miss
    UnloadSound(sounds.terrainhit); //Sound for the terrain hit

    UnloadTexture(textures.player);  //Texture for the player tank
    UnloadTexture(textures.enemy); //Texture for the enemy tank
    UnloadTexture(textures.bullet); //Texture for the bullet
    UnloadTexture(textures.explosion); //Texture for the explosion for bullets
    UnloadTexture(textures.wall); //Wall texture
    UnloadTexture(textures.smoke); //Smoke texture
    UnloadTexture(textures.health); //Health texture
    UnloadTexture(textures.energy); //Energy texture
    UnloadTexture(textures.explosionVehicles); //Texture for the explosion for vehicles
}