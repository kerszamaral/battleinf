#include "game.h"

//Game
void game( Setti *settings )
{
    SetExitKey(0); //Set the exit key to 0, ESC is now pause
    ClearWindowState( FLAG_WINDOW_RESIZABLE ); //Disable resizing
    settings->realTime = GetTime(); //Get the current time

    /********************** TEXTURES *******************************/
    Textus textures = 
    {   //Loads the textures
        LoadTexture( "assets/player.png" ),  //Texture for the player tank
        LoadTexture( "assets/enemy.png" ), //Texture for the enemy tank
        LoadTexture( "assets/bullet.png" ), //Texture for the bullet
        LoadTexture( "assets/explosionBullets.png" ), //Texture for the explosion for bullets
        LoadTexture( "assets/wall.png" ), //Wall texture
        LoadTexture( "assets/fire.png" ), //Smoke texture
        LoadTexture( "assets/health.png" ), //Health texture
        LoadTexture( "assets/energy.png" ), //Energy texture
        LoadTexture( "assets/explosionVehicles.png" ) //Texture for the explosion of vehicles
    };

    /********************** SOUNDS *******************************/
    SFX sounds = 
    {   //Loads the sounds
        LoadSound( "assets/gamestartSFX.wav" ),  //Sound for the start of the game
        LoadSound( "assets/gameendSFX.wav" ), //Sound for the end of the game
        LoadSound( "assets/BulletShotSFX.wav" ), //Sound for the shoot
        LoadSound( "assets/BulletMissSFX.wav" ), //Sound for the bullet miss
        LoadSound( "assets/TerrainHitSFX.wav" ) //Sound for the terrain hit
    };
    //Sound volume (for not going deaf)
    SetSoundVolume( sounds.gamestart, 0.03 );
    SetSoundVolume( sounds.gameend, 0.05 );
    SetSoundVolume( sounds.shoot, 0.02 );
    SetSoundVolume( sounds.bulletmiss, 0.02 );
    SetSoundVolume( sounds.terrainhit, 0.02 );

    /********************** MENU VARIABELS *******************************/
    //Rectangles for the Menu
    Rectangle Menu[4] = 
    {
        (Rectangle){ 0 , 0 , GetScreenWidth() , 50*scaleY() }, //Rectangle for the ingame menu
        (Rectangle){ 0 , GetScreenHeight() - 5*scaleY() , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
        (Rectangle){ 0 , 0 , 5*scaleX(), GetScreenHeight() }, //Rectangle for left border
        (Rectangle){ GetScreenWidth() - 5*scaleX() , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
    };

    /********************** PLAYER VARIABELS *******************************/
    //Object
    Obj player[settings->players]; 
    player->ratio = (float) textures.player.width / textures.player.height; //!Fixes weird bug found to fill entire screen
    for (int i = 0; i < settings->players; i++) //Loop for multiplayer support
    {
        player[i] = 
        (Obj){
            i, //id of the object
            (Vector2){ 0, 0 }, //Vector2 pos, x and y
            (float) textures.player.width / textures.player.height, //ratio
            (Vector2){ textures.player.width * scaleY()/20, ( textures.player.height * player->ratio ) * scaleY()/20 }, //Vector2 center, x and y
            (Vector2){ 0, 0 }, //Vector 2 for drawing position, has x and y
            3, //Health
            0, //Object rotation
            0, //Score
            0, //Time
            0, //Death
            2*scaleY(), //Speed
            false, //Ammo
            (Rectangle){ 0, 0, textures.player.width, textures.player.height }, //sourceRec
            (Rectangle){ 0, 0, 0, 0 }, //colRec for object collision, created here, updated in loop
            (Rectangle){ 0, 0, 0, 0 }, //drawRec for drawing and object rotation, created here, updated in loop
            (Vector4){ 0, 0, 0, 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
            WHITE, //Color for enemy difficulty and mutliplayer
            false, //dying
            (Vector2){ 0, 0 }, //deathpos
            0, //deathtimer
            LoadSound( "assets/explosionVehicleSFX.wav" ), //Sound for the explosion
            LoadSound( "assets/gamestartSFX.wav" ),  //! just for not complaining
            GetTime() - SHOOTINGDELAY //For realTime
        };
        SetSoundVolume( player[i].soundEffect, 0.02 ); //Sound effect volume

        switch ( player[i].id ) //Switch for setting colors for each player
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
    for (int i = 0; i < settings->level; i++) //Loop for creating enemies enough for each level
    {
        enemy[i] = 
        (Obj){
            i, //id of the object
            (Vector2){ GetScreenWidth(), GetScreenHeight() }, //Vector2 pos, x and y     Bullets spawn 0,0 conflict enemy spawn
            (float) textures.enemy.width / textures.enemy.height, //ratio
            (Vector2){ textures.enemy.width * scaleY()/20, ( textures.enemy.height * enemy->ratio ) * scaleY()/20 }, //Vector2 center, x and y
            (Vector2){ 0, 0 }, //Vector 2 for drawing position, has x and y
            0, //Health
            0, //Object rotation
            0, //Score
            0, //Time
            0, //Death
            1*scaleY(), //Speed
            false, //Ammo
            (Rectangle){ 0, 0, textures.enemy.width, textures.enemy.height }, //sourceRec
            (Rectangle){ 0, 0, 0, 0 }, //colRec for object collision, created here, updated in loop
            (Rectangle){ 0, 0, 0, 0 }, //drawRec for drawing and object rotation, created here, updated in loop
            (Vector4){ 0, 0, 0, 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
            WHITE, //Color for enemy difficulty and mutliplayer
            false, //dying
            (Vector2){ 0 , 0 }, //deathpos
            0, //deathtimer
            LoadSound( "assets/explosionVehicleSFX.wav" ), //Sound for the explosion
            LoadSound( "assets/gamestartSFX.wav" ),  //! just for not complaining
            GetTime() - SHOOTINGDELAY //For realTime
        };
        SetSoundVolume(enemy[i].soundEffect, 0.02); //Sound effect volume

        if (!GetRandomValue( 0 , 15 )) //Sets enemy difficulty and color
            enemy[i].color = RED;
        else
            enemy[i].color = WHITE;
    }

    /********************** BULLET VARIABELS *******************************/
    //Objects
    Obj bullet[ settings->players + settings->level ]; //Declares the number of bullets used for the level
    bullet->ratio = (float) textures.bullet.width / textures.bullet.height; //!Fixes weird bug found by player that caused bullets to fill entire screen
    for (int i = 0; i < settings->players + settings->level ; i++) //Loop for creating enough bullets for players + enemies
    {
        bullet[i] =
        (Obj){
            i, //id of the object
            (Vector2){ 0, 0 }, //Vector2 pos, x and y
            (float) textures.bullet.width / textures.bullet.height, //ratio
            (Vector2){ textures.bullet.width * scaleY()/100, ( textures.bullet.height * bullet->ratio ) * scaleY()/100 }, //Vector2 center, x and y
            (Vector2){ 0, 0 }, //Vector 2 for drawing position, has x and y
            0, //Health
            0, //Object rotation
            0, //Score
            0, //Time
            0, //Death
            3*scaleY(), //Speed
            true, //Ammo
            (Rectangle){ 0, 0, textures.bullet.width, textures.bullet.height }, //sourceRec
            (Rectangle){ 0, 0, 0, 0 }, //colRec for object collision, created here, updated in loop
            (Rectangle){ 0, 0, 0, 0 }, //drawRec for drawing and object rotation, created here, updated in loop
            (Vector4){ 0, 0, 0, 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
            WHITE, //Color for enemy difficulty and mutliplayer
            false, //dying
            (Vector2){ 0, 0 }, //deathpos
            0, //deathtimer
            LoadSound( "assets/BulletShotSFX.wav" ), //Sound for firing
            LoadSound( "assets/BulletMissSFX.wav" ), //Sound for missing
            GetTime() - SHOOTINGDELAY //For realTime
        };
        SetSoundVolume(bullet[i].soundEffect, 0.02); //Sound effect volume
        SetSoundVolume(bullet[i].soundEffect2, 0.02); //Sound effect2 volume
    };

    /********************** ENERGY VARIABELS *******************************/
    //Object
    Obj energy = 
    {
        0, //id of the object
        (Vector2){ 0, 0 }, //Vector2 pos, x and y
        (float) textures.energy.width / textures.energy.height, //ratio
        (Vector2){ textures.energy.width * scaleY()/30 , textures.energy.height * scaleY()/30 }, //Vector2 center, x and y
        (Vector2){ 0, 0 }, //Vector 2 for drawing position, has x and y
        0, //Health
        0, //Object rotation
        0, //Score
        0, //Time
        0, //Death
        0, //Speed
        false, //Ammo
        (Rectangle){ 0, 0, textures.energy.width, textures.energy.height }, //sourceRec
        (Rectangle){ 0, 0, 0, 0 }, //colRec for object collision, created here, updated in loop
        (Rectangle){ 0, 0, 0, 0 }, //drawRec for drawing and object rotation, created here, updated in loop
        (Vector4){ 0, 0, 0, 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
        WHITE, //Color for enemy difficulty and mutliplayer
        false, //dying
        (Vector2){ 0, 0 }, //deathpos
        0, //deathtimer
        LoadSound("assets/gamestartSFX.wav"),  //! just for not complaining
        LoadSound("assets/gamestartSFX.wav"),  //! just for not complaining
        GetTime() - SHOOTINGDELAY //For realTime
    };
    
    /********************** TERRAIN VARIABELS *******************************/
    Rectangle sourceWall = { 0, 0, textures.wall.width, textures.wall.height }; //Rectangle with size of original image
    bool levelfilefound = false; //For knowing if a file with the level name was found
    char filename[100]; //For storing the filename
    char terrainspace [MAPSIZE];   //15x41 terrain space 
    Rectangle terrainarray[MAPSIZE];

    //*Creating
    //Load Map
    if ( !settings->loadgame ) //If the game is not loading a game
        if ( FileExists( TextFormat( "saves/nivel%d.txt", settings->level ) ) ) //If the file with the level name exists
        {
            levelfilefound = true;
            strcpy( filename, TextFormat( "saves/nivel%d", settings->level ) );
        }
    //Random Map Generator 
    if ( !levelfilefound && !settings->loadgame ) //If the level file was not found and the game is not loading a game
    {
        terraincreate( terrainspace ); //Creates a random map
        terrainplace( terrainarray, terrainspace );//Places the char array onto the rectangle arrray
    }
    
    if ( settings->loadgame ) //If the game is loading a game
        strcpy( filename, TextFormat( "saves/%s", settings->loadgamename ) ); //Copies the name of the file to the filename with the path
    
    if ( levelfilefound || settings->loadgame ) //If the level file was found or the game is loading a game
        loading( filename, settings, player, enemy, &energy, terrainarray, terrainspace, 1 ); //Loads the level and the game position for everything

    //Random player starting position
    if ( settings->foundplayerposition <= settings->players ) //If the player position was not found for all players
        for (int p = settings->foundplayerposition; p < settings->players; p++) //Loop for finding the player position for all players remaining
            spawn( settings, &player[p], terrainspace, terrainarray, player, enemy); //Spawns the player in a random position

    PlaySound( sounds.gamestart ); //Plays the game start sound

    //!Main game loop
    while( !settings->exitgame ) //End if the player.health gets to 0 or you select to exit on pause
    {
        BeginDrawing();
        ClearBackground( settings->theme ); //Background color
        
        if (IsKeyPressed(KEY_ESCAPE) || IsGamepadButtonPressed( 0 , 15) ) //Ends game if esc is pressed
        {
            settings->pause = !settings->pause; //Toggles pause
            strcpy(settings->error, " "); //Reset the error
        }
        /********************** TERRAIN CREATION *******************************/
        for (int i = 0; i < MAPSIZE; i++) //Loop for drawing the terrain
                if ( terrainspace[i] == '#' ) //Checks only for places where the char array says there's a wall
                {
                    for (int k = 0; k < settings->players + settings->level; k++) //Loop for player and enemy
                        terraindestruct( bullet[k], &terrainarray[i], &sounds ); //Destroys the terrain if the bullet hits it
                    DrawTexturePro( textures.wall , sourceWall , terrainarray[i] , (Vector2){ 0 , 0 } , 0 , WHITE ); //Draws the wall
                }
        
        //? ********************* MENU CREATION *******************************/
        //*Bars
        for (int i = 0; i < 4; i++) //Loop for drawing the menu in the 4 sides
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //*Text 
        int score = settings->score; //Score for the menu
        for (int p = 0; p < settings->players; p++) //Loop for drawing the player score
            score += player[p].score; //Adds the score of each player
        int enemiesrem = settings->level - (score - settings->score) / 800; //Enemies remaining for the menu
        DrawText( TextFormat( "Level %d" , settings->level ), GetScreenWidth() / 2 - MeasureText( TextFormat( "Level %d" , settings->level ), 40*scaleY() )/2, 10*scaleY(), 40*scaleY(), YELLOW );
        DrawText( TextFormat( "Score: %d", score ), GetScreenWidth() - MeasureText( TextFormat( "Score: %d", score ), 32*scaleY() ) - 10*scaleY(), 13*scaleY(), 32*scaleY(), RED );
        DrawText( TextFormat( "Enemies Remaining: %d/%d", enemiesrem, settings->level ), MeasureText( TextFormat( "Enemies Remaining: %d/%d", enemiesrem, settings->level ), 24*scaleY())/2, 15*scaleY(), 24*scaleY(), BLUE );
        
        //Draws player health for health remaining
        for (int p = 0, healthy = 10; p < settings->players; p++, healthy += (45*scaleY())/settings->players )   
            for ( int i = 0, healthx = 5 ; i < player[p].health; i++ , healthx += (35*scaleY())/settings->players )
                DrawTextureEx( textures.health, (Vector2){ healthx, healthy }, 0, (0.05*scaleY())/settings->players, player[p].color );

        //? ******************** ENERGY *******************************/
        if ( !energy.health )
        {
            //*Spawning
            if ( energy.time >= 60*3 && !GetRandomValue( 0, 63 ) && !settings->pause ) //Spawns energy every 3 seconds
            {
                energy.time = 0; //Resets the energy timer
                spawn( settings, &energy, terrainspace, terrainarray, player, enemy); //Spawns the energy at random position
                energy.draw = (Vector2){ energy.pos.x + energy.cen.x, energy.pos.y + energy.cen.y }; //Sets the energy draw position
                energy.colRec = (Rectangle){ energy.pos.x, energy.pos.y, energy.cen.x*2, energy.cen.y*2 }; //Sets the energy collision rectangle
                energy.health = 1; //Sets the energy health to 1
            }
            else if ( !settings->pause ) //If the game is not pausedd
                energy.time++; //Increases the energy time
        }
        else
        {
            //*Drawing
            DrawTexturePro( textures.energy, energy.sourceRec, energy.colRec, (Vector2){ 0 , 0 }, 0, WHITE ); //Draws the energy
            for (int p = 0; p < settings->players; p++) //Loop for each player
                collision( &energy , player[p].colRec , 2 ); //Checks for collision with the energy
            if ( energy.colSide.x || energy.colSide.y || energy.colSide.z || energy.colSide.w ) //If the energy has collision with player 
            {
                energy.health--; //Decreases the energy health
                for (int p = 0; p < settings->players; p++) //Loop for each player
                {
                    player[p].speed *= 1.5; //Increases the player speed
                    bullet[p].speed *= 1.5; //Increases the bullet speed
                }
                energy.ammo = true; //Sets the energy ammo to true
            }
        }
        //*Energy falloff
        if ( energy.time >= 60*3 && energy.ammo ) //if three seconds has passed since the energy was picked up (prevents stacking speed)
        {
            for (int p = 0; p < settings->players; p++) //Loop for each player
            {
                player[p].speed /= 1.5; //Decreases the player speed
                bullet[p].speed /= 1.5; //Decreases the bullet speed
            }
            energy.ammo = false; //Sets the energy ammo to false
        }
        //? ********************* PLAYERS **************************************/
        for (int p = 0; p < settings->players; p++) //For Multiplayer support
        {
            //*Drawing (Because cen is the center(1/2) of the image scaled, multiply by 2 to get the full size)
            player[p].draw = (Vector2){ player[p].pos.x + player[p].cen.x, player[p].pos.y + player[p].cen.y }; //Draw position and draw rectangle update / Sets player.draw to be player.pos + offset
            player[p].drawRec = (Rectangle){ player[p].draw.x, player[p].draw.y, player[p].cen.x*2, player[p].cen.y*2 }; //Rectangle resized and offset for player[p] drawing
            player[p].colRec = (Rectangle){ player[p].pos.x, player[p].pos.y, player[p].cen.x*2, player[p].cen.y*2 }; //player[p] collision rectangle update
            
            if ( player[p].health > 0 ) //If the player is alive
                DrawTexturePro( textures.player , player[p].sourceRec , player[p].drawRec , player[p].cen , player[p].rot , player[p].color ); //Draws player[p] tank
            else //If the player has died
            {
                if ( !player[p].dying && player[p].health != -100 ) //Start the death sequence
                {                
                    PlaySound(player[p].soundEffect); //Play Death sound
                    player[p].deathpos = player[p].draw; //Store death position
                    player[p].dying = true; //Set dying to true
                }
                player[p].pos = (Vector2){ 0 , 0 }; //Reset player position to 0,0
            }
            if ( player[p].dying ) //If dying is true
            {
                DrawTexturePro( textures.explosionVehicles, (Rectangle){ textures.explosionVehicles.width/36 * player[p].deathtimer, 0, textures.explosionVehicles.width/39, textures.explosionVehicles.height }, (Rectangle){player[p].deathpos.x, player[p].deathpos.y, textures.explosionVehicles.width/180, textures.explosionVehicles.height/5 }, (Vector2){ (textures.explosionVehicles.width/180)/2, (textures.explosionVehicles.height/5)/2 }, player[p].rot, WHITE ); //Draw explosion animation
                if (!settings->pause) //If the game is not paused
                    player[p].deathtimer++; //Increase the death timer for animation
                if (player[p].deathtimer > 36) //If the death timer is greater than 36
                {
                    player[p].health = -100; //Set the player health to -100
                    player[p].dying = false; //Set dying to false
                    player[p].deathtimer = 0; //Reset the death timer
                }
            }

            //*Collision and Movement
            player[p].colSide = (Vector4){ 0, 0, 0, 0 }; //Resets collision detection
            for (int i = 0; i < 4; i++) //Checks collision with sides
                collision( &player[p], Menu[i], 2 );
            for (int i = 0; i < settings->level; i++) //Checks collision with enemies
                collision( &player[p], enemy[i].colRec, 2 );
            for (int i = 0; i < settings->players; i++) //Checks collision with other players
                    collision( &player[p] , player[i].colRec, 2 ); 
            for (int i = 0; i < MAPSIZE; i++) //Checks collision with terrain
                if ( terrainspace[i] == '#' ) //If the terrain is a wall
                    collision( &player[p], terrainarray[i], 2 );
            if ( !settings->pause ) //If the game is not paused
                moveplayer( &player[p], settings ); //Move the player
            //*Shooting
            if ( !settings->pause ) //If the game is not paused
                playershoot( &player[p], &bullet[p], settings ); //The player can shoot
            shooting( settings, &bullet[p], bullet, player, enemy, Menu, terrainspace, terrainarray, &textures ); //Shoot the bullet
        }

        //? ********************* ENEMIES *******************************/
        for (int k = 0; k < settings->level; k++) //Loop for every enemy possible in the level
        {
            //*HITBOX (Because cen is the center(1/2) of the image scaled, multiply by 2 to get the full size)
            enemy[k].draw = (Vector2){ enemy[k].pos.x + enemy[k].cen.x, enemy[k].pos.y + enemy[k].cen.y }; //Draw position and draw rectangle update / Sets player.draw to be player.pos + offset
            enemy[k].drawRec = (Rectangle){ enemy[k].draw.x, enemy[k].draw.y, enemy[k].cen.x*2, enemy[k].cen.y*2 }; //Rectangle resized and offset for enemy[k] drawing
            enemy[k].colRec = (Rectangle){ enemy[k].pos.x, enemy[k].pos.y, enemy[k].cen.x*2, enemy[k].cen.y*2 }; //enemy[k] collision rectangle update
            //*SPAWNING
            if ( !settings->pause && settings->enemiesremaining < settings->level ) //If the game is not paused and there are enemies remaining that should be spawned
                enemyspawn( settings, &enemy[k], terrainspace, terrainarray, player, enemy ); //Spawn an enemy
            if ( enemy[k].health > 0 ) //If the enemy is alive
                DrawTexturePro( textures.enemy, enemy[k].sourceRec, enemy[k].drawRec, enemy[k].cen, enemy[k].rot, enemy[k].color ); //Draws Enemy tank
            if (enemy[k].dying) //If dying is true
            { 
                DrawTexturePro( textures.explosionVehicles, (Rectangle){ textures.explosionVehicles.width/36 * enemy[k].deathtimer, 0, textures.explosionVehicles.width/39, textures.explosionVehicles.height }, (Rectangle){enemy[k].deathpos.x, enemy[k].deathpos.y, textures.explosionVehicles.width/180, textures.explosionVehicles.height/5 }, (Vector2){ (textures.explosionVehicles.width/180)/2, (textures.explosionVehicles.height/5)/2 }, enemy[k].rot, WHITE );
                if (!settings->pause) //If the game is not paused
                    enemy[k].deathtimer++; //Increase the death timer for animation
                if (enemy[k].deathtimer > 36) //If the death timer is greater than 36
                {
                    enemy[k].dying = false; //Set dying to false
                    enemy[k].deathtimer = 0; //Reset the death timer
                }
            }

            //*Collision and Movement
            enemy[k].colSide = (Vector4){ 0, 0, 0, 0 }; //Resets collision detection
            for (int i = 0; i < 4; i++) //Checks collision with sides
                collision( &enemy[k], Menu[i], 2 );
            for (int i = 0; i < settings->level; i++) //Checks collision with other enemies
                    collision( &enemy[k], enemy[i].colRec, 2 );
            for (int i = 0; i < settings->players; i++) //Checks collision with players
                collision( &enemy[k] , player[i].colRec, 2 ); 
            for (int i = 0; i < MAPSIZE; i++) //Checks collision with terrain
                if ( terrainspace[i] == '#' ) //If the terrain is a wall
                    collision( &enemy[k], terrainarray[i], 2 );
            if ( enemy[k].health >= 1 && !settings->pause) //If the enemy is alive and the game is not paused
                enemymove( settings, &enemy[k], player ); //Move the enemy
            //*Shooting
            if ( !GetRandomValue(0,15) && bullet[settings->players + k].ammo == true && enemy[k].health >= 1 && !settings->pause && GetTime() > bullet[settings->players + k].realTime + SHOOTINGDELAY ) //If the enemy is alive and the game is not paused and the enemy can shoot
                shoot( &enemy[k], &bullet[settings->players + k] ); //The enemy will shoot
            shooting( settings, &bullet[settings->players + k], bullet, player, enemy, Menu, terrainspace, terrainarray, &textures); //Shoot the bullet
        }

        if ( settings->pause ) //If the game is paused
            pausescreen( settings, terrainspace , player, enemy , &energy, terrainarray); //Draw the pause screen
        else //If the game is not paused
            settings->pauseselect = 0; //Reset the pause select
            
        //? ********************* GAME END *******************************/
        //*Win
        if ( score >= settings->score + 800 * settings->level ) //If the score is equal to the maximum score for that level
        {
            if (!settings->pause) //If the game is not paused
                player[0].time++; //Increase the time
            DrawText( "LEVEL COMPLETE", GetScreenWidth() / 2 - MeasureText( "LEVEL COMPLETE", 40*scaleY() )/2, GetScreenHeight()/2, 40*scaleY(), GOLD );
            if ( player[0].time == 60 * 2 ) //If the time to read has passed
            {
                PlaySound(sounds.gameend); //Play the game end sound
                settings->won = true; //Set won to true
                break; //Break out of the game loop
            }
        }
        else
            settings->won = false; //Set won to false
        //*Lose
        int deathcount = 0; //Reset the death counter
        for (int p = 0; p < settings->players; p++) //Loop for every player
            if( player[p].health <= 0 ) //If the player is dead
                deathcount++; //Increase the death counter
        if( deathcount >= settings->players ) //If all player are dead
        {
            if (!settings->pause) //If the game is not paused
                player[0].time++; //Increase the time
            DrawText( "YOU DIED", GetScreenWidth() / 2 - MeasureText( "YOU DIED", 40*scaleY() )/2, GetScreenHeight()/2, 40*scaleY(), RED );
            if ( player[0].time == 60 * 2 ) //If the time to read has passed
            {
                PlaySound(sounds.gameend); //Play the game end sound
                remove("saves/savegame.txt"); //Remove the savegame file (prevents savescumming)
                break; //Break out of the game loop
            }
        }

        EndDrawing();

        if( WindowShouldClose() ) //If the window should close
        {
            settings->select = 5; //Set the select to 5
            settings->quit = true; //Set the quit boolean to true
            settings->exitgame = true; //Set the exitgame boolean to true
        }
    }

    //? ********************** VARIABLE STORING *******************************/
    for (int p = 0; p < settings->players; p++) //Loop for every player
        settings->score += player[p].score; //Add the player's score to the total score
    memset( terrainspace, 0, sizeof(terrainspace) ); //Reset the terrain char array
    memset( terrainarray, 0, sizeof(terrainarray) ); //Reset the terrain rec array
    memset( filename, 0, sizeof(filename) ); //Reset the filename char array

    //? ********************** UNLOADING AREA *******************************/
    StopSoundMulti(); //Stops all sounds in the multichannel
    UnloadSound(sounds.gamestart);  //Unload the game start sound
    UnloadSound(sounds.gameend); //Unload the game end sound
    UnloadSound(sounds.shoot); //Unload the shoot sound
    UnloadSound(sounds.bulletmiss); //Unload the bullet miss sound
    UnloadSound(sounds.terrainhit); //Unload the terrain hit sound
    //*For solving audio buffer issues
    UnloadSound(energy.soundEffect); //Unload the energy sound effect
    UnloadSound(energy.soundEffect2); //Unload the energy sound effect2
    for (int i = 0; i < settings->players; i++) //Loop for every player
    {
        UnloadSound(player[i].soundEffect); //Unload the player sound effect
        UnloadSound(player[i].soundEffect2); //Unload the player sound effect2
    }
    for (int i = 0; i < settings->level; i++) //Loop for every enemy
    {
        UnloadSound(enemy[i].soundEffect); //Unload the enemy sound effect
        UnloadSound(enemy[i].soundEffect2); //Unload the enemy sound effect2
    }
    for (int i = 0; i < settings->players + settings->level ; i++) //Loop for every bullet
    {
        UnloadSound(bullet[i].soundEffect); //Unload the bullet sound effect
        UnloadSound(bullet[i].soundEffect2); //Sound for the player
    }
    UnloadTexture(textures.player); //Unload the player texture
    UnloadTexture(textures.enemy); //Unload the enemy texture
    UnloadTexture(textures.bullet); //Unload the bullet texture
    UnloadTexture(textures.explosion); //Unload the explosion texture
    UnloadTexture(textures.wall); //Unload the wall texture
    UnloadTexture(textures.smoke); //Unload the smoke texture
    UnloadTexture(textures.health); //Unload the health texture
    UnloadTexture(textures.energy); //Unload the energy texture
    UnloadTexture(textures.explosionVehicles); //Unload the explosion texture
}