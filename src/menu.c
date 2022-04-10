#include "menu.h"

void startscreen(Setti *settings)
{
    //? ***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false; //Booleans for animation
    settings->select = 0; //Reset selection
    double time = GetTime(); //Time for animation
    int optionsnumber = 6; //Number of options
    char options[6][100] =
    { //Option names
        "Start\0",
        "Continue\0",
        "Load\0",
        "High Scores\0",
        "Settings\0",
        "Quit\0"
    };

    //? ***************** EFFECTS *********************************/
    Textus textures;
    textures.player = LoadTexture("assets/player.png");  //Texture for the player tank
    textures.bullet = LoadTexture("assets/bullet.png"); //Texture for the bullet
    textures.explosion = LoadTexture("assets/explosionBullets.png"); //Texture for the explosion for bullets
    textures.smoke = LoadTexture("assets/fire.png"); //Smoke texture
    textures.wall = LoadTexture("assets/wall.png"); //Texture for the wall
    SFX sounds;
    sounds.shoot =  LoadSound("assets/BulletShotSFX.wav"); //Sound for the shoot
    sounds.bulletmiss = LoadSound("assets/BulletMissSFX.wav"); //Sound for the bullet miss
    SetSoundVolume(sounds.shoot, 0.02);
    SetSoundVolume(sounds.bulletmiss, 0.02);
    //*Bullets
    bool bulletdying = false; //Boolean for the bullet dying
    Vector2 bulletexplosion = {0,0}; //Vector for the bullet explosion
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0; //Timers for the bullet
    Rectangle bulletdrawRec = { 0, 0, 0, 0 }; //Rectangle for the bullet
    //*MAP ART
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    char terrainspace[MAPSIZE];   //Terrain space 
    Rectangle terrainarray[MAPSIZE];
    //*FOR FAKING LOADING A MAP
    Obj player[1], enemy[1], energy; //Objects for the fake player, enemy and energy
    if ( FileExists( "assets/startscreen.txt" ) )
        loading( "assets/startscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 1 );

    while ( !settings->quit )
    {
        //? ***************** RESIZABLE MENU BAR *****************************/
        //Rectangles for the Menu
        Rectangle Menu[4] = 
        {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50*scaleY() }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*scaleY() , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5*scaleX(), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5*scaleX() , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };

        //? ***************** PLAYER DRAWING *****************************/
        Rectangle playerdrawRec = 
        { 
            GetScreenWidth() / 2 - MeasureText(&options[settings->select][0], 20)/2*scaleX() - textures.player.width/20*scaleX(), 
            GetScreenHeight() / 4 + 50 * settings->select*scaleY() - 5*scaleY(), 
            textures.player.width*scaleY()/10 , 
            ( textures.player.height * textures.player.width / textures.player.height )*scaleY()/10 
        };

        //? ***************** MENU OPTIONS *****************************/
        if( !selected && !shoot && !bulletdying && GetTime() > time + 0.5) //If the player hasn't selected, they can switch options 
        {
            if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->select < optionsnumber - 1)
                settings->select += 1;
            if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && settings->select > 0)
                settings->select -= 1;
            if ( IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
                selected = true;
        }
        
        //? ****************** MENU ANIMATIONS ************************/
        if ( selected ) //If the player has selected
        {
            shoot = true; //Start the animation
            PlaySoundMulti(sounds.shoot); //Play the sound
            bulletdrawRec = (Rectangle){ //Set the bullet rectangle position  
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width*scaleX()/100, 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( scaleY() / 100 ), 
                    textures.bullet.width*scaleY()/50, 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height )*scaleY()/50 
                };
            selected = false; //Reset the selection
        }
        
        if ( shoot ) //If the animation should start
        {
            //*Smoke animation
            if (bulletsmoke > 20) 
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            //*Bullet animation
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + textures.bullet.width*scaleX()/25 + playerdrawRec.width / 2 + MeasureText(&options[settings->select][0], 40*scaleX())/2 ) //If the bullet has reached the end of the word
        {
            bulletexplosion.x = bulletdrawRec.x; //Set the explosion position X
            bulletexplosion.y = bulletdrawRec.y + 5; //Set the explosion position Y
            bulletdying = true; //Start the explosion animation
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 }; //Reset the bullet rectangle
            bulletdeathtimer = 0; //Reset the bullet death timer
            shoot = false; //Reset the shoot boolean
            PlaySoundMulti(sounds.bulletmiss); //Play the miss sound
        }
        if (bulletdeathtimer > 39) //If the bullet explosion animation has ended
        {
            bulletexplosion.x = GetScreenWidth()*2; //Set the explosion position X
            bulletexplosion.y = 0; //Set the explosion position Y
            bulletdying = false; //Reset the bullet dying
            bulletdeathtimer = 0; //Reset the bullet death timer
            break; //Break the loop
        }
        
        BeginDrawing();
        ClearBackground( settings->theme ); //Background color
        //* Map art
        if (IsWindowResized())
            if ( FileExists( "assets/startscreen.txt" ) )
                loading( "assets/startscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 0 ); //For the mapart to be resizable
        for (int i = 0; i < MAPSIZE; i++)
                if (terrainspace[i] == '#')
                    DrawTexturePro( textures.wall , sourceWall , terrainarray[i] , (Vector2){ 0 , 0 } , 0 , WHITE ); //Draw the wall
        //* Menu bars
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //* Texts
        DrawText( "BATTLEINF", GetScreenWidth() / 2 - MeasureText("BATTLEINF", 40)/2*scaleY(), 10*scaleY(), 40*scaleY(), LIME ); //Draw the title
        
        for (int i = 0; i < optionsnumber; i++) //Draw the options
            DrawText( &options[i][0], GetScreenWidth() / 2 - MeasureText(&options[i][0], 20)/2*scaleY(), GetScreenHeight() / 4 + 50 * i * scaleY(), 20*scaleY(), settings->lettercolor );
        
        //? *************** FOR ANIMATIONS ****************/
        //*Player
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        //*Bullet
        if ( bulletdying ) //Bullet dying animation
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        //Bullet smoke
        DrawTexturePro( textures.smoke, (Rectangle){ bulletsmoke*textures.smoke.width/20, 0, textures.smoke.width/20, textures.smoke.height }, (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height }, (Vector2){ bulletdrawRec.width/2, bulletdrawRec.height/2 }, 90-180, WHITE );
        DrawTexturePro( textures.smoke, (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20, 0, textures.smoke.width/20, textures.smoke.height }, (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width, bulletdrawRec.height }, (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }, 90-180, WHITE );
        //Bullet sprite
        DrawTexturePro( textures.bullet, (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        
        //*Error displaying
        if ( strcmp(settings->error, " ") )
        {
            DrawText(TextFormat("Error: %s", settings->error), GetScreenWidth() / 2 - MeasureText(TextFormat("Error: %s", settings->error), 25)/2*scaleY(), GetScreenHeight() - GetScreenHeight() / 4 + 400*(GetScreenHeight()*(1/655)), 25*scaleY(), RED);
            if (GetTime() > time + 2) //If the error has been displayed for 2 seconds
                strcpy(settings->error, " "); //Reset the error
        }

        EndDrawing();

        if( WindowShouldClose() ) //If the window should close
        {
            settings->select = 5; //Set the select to 5
            settings->quit = true; //Set the quit boolean to true
        }
    }
    //? ************** UNLOADING AREA ********************/
    StopSoundMulti(); //Stops all sounds in the multichannel
    UnloadTexture(textures.player);  //Unloads the player texture
    UnloadTexture(textures.bullet); //Unloads the bullet texture
    UnloadTexture(textures.explosion); //Unloads the explosion texture
    UnloadTexture(textures.smoke); //Unload the smoke texture
    UnloadTexture(textures.wall); //Unload the wall texture
    UnloadSound(sounds.shoot); //Unload the shoot sound
    UnloadSound(sounds.bulletmiss); //Unload the bullet miss sound
}

void nome(Setti *settings)
{
    //? ***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false; //Booleans for animation
    settings->select = 0; //Reset selection
    double time = GetTime(); //Time for animation
    int ranking = 6; //Ranking for the player, starts at 6
    
    FILE *highscoresread = fopen("assets/highscores.bin", "rb"); //Opens the highscores file
    char scores[15][100] = {0}; //Array for the highscores
    fread(scores, sizeof(scores), 1, highscoresread); //Reads the highscores
    fclose(highscoresread); //Closes the highscores file
    for (int i = 0; i < 5; i++) //Shuffles the highscores array until the new score is in the right place
        if (settings->score > atoi(scores[i*3]))
        {
            strcpy(scores[(i*3)], scores[((i+1)*3)]);
            strcpy(scores[(i*3)+1], scores[((i+1)*3)+1]);
            strcpy(scores[(i*3)+2], scores[((i+1)*3)+2]);
            ranking--; //Decrease the ranking
        }
    char name[9 + 1] = "\0"; //+1 for the \0
    int letterCount = 0; //Counts the letters of the name

    int optionsnumber = 4; //Number of options
    char options[4][100] = 
    {   //Option names
        "\0",
        "Digite seu nome para registrar sua Pontuação!\0",
        "\0",
        "\0"
    };
    //For dinamic options
    strcpy(options[0], TextFormat("Seu Ranking: %i\0", ranking));
    strcpy(options[2], TextFormat("Seu Score: %i\0", settings->score));
    strcpy(options[3], TextFormat("Sua maior fase: %i\0", settings->level));

    /***************** EFFECTS *********************************/
    Textus textures;
    textures.player = LoadTexture("assets/player.png");  //Texture for the player tank
    textures.bullet = LoadTexture("assets/bullet.png"); //Texture for the bullet
    textures.explosion = LoadTexture("assets/explosionBullets.png"); //Texture for the explosion for bullets
    textures.smoke = LoadTexture("assets/fire.png"); //Smoke texture
    textures.wall = LoadTexture("assets/wall.png"); //Texture for the wall
    SFX sounds;
    sounds.shoot =  LoadSound("assets/BulletShotSFX.wav"); //Sound for the shoot
    sounds.bulletmiss = LoadSound("assets/BulletMissSFX.wav"); //Sound for the bullet miss
    SetSoundVolume(sounds.shoot, 0.02);
    SetSoundVolume(sounds.bulletmiss, 0.02);
    //*Bullets
    bool bulletdying = false;
    Vector2 bulletexplosion = {0,0};
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0;
    Rectangle bulletdrawRec = { 0, 0, 0, 0 };
    //*MAP ART
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    char terrainspace[MAPSIZE];   //Terrain space 
    Rectangle terrainarray[MAPSIZE];
    //*FOR FAKING LOADING A MAP
    Obj player[1], enemy[1], energy;
    if ( FileExists( "assets/namescreen.txt" ) )
        loading( "assets/namescreen", settings, player, enemy, &energy, terrainarray, terrainspace, 1 );

    while ( !settings->quit )
    {
        //? ***************** TEXT BAR *****************************/
        int key = GetCharPressed();
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (letterCount < 9))
            {
                name[letterCount] = (char)key;
                name[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                letterCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            name[letterCount] = '\0';
        }

        //? ***************** RESIZABLE MENU BAR *****************************/
        //Rectangles for the Menu
        Rectangle Menu[4] = 
        {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50*scaleY() }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*scaleY() , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5*scaleX(), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5*scaleX() , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };

        /***************** PLAYER DRAWING *****************************/
        Rectangle playerdrawRec = 
        { 
            GetScreenWidth() / 2 - MeasureText("Next", 20)/2 *scaleX() - textures.player.width/20 * scaleX(), 
            (GetScreenHeight() - GetScreenHeight() / 5) - 5*scaleY(), 
            textures.player.width*scaleY()/10, 
            ( textures.player.height * textures.player.width / textures.player.height )*scaleY()/10
        };

        //? ***************** MENU OPTIONS *****************************/
        if( !selected && !shoot && !bulletdying && GetTime() > time + 0.5) //If the player hasn't pressed enter
            if ( (IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12)) && letterCount > 0 )
                selected = true;
    
        //? ****************** MENU ANIMATIONS ************************/
        if ( selected ) //If the player has selected
        {
            shoot = true; //Start the animation
            PlaySoundMulti(sounds.shoot); //Play the shoot sound
            bulletdrawRec = (Rectangle){  //Set the bullet rectangle position  
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width*scaleX()/100 , 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height )*scaleY()/100, 
                    textures.bullet.width*scaleY()/50, 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height )*scaleY()/50 
                };
            selected = false; //Reset the selection
        }
        
        if ( shoot ) //If the animation should start
        {
            //*Smoke animation
            if (bulletsmoke > 20) 
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            //*Bullet animation
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + textures.bullet.width*scaleX()/25 + playerdrawRec.width / 2 + MeasureText("Next", GetFontDefault().baseSize)*scaleX()*2 ) //If the bullet has reached the end of the word
        {
            bulletexplosion.x = bulletdrawRec.x; //Set the explosion position X
            bulletexplosion.y = bulletdrawRec.y + 5; //Set the explosion position Y
            bulletdying = true; //Start the explosion animation
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 }; //Reset the bullet rectangle
            bulletdeathtimer = 0; //Reset the bullet death timer
            shoot = false; //Reset the shoot boolean
            PlaySoundMulti(sounds.bulletmiss); //Play the miss sound
        }
        if (bulletdeathtimer > 39) //If the bullet explosion animation has ended
        {
            bulletexplosion.x = GetScreenWidth()*2; //Set the explosion position X
            bulletexplosion.y = 0; //Set the explosion position Y
            bulletdying = false; //Reset the bullet dying
            bulletdeathtimer = 0; //Reset the bullet death timer
            break; //Break the loop
        }
        
        BeginDrawing();

        ClearBackground( settings->theme );
        //* Map art
        if (IsWindowResized())
            if (FileExists("assets/namescreen.txt"))
                loading("assets/namescreen", settings, player, enemy, &energy, terrainarray, terrainspace, 0 ); //For the mapart to be resizable
        for (int i = 0; i < MAPSIZE; i++)
                if (terrainspace[i] == '#')
                    DrawTexturePro( textures.wall , sourceWall , terrainarray[i] , (Vector2){ 0 , 0 } , 0 , WHITE );
        //* Menu bars
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //* Texts
        DrawText( "BATTLEINF", GetScreenWidth() / 2 - MeasureText("BATTLEINF", 40)/2*scaleY(), 10*scaleY(), 40*scaleY(), LIME ); //Draw the title
        
        for (int i = 0; i < optionsnumber; i++)
            DrawText( &options[i][0], GetScreenWidth() / 2 - MeasureText(&options[i][0], 25)/2*scaleY(), GetScreenHeight() / 8 + 50 * i * scaleY(), 25*scaleY(), GOLD );

        DrawText(name, GetScreenWidth()/2 - MeasureText(name, 40)/2, GetScreenHeight() / 8 + 50 * 5*scaleY(), 40*scaleY(), PURPLE);

        if (letterCount < 9)
            DrawText("_", GetScreenWidth()/2 + 8 * scaleX() + MeasureText(name, 40)/2,  GetScreenHeight() / 8 + 50 * 5 * scaleY(), 40*scaleY(), PURPLE);
        else
            strcpy(settings->error, "Numero Maximo de Caracteres");

        DrawText("Next", GetScreenWidth() / 2 - MeasureText("Next", 20)/2 * scaleY(), (GetScreenHeight() - GetScreenHeight() / 5), 20 * scaleY(), settings->lettercolor);
        
        //? *************** FOR ANIMATIONS ****************/
        //*Player
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        //*Bullet
        if ( bulletdying ) //Bullet dying animation
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        //Bullet smoke
        DrawTexturePro( textures.smoke, (Rectangle){ bulletsmoke*textures.smoke.width/20, 0, textures.smoke.width/20, textures.smoke.height }, (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height }, (Vector2){ bulletdrawRec.width/2, bulletdrawRec.height/2 }, 90-180, WHITE );
        DrawTexturePro( textures.smoke, (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20, 0, textures.smoke.width/20, textures.smoke.height }, (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width, bulletdrawRec.height }, (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }, 90-180, WHITE );
        //Bullet sprite
        DrawTexturePro( textures.bullet, (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        
        //*Error displaying
        if ( strcmp(settings->error, " ") )
        {
            DrawText(TextFormat("%s", settings->error), GetScreenWidth() / 2 - MeasureText(TextFormat("%s", settings->error), 15)/2*scaleY(), GetScreenHeight() - GetScreenHeight() / 3 + 100*(GetScreenHeight()*(1/655)), 15*scaleY(), RED);
            if (GetTime() > time + 2) //If the error has been displayed for 2 seconds
                strcpy(settings->error, " "); //Reset the error
        }

        EndDrawing();

        if( WindowShouldClose() ) //If the window should close
        {
            settings->select = 5; //Set the select to 5
            settings->quit = true; //Set the quit boolean to true
        }
    }
    //? ************** UNLOADING AREA ********************/
    if (!settings->quit) //If the game is not exited
    {
        int id = 5 - ranking; //The id of the player in the ranking
        FILE *highscoreswrite = fopen("assets/highscores.bin", "wb"); //Open the highscores file
        sprintf(scores[id*3], "%d", settings->score); //Set the score of the player in the ranking
        strcpy(scores[id*3+1], name); //Set the name of the player in the ranking
        sprintf(scores[id*3+2], "%d", settings->level); //Set the level of the player in the ranking
        fwrite(scores, sizeof(scores), 1, highscoreswrite); //Write the highscores in the file
        settings->lowscore = atoi(scores[0]); //Update the lowest score
        fclose(highscoreswrite); //Close the highscores file
    }

    StopSoundMulti(); //Stops all sounds in the multichannel
    UnloadTexture(textures.player);  //Unloads the player texture
    UnloadTexture(textures.bullet); //Unloads the bullet texture
    UnloadTexture(textures.explosion); //Unloads the explosion texture
    UnloadTexture(textures.smoke); //Unload the smoke texture
    UnloadTexture(textures.wall); //Unload the wall texture
    UnloadSound(sounds.shoot); //Unload the shoot sound
    UnloadSound(sounds.bulletmiss); //Unload the bullet miss sound
}

void settingscreen(Setti *settings)
{
    //? ***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false, submenu = false; //Booleans for animation
    settings->select = 0; //Reset selection
    double time = GetTime(), time2 = 0, error = 0; //Time for animation
    int optionsnumber = 5, subselect = 0; //Number of options
    char options[5][100] = 
    { //Option names
        "Resolution\0",
        "Number of Players\0",
        "Theme\0",
        "Extended Play\0",
        "Back\0"
    };
    char resolutions[5][100] = 
    { //Resolutions
        "1010x655\0",
        "800x450\0",
        "1920x1080\0",
        "1280x720\0",
        "Fullscreen\0"
    };

    //? ***************** EFFECTS *********************************/
    Textus textures;
    textures.player = LoadTexture("assets/player.png");  //Texture for the player tank
    textures.bullet = LoadTexture("assets/bullet.png"); //Texture for the bullet
    textures.explosion = LoadTexture("assets/explosionBullets.png"); //Texture for the explosion for bullets
    textures.smoke = LoadTexture("assets/fire.png"); //Smoke texture
    textures.wall = LoadTexture("assets/wall.png"); //Texture for the wall
    textures.energy = LoadTexture("assets/controller.png"); //Texture for the extended play
    SFX sounds;
    sounds.shoot =  LoadSound("assets/BulletShotSFX.wav"); //Sound for the shoot
    sounds.bulletmiss = LoadSound("assets/BulletMissSFX.wav"); //Sound for the bullet miss
    SetSoundVolume(sounds.shoot, 0.02);
    SetSoundVolume(sounds.bulletmiss, 0.02);
    //*Bullets
    bool bulletdying = false; //Boolean for the bullet dying
    Vector2 bulletexplosion = {0,0}; //Vector for the bullet explosion
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0; //Timers for the bullet
    Rectangle bulletdrawRec = { 0, 0, 0, 0 }; //Rectangle for the bullet
    //*MAP ART
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    char terrainspace[MAPSIZE];   //Terrain space 
    Rectangle terrainarray[MAPSIZE];
    //*FOR FAKING LOADING A MAP
    Obj player[1], enemy[1], energy; //Objects for the fake player, enemy and energy
    if (FileExists( "assets/settingsscreen.txt" ))
        loading( "assets/settingsscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 1 );

    while ( !settings->quit )
    {
        //? ***************** RESIZABLE MENU BAR *****************************/
        //Rectangles for the Menu
        Rectangle Menu[4] = 
        {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50*scaleY() }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*scaleY() , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5*scaleX(), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5*scaleX() , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };

        //? ***************** PLAYER DRAWING *****************************/
        Rectangle playerdrawRec = 
        { 
            GetScreenWidth() / 2 - MeasureText(&options[settings->select][0], 20)/2*scaleX() - textures.player.width/20*scaleX(), 
            GetScreenHeight() / 4 + 50 * settings->select*scaleY() - 5*scaleY(), 
            textures.player.width*scaleY()/10 , 
            ( textures.player.height * textures.player.width / textures.player.height )*scaleY()/10 
        };

        //? ***************** MENU OPTIONS *****************************/
        if( !selected && !shoot && !bulletdying && GetTime() > time + 0.5)  //If the player hasn't selected, they can switch options 
        {
            if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && !submenu && settings->select < optionsnumber - 1)
                settings->select += 1;
            if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && !submenu && settings->select > 0)
                settings->select -= 1;
            if ( IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
            {   
                if (settings->select == 4) //If the player selects back
                    selected = true; //The player has selected
                else if (!submenu) //If the player selected a submenu
                {
                    submenu = true; //The player has selected a submenu
                    subselect = 0; //Reset the submenu selection
                    time2 = GetTime(); //Reset the time2
                }
            }
        }
        //? ****************** MENU ANIMATIONS ************************/
        if ( selected ) //If the player has selected
        {
            shoot = true; //Start the animation
            PlaySoundMulti(sounds.shoot); //Play the sound
            bulletdrawRec = (Rectangle){ //Set the bullet rectangle position  
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width*scaleX()/100, 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( scaleY() / 100 ), 
                    textures.bullet.width*scaleY()/50, 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height )*scaleY()/50 
                };
            selected = false; //Reset the selection
        }
        
        if ( shoot ) //If the animation should start
        {
            //*Smoke animation
            if (bulletsmoke > 20) 
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            //*Bullet animation
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + textures.bullet.width*scaleX()/25 + playerdrawRec.width / 2 + MeasureText(&options[settings->select][0], 20*scaleX()) ) //If the bullet has reached the end of the word
        {
            bulletexplosion.x = bulletdrawRec.x; //Set the explosion position X
            bulletexplosion.y = bulletdrawRec.y + 5; //Set the explosion position Y
            bulletdying = true; //Start the explosion animation
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 }; //Reset the bullet rectangle
            bulletdeathtimer = 0; //Reset the bullet death timer
            shoot = false; //Reset the shoot boolean
            PlaySoundMulti(sounds.bulletmiss); //Play the miss sound
        }
        if (bulletdeathtimer > 39) //If the bullet explosion animation has ended
        {
            bulletexplosion.x = GetScreenWidth()*2; //Set the explosion position X
            bulletexplosion.y = 0; //Set the explosion position Y
            bulletdying = false; //Reset the bullet dying
            bulletdeathtimer = 0; //Reset the bullet death timer
            break; //Break the loop
        }
        
        BeginDrawing();
        ClearBackground( settings->theme ); //Background color
        //* Map art
        if (IsWindowResized())
            if (FileExists( "assets/settingsscreen.txt" ))
                loading( "assets/settingsscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 0 );
        for (int i = 0; i < MAPSIZE; i++)
                if (terrainspace[i] == '#')
                    DrawTexturePro( textures.wall , sourceWall , terrainarray[i] , (Vector2){ 0 , 0 } , 0 , WHITE );
        //* Menu bars
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //* Texts
        DrawText( "SETTINGS", GetScreenWidth() / 2 - MeasureText("SETTINGS", 40)/2 * scaleY(), 10*scaleY(), 40*scaleY(), LIME );
        
        for (int i = 0; i < optionsnumber; i++)
            DrawText( &options[i][0], GetScreenWidth() / 2 - MeasureText(&options[i][0], 20)/2*scaleY(), GetScreenHeight() / 4 + 50 * i * scaleY(), 20*scaleY(), settings->lettercolor );
        
        if (submenu) //if submenu is true
        {
            if (settings->select == 0) //See which submenu to display (Switch didn't work here for some reason)
            {
                if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && subselect < optionsnumber - 1)
                    subselect += 1;
                if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && subselect > 0)
                    subselect -= 1;
                for (int i = 0; i < 5; i++)
                {
                    if (i == subselect) //if the subselect is the same as the submenu item draw it highlighted
                        DrawText( &resolutions[i][0], GetScreenWidth() - GetScreenWidth() / 3 - MeasureText(&resolutions[i][0], 20)/2* scaleX(), GetScreenHeight() / 4 + 50 * i * scaleY(), 20*scaleY(), GOLD );
                    else
                        DrawText( &resolutions[i][0], GetScreenWidth() - GetScreenWidth() / 3 - MeasureText(&resolutions[i][0], 20)/2* scaleX(), GetScreenHeight() / 4 + 50 * i * scaleY(), 20*scaleY(), settings->lettercolor );
                }
                if ( (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12)) && GetTime() > time2 + 0.5)
                {
                    if (IsWindowFullscreen())
                        ToggleFullscreen();
                    switch (subselect)
                    {
                    case 0:
                        SetWindowSize( 1010, 655 );
                        if (FileExists( "assets/settingsscreen.txt" ))
                            loading( "assets/settingsscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 0 );
                        break;
                    case 1:
                        SetWindowSize( 800, 450 );
                        if (FileExists( "assets/settingsscreen.txt" ))
                            loading( "assets/settingsscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 0 );
                        break;
                    case 2:
                        SetWindowSize( 1920, 1080 );
                        if (FileExists( "assets/settingsscreen.txt" ))
                            loading( "assets/settingsscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 0 );
                        break;
                    case 3:
                        SetWindowSize( 1280, 720 );
                        if (FileExists( "assets/settingsscreen.txt" ))
                            loading( "assets/settingsscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 0 );
                        break;
                    case 4:
                        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()),GetMonitorHeight(GetCurrentMonitor()));
                        ToggleFullscreen();
                        if (FileExists( "assets/settingsscreen.txt" ))
                            loading( "assets/settingsscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 0 );
                        break;
                    }
                    submenu = false; //Reset the submenu
                }
            }
            if (settings->select == 1) //Changes the number of players
            {
                if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->players < 2 + settings->extended)
                    settings->players++;
                if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && settings->players > 1 )
                    settings->players--;
                if (( IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsGamepadButtonPressed(0, 3)  ) && settings->players == 2 + settings->extended && settings->players != 5)
                {
                    strcpy(settings->error, TextFormat("Can't have more than %d players, increase Extended Play", settings->players));
                    error = GetTime();
                }
                Color playercolor[5] = { WHITE, RED, GREEN, ORANGE, MAGENTA }; //Different colors for each player
                for (int i = 0; i < settings->players; i++)
                    DrawTextureEx(textures.player, (Vector2){ GetScreenWidth() - GetScreenWidth() / 3, GetScreenHeight() / 4 + 50 * i * scaleY()}, 0, 0.1, playercolor[i]);
                
                if ( (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12)) && GetTime() > time2 + 0.5)
                    submenu = false; //Reset the submenu
            }
            if (settings->select == 2) //Changes the theme of the game
            {
                if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && subselect < 1)
                    subselect += 1;
                if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && subselect > 0)
                    subselect -= 1;
                if ( subselect == 0 )
                    DrawTextureEx(textures.player, (Vector2){ GetScreenWidth() - GetScreenWidth() / 3, GetScreenHeight() / 4 + 50 * 2 * scaleY()}, 0, 0.2, DARKPURPLE);               
                else 
                    DrawTextureEx(textures.player, (Vector2){ GetScreenWidth() - GetScreenWidth() / 3, GetScreenHeight() / 4 + 50 * 2 * scaleY()}, 0, 0.2, GOLD);
                if ( (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12)) && GetTime() > time2 + 0.5)
                {
                    switch (subselect)
                    {
                    case 0:
                        settings->theme = BLACK;
                        settings->lettercolor = RAYWHITE;
                        break;
                    case 1:
                        settings->theme = RAYWHITE;
                        settings->lettercolor = BLACK;
                        break;
                    }
                    submenu = false; //Reset the submenu
                }
            }
            if (settings->select == 3) //Activates the extended play option
            {
                if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->extended < 3)
                    settings->extended++;
                if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && settings->extended > 0)
                    settings->extended--;
                if( !settings->extended )
                    DrawText( "Extended Play Disabled", GetScreenWidth() - GetScreenWidth() / 4 - MeasureText("Extended Play Disabled", 20)/2* scaleX(), GetScreenHeight() / 4 , 20*scaleY(), settings->lettercolor );
                for (int i = 0; i < settings->extended; i++)
                    DrawTextureEx(textures.energy, (Vector2){ GetScreenWidth() - GetScreenWidth() / 3, GetScreenHeight() / 4 + 50 * i * scaleY()}, 0, 0.1, WHITE);
                if ( (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12)) && GetTime() > time2 + 0.5)
                    submenu = false; //Reset the submenu
            }
        }
        
        //? *************** FOR ANIMATIONS ****************/
        //*Player
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        //*Bullet
        if ( bulletdying ) //Bullet dying animation
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        //Bullet smoke
        DrawTexturePro( textures.smoke, (Rectangle){ bulletsmoke*textures.smoke.width/20, 0, textures.smoke.width/20, textures.smoke.height }, (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height }, (Vector2){ bulletdrawRec.width/2, bulletdrawRec.height/2 }, 90-180, WHITE );
        DrawTexturePro( textures.smoke, (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20, 0, textures.smoke.width/20, textures.smoke.height }, (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width, bulletdrawRec.height }, (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }, 90-180, WHITE );
        //Bullet sprite
        DrawTexturePro( textures.bullet, (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        
        //*Error displaying
        if ( strcmp(settings->error, " ") )
        {
            DrawText(TextFormat("Error: %s", settings->error), GetScreenWidth() / 2 - MeasureText(TextFormat("Error: %s", settings->error), 25)/2 * scaleY(), GetScreenHeight() - GetScreenHeight() / 4 + 400*(GetScreenHeight()*(1/655)), 25*scaleY(), RED);
            if (GetTime() > error + 2) //If the error has been displayed for 2 seconds
                strcpy(settings->error, " "); //Reset the error
        }
        
        EndDrawing();

        if( WindowShouldClose() ) //If the window should close
        {
            settings->select = 5; //Set the select to 5
            settings->quit = true; //Set the quit boolean to true
        }
    }
    //? ************** UNLOADING AREA ********************/
    StopSoundMulti(); //Stops all sounds in the multichannel
    UnloadTexture(textures.player);  //Unloads the player texture
    UnloadTexture(textures.bullet); //Unloads the bullet texture
    UnloadTexture(textures.explosion); //Unloads the explosion texture
    UnloadTexture(textures.smoke); //Unload the smoke texture
    UnloadTexture(textures.wall); //Unload the wall texture
    UnloadSound(sounds.shoot); //Unload the shoot sound
    UnloadSound(sounds.bulletmiss); //Unload the bullet miss sound
}

void pausescreen(Setti *settings, char terrainspace[], Obj player[], Obj enemy[] , Obj *energy, Rectangle terrainarray[] )
{
    //? *************** NAVEGATION ****************/
    if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->pauseselect < 3 )
        settings->pauseselect += 1;
    if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W) || IsGamepadButtonReleased(0, 1)  ) && settings->pauseselect > 0)
        settings->pauseselect -= 1;
    if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
    {
        switch (settings->pauseselect) //Switch for the option selected
        {
        case 0: //Resumes the game
            settings->pause = false;
            break;
        case 1: //Saves the game and exits
            saving(settings, terrainspace, player, enemy, energy, terrainarray);
            settings->exitgame = true;
            settings->pause = false;
            break;
        case 2: //Exits to the main menu
            remove("saves/savegame.txt");
            settings->exitgame = true;
            settings->pause = false;
            break;
        case 3: //Quits the game
            remove("saves/savegame.txt");
            settings->quit = true;
            settings->exitgame = true;
            settings->pause = false;
            break;
        }
    }

    char pauseoptions[4][100] = 
    {
        "Resume\0",
        "Save and Exit\0",
        "Main Menu\0",
        "Quit Game\0"
    };
    DrawText("Paused", GetScreenWidth() / 2 - MeasureText("Paused", GetFontDefault().baseSize) * 2 * scaleX(), GetScreenHeight() / 4, 40*scaleY(), GOLD);
    for (int i = 0; i < 4; i++)
    {
        if (i == settings->pauseselect)
            DrawText( &pauseoptions[i][0], GetScreenWidth() / 2 - MeasureText(&pauseoptions[i][0], 20*scaleY())/2, GetScreenHeight() / 4 + 100*scaleY() + 50 * i * scaleY(), 20*scaleY(), GOLD );
        else                
            DrawText( &pauseoptions[i][0], GetScreenWidth() / 2 - MeasureText(&pauseoptions[i][0], 20*scaleY())/2, GetScreenHeight() / 4 + 100*scaleY() + 50 * i * scaleY(), 20*scaleY(), settings->lettercolor );  
    }
}

void highscorescreen(Setti *settings)
{
   //? ***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false; //Booleans for animation
    settings->select = 0; //Reset selection
    double time = GetTime(); //Time for animation
    int displaynumber = 6; //Display Number
    char options[4][100] = 
    { //Option names
        "Rº\0",
        "Score\0",
        "Name\0",
        "Level\0"
    };

    FILE *highscoresdisplay = fopen("assets/highscores.bin", "rb"); //Opens the highscores file
    char scoresdisplay[15][100] = {0}; //Array for the highscores
    fread(scoresdisplay, sizeof(scoresdisplay), 1, highscoresdisplay); //Reads the highscores
    fclose(highscoresdisplay); //Closes the highscores file

    //? ***************** EFFECTS *********************************/
    Textus textures;
    textures.player = LoadTexture("assets/player.png");  //Texture for the player tank
    textures.bullet = LoadTexture("assets/bullet.png"); //Texture for the bullet
    textures.explosion = LoadTexture("assets/explosionBullets.png"); //Texture for the explosion for bullets
    textures.smoke = LoadTexture("assets/fire.png"); //Smoke texture
    textures.wall = LoadTexture("assets/wall.png"); //Texture for the wall
    SFX sounds;
    sounds.shoot =  LoadSound("assets/BulletShotSFX.wav"); //Sound for the shoot
    sounds.bulletmiss = LoadSound("assets/BulletMissSFX.wav"); //Sound for the bullet miss
    SetSoundVolume(sounds.shoot, 0.02);
    SetSoundVolume(sounds.bulletmiss, 0.02);
    //*Bullets
    bool bulletdying = false; //Boolean for the bullet dying
    Vector2 bulletexplosion = {0,0}; //Vector for the bullet explosion
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0; //Timers for the bullet
    Rectangle bulletdrawRec = { 0, 0, 0, 0 }; //Rectangle for the bullet
    //*MAP ART
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    char terrainspace[MAPSIZE];   //Terrain space 
    Rectangle terrainarray[MAPSIZE];
    //*FOR FAKING LOADING A MAP
    Obj player[1], enemy[1], energy; //Objects for the fake player, enemy and energy
    if ( FileExists( "assets/highscorescreen.txt" ) )
        loading( "assets/highscorescreen", settings, player, enemy, &energy, terrainarray, terrainspace, 1 );

    while ( !settings->quit )
    {
        //? ***************** RESIZABLE MENU BAR *****************************/
        //Rectangles for the Menu
        Rectangle Menu[4] = 
        {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50*scaleY() }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*scaleY() , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5*scaleX(), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5*scaleX() , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };

         //? ***************** PLAYER DRAWING *****************************/
        Rectangle playerdrawRec = 
        { 
            GetScreenWidth() / 2 - MeasureText("Back", 20)/2 *scaleX() - textures.player.width/20*scaleX(), 
            (GetScreenHeight() - GetScreenHeight() / 5) - 5*scaleY(), 
            textures.player.width*scaleY() / 10, 
            ( textures.player.height * textures.player.width / textures.player.height )*scaleY()/10 
        };

        //? ***************** MENU OPTIONS *****************************/
        if( !selected && !shoot && !bulletdying && GetTime() > time + 0.5) //If the player hasn't selected, they can switch options 
            if ( IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
                selected = true; //The player has selected
        
        //? ****************** MENU ANIMATIONS ************************/
        if ( selected ) //If the player has selected
        {
            shoot = true; //Start the animation
            PlaySoundMulti(sounds.shoot); //Play the sound
            bulletdrawRec = (Rectangle){ //Set the bullet rectangle position  
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width*scaleX()/100, 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( scaleY() / 100 ), 
                    textures.bullet.width*scaleY()/50, 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height )*scaleY()/50 
                };
            selected = false; //Reset the selection
        }
        
        if ( shoot ) //If the animation should start
        {
            //*Smoke animation
            if (bulletsmoke > 20) 
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            //*Bullet animation
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + textures.bullet.width * ( scaleX() / 25 ) + playerdrawRec.width / 2 + MeasureText("Back", 25*scaleY()) )
        {
            bulletexplosion.x = bulletdrawRec.x; //Set the explosion position X
            bulletexplosion.y = bulletdrawRec.y + 5; //Set the explosion position Y
            bulletdying = true; //Start the explosion animation
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 }; //Reset the bullet rectangle
            bulletdeathtimer = 0; //Reset the bullet death timer
            shoot = false; //Reset the shoot boolean
            PlaySoundMulti(sounds.bulletmiss); //Play the miss sound
        }
        if (bulletdeathtimer > 39) //If the bullet explosion animation has ended
        {
            bulletexplosion.x = GetScreenWidth()*2; //Set the explosion position X
            bulletexplosion.y = 0; //Set the explosion position Y
            bulletdying = false; //Reset the bullet dying
            bulletdeathtimer = 0; //Reset the bullet death timer
            break; //Break the loop
        }
        
        BeginDrawing();
        ClearBackground( settings->theme ); //Background color
        //* Map art
        if (IsWindowResized())
            if ( FileExists( "assets/highscorescreen.txt" ) )
                loading( "assets/highscorescreen", settings, player, enemy, &energy, terrainarray, terrainspace, 0 );
        for (int i = 0; i < 15 * 41; i++)
                if (terrainspace[i] == '#')
                    DrawTexturePro( textures.wall, sourceWall, terrainarray[i], (Vector2){ 0 , 0 }, 0, WHITE );
        //* Menu bars
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //* Texts
        DrawText( "HIGHSCORES", GetScreenWidth() / 2 - (MeasureText("HIGHSCORES", 40)*scaleY())/2, 10*scaleY(), 40*scaleY(), LIME ); //Draw the title
        
        for (int i = 0; i < displaynumber; i++)
        {
            if ( i == 0 ) //The first line is the titles for each row
            {
                DrawText(options[0], (GetScreenWidth() / 5 ) - MeasureText(options[0], 25)/2 * scaleY(), GetScreenHeight() / 4  + i * 50 * scaleY(), 25 * scaleY(), GOLD);
                DrawText(options[1], (GetScreenWidth() / 5 *2) - MeasureText(options[1], 25)/2 * scaleY(), GetScreenHeight() / 4 + i * 50 * scaleY(), 25 * scaleY(), GREEN);
                DrawText(options[2], (GetScreenWidth() / 5 *3) - MeasureText(options[2], 25)/2 * scaleY(), GetScreenHeight() / 4 + i * 50 * scaleY(), 25 * scaleY(), VIOLET);
                DrawText(options[3], (GetScreenWidth() / 5 *4) - MeasureText(options[3], 25)/2 * scaleY(), GetScreenHeight() / 4 + i * 50 * scaleY(), 25 * scaleY(), BLUE);
            }
            else //After that, display the highscores in decreasing order
            {
                DrawText(TextFormat("%d", i), (GetScreenWidth() / 5 ) - MeasureText(TextFormat("%d", i), 25)/2 * scaleY(), GetScreenHeight() / 4  + i * 50 * scaleY(), 25 * scaleY(), settings->lettercolor);
                DrawText(scoresdisplay[12 - ((i-1)*3)], (GetScreenWidth() / 5 *2) - MeasureText(scoresdisplay[12 - ((i-1)*3)], 25)/2 * scaleY(), GetScreenHeight() / 4  + i * 50 * scaleY(), 25 * scaleY(), settings->lettercolor);
                DrawText(scoresdisplay[13 - ((i-1)*3)], (GetScreenWidth() / 5 *3) - MeasureText(scoresdisplay[13 - ((i-1)*3)], 25)/2 * scaleY(), GetScreenHeight() / 4  + i * 50 * scaleY(), 25 * scaleY(), settings->lettercolor);
                DrawText(scoresdisplay[14 - ((i-1)*3)], (GetScreenWidth() / 5 *4) - MeasureText(scoresdisplay[14 - ((i-1)*3)], 25)/2 * scaleY(), GetScreenHeight() / 4  + i * 50 * scaleY(), 25 * scaleY(), settings->lettercolor);
            }
        }
        DrawText("Back", GetScreenWidth() / 2 - MeasureText("Back", 20)/2 * scaleY(), (GetScreenHeight() - GetScreenHeight() / 5), 20 * scaleY(), settings->lettercolor);
        
        //? *************** FOR ANIMATIONS ****************/
        //*Player
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        //*Bullet
        if ( bulletdying ) //Bullet dying animation
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        //Bullet smoke
        DrawTexturePro( textures.smoke, (Rectangle){ bulletsmoke*textures.smoke.width/20, 0, textures.smoke.width/20, textures.smoke.height }, (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height }, (Vector2){ bulletdrawRec.width/2, bulletdrawRec.height/2 }, 90-180, WHITE );
        DrawTexturePro( textures.smoke, (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20, 0, textures.smoke.width/20, textures.smoke.height }, (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width, bulletdrawRec.height }, (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }, 90-180, WHITE );
        //Bullet sprite
        DrawTexturePro( textures.bullet, (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        
        EndDrawing();

        if( WindowShouldClose() ) //If the window should close
        {
            settings->select = 5; //Set the select to 5
            settings->quit = true; //Set the quit boolean to true
        }
    }
    //? ************** UNLOADING AREA ********************/
    StopSoundMulti(); //Stops all sounds in the multichannel
    UnloadTexture(textures.player);  //Unloads the player texture
    UnloadTexture(textures.bullet); //Unloads the bullet texture
    UnloadTexture(textures.explosion); //Unloads the explosion texture
    UnloadTexture(textures.smoke); //Unload the smoke texture
    UnloadTexture(textures.wall); //Unload the wall texture
    UnloadSound(sounds.shoot); //Unload the shoot sound
    UnloadSound(sounds.bulletmiss); //Unload the bullet miss sound
}

void loadscreen(Setti *settings)
{
    //? ***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false; //Booleans for animation
    settings->select = 0; //Reset selection
    double time = GetTime(), error = 0; //Time for animation
    char name[20] = {0}; //Name for the file
    int letterCount = 0; //Letter count for the filename
    settings->filenamefound = false; //Reset the filename found
    int optionsnumber = 2; //Number of options
    char options[2][100] = 
    { //Option names
        "Next\0",
        "Back\0"
    };

    //? ***************** EFFECTS *********************************/
    Textus textures;
    textures.player = LoadTexture("assets/player.png");  //Texture for the player tank
    textures.bullet = LoadTexture("assets/bullet.png"); //Texture for the bullet
    textures.explosion = LoadTexture("assets/explosionBullets.png"); //Texture for the explosion for bullets
    textures.smoke = LoadTexture("assets/fire.png"); //Smoke texture
    textures.wall = LoadTexture("assets/wall.png"); //Texture for the wall
    SFX sounds;
    sounds.shoot =  LoadSound("assets/BulletShotSFX.wav"); //Sound for the shoot
    sounds.bulletmiss = LoadSound("assets/BulletMissSFX.wav"); //Sound for the bullet miss
    SetSoundVolume(sounds.shoot, 0.02);
    SetSoundVolume(sounds.bulletmiss, 0.02);
    //*Bullets
    bool bulletdying = false; //Boolean for the bullet dying
    Vector2 bulletexplosion = {0,0}; //Vector for the bullet explosion
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0; //Timers for the bullet
    Rectangle bulletdrawRec = { 0, 0, 0, 0 }; //Rectangle for the bullet
    //*MAP ART
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    char terrainspace[MAPSIZE];   //Terrain space 
    Rectangle terrainarray[MAPSIZE];
    //*FOR FAKING LOADING A MAP
    Obj player[1], enemy[1], energy; //Objects for the fake player, enemy and energy
    if ( FileExists( "assets/loadscreen.txt" ) )
        loading( "assets/loadscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 1 );

    while ( !settings->quit )
    {
        //? ***************** TEXT BAR *****************************/
        int key = GetCharPressed();
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (letterCount < 20))
            {
                name[letterCount] = (char)key;
                letterCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }
        if (IsKeyReleased(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            name[letterCount] = 0; // Add nothing at the end of the string.
        }

        //? ***************** RESIZABLE MENU BAR *****************************/
        //Rectangles for the Menu
        Rectangle Menu[4] = 
        {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50*scaleY() }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*scaleY() , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5*scaleX(), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5*scaleX() , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };

        //? ***************** PLAYER DRAWING *****************************/
        Rectangle playerdrawRec = 
        { 
            GetScreenWidth() / 2 - MeasureText(&options[settings->select][0], 20)/2*scaleX() - textures.player.width/20*scaleX(), 
            (GetScreenHeight() - GetScreenHeight() / 5) + 50 * settings->select * scaleY(),
            textures.player.width*scaleY()/10 , 
            ( textures.player.height * textures.player.width / textures.player.height )*scaleY()/10 
        };

        //? ***************** MENU OPTIONS *****************************/
        if( !selected && !shoot && !bulletdying && GetTime() > time + 0.5) //If the player hasn't selected, they can switch options 
        {
            if (( IsKeyReleased(KEY_DOWN) || IsGamepadButtonReleased(0, 3)  ) && settings->select < optionsnumber - 1)
                settings->select += 1;
            if (( IsKeyReleased(KEY_UP) || IsGamepadButtonReleased(0, 1)  ) && settings->select > 0)
                settings->select -= 1;
            if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
                selected = true;
        }
        
        //? ****************** MENU ANIMATIONS ************************/
        if ( selected ) //If the player has selected
        {
            shoot = true; //Start the animation
            PlaySoundMulti(sounds.shoot); //Play the sound
            bulletdrawRec = (Rectangle){ //Set the bullet rectangle position  
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width*scaleX()/100, 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( scaleY() / 100 ), 
                    textures.bullet.width*scaleY()/50, 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height )*scaleY()/50 
                };
            selected = false; //Reset the selection
        }
        
        if ( shoot ) //If the animation should start
        {
            //*Smoke animation
            if (bulletsmoke > 20) 
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            //*Bullet animation
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + textures.bullet.width*scaleX()/25 + playerdrawRec.width / 2 + MeasureText(&options[settings->select][0], 40*scaleX())/2 ) //If the bullet has reached the end of the word
        {
            bulletexplosion.x = bulletdrawRec.x; //Set the explosion position X
            bulletexplosion.y = bulletdrawRec.y + 5; //Set the explosion position Y
            bulletdying = true; //Start the explosion animation
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 }; //Reset the bullet rectangle
            bulletdeathtimer = 0; //Reset the bullet death timer
            shoot = false; //Reset the shoot boolean
            PlaySoundMulti(sounds.bulletmiss); //Play the miss sound
        }
        if (bulletdeathtimer > 39)
        {
            bulletexplosion.x = GetScreenWidth()*2; //Set the explosion position X
            bulletexplosion.y = 0; //Set the explosion position Y
            bulletdying = false; //Reset the bullet dying
            bulletdeathtimer = 0; //Reset the bullet death timer
            if (settings->select == 0) //If the selected is next
            {
                if(FileExists(TextFormat("saves/%s.txt",name))) //Checks if the file exists
                {
                    strcpy(settings->loadgamename, name); //Copy the name to the settings
                    settings->filenamefound = true; //Set the filename found boolean
                    break; //Break the loop
                }
                else
                {
                    strcpy(settings->error, "Arquivo nao existe\0"); //Copy the error message to the settings
                    error = GetTime(); //Set the error timer
                }
            }
            else
                break; //Break the loop
        }
        
        BeginDrawing();
        ClearBackground( settings->theme ); //Background color
        //* Map art
        if (IsWindowResized())
            if ( FileExists( "assets/loadscreen.txt" ) )
                loading( "assets/loadscreen", settings, player, enemy, &energy, terrainarray, terrainspace, 0 );
        for (int i = 0; i < MAPSIZE; i++)
                if (terrainspace[i] == '#')
                    DrawTexturePro( textures.wall , sourceWall , terrainarray[i] , (Vector2){ 0 , 0 } , 0 , WHITE );
        //* Menu bars
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //* Texts
        DrawText( "LOADGAME", GetScreenWidth() / 2 - MeasureText("LOADGAME", 40)/2 * scaleY(), 10*scaleY(), 40*scaleY(), LIME );
        
        DrawText( "Digite o nome do arquivo que quer carregar:", GetScreenWidth() / 2 - MeasureText("Digite o nome do arquivo que quer carregar:", 25)/2*scaleY(), GetScreenHeight() / 8 + 50 * 2 * scaleY(), 25*scaleY(), GOLD );
        
        DrawText(name, GetScreenWidth()/2 - MeasureText(name, 40)/2, GetScreenHeight() / 8 + 50 * 5 * scaleY(), 40*scaleY(), settings->lettercolor);
        
        if (letterCount < 20)
        {
            DrawText("_", GetScreenWidth()/2 + 8 * scaleX() + MeasureText(name, 40)/2,  GetScreenHeight() / 8 + 50 * 5 * scaleY(), 40*scaleY(), settings->lettercolor);
        }
        else
            strcpy(settings->error, "Numero Maximo de Caracteres");
        
        for (int i = 0; i < optionsnumber; i++)
            DrawText( &options[i][0], GetScreenWidth() / 2 - MeasureText(&options[i][0], 25)/2*scaleY(), (GetScreenHeight() - GetScreenHeight() / 5) + 50 * i * scaleY(), 25*scaleY(), settings->lettercolor );

        //? *************** FOR ANIMATIONS ****************/
        //*Player
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        //*Bullet
        if ( bulletdying ) //Bullet dying animation
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        //Bullet smoke
        DrawTexturePro( textures.smoke, (Rectangle){ bulletsmoke*textures.smoke.width/20, 0, textures.smoke.width/20, textures.smoke.height }, (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height }, (Vector2){ bulletdrawRec.width/2, bulletdrawRec.height/2 }, 90-180, WHITE );
        DrawTexturePro( textures.smoke, (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20, 0, textures.smoke.width/20, textures.smoke.height }, (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width, bulletdrawRec.height }, (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }, 90-180, WHITE );
        //Bullet sprite
        DrawTexturePro( textures.bullet, (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        
        //*Error displaying
        if ( strcmp(settings->error, " ") )
        {
            DrawText(TextFormat("%s", settings->error), GetScreenWidth() / 2 - MeasureText(TextFormat("%s", settings->error), 15)/2*scaleY(), GetScreenHeight() - GetScreenHeight() / 3 + 100*(GetScreenHeight()*(1/655)), 15*scaleY(), RED);
            if (GetTime() > error + 2) //If the error has been displayed for 2 seconds
                strcpy(settings->error, " "); //Reset the error
        }

        EndDrawing();

        if( WindowShouldClose() ) //If the window should close
        {
            settings->select = 5; //Set the select to 5
            settings->quit = true; //Set the quit boolean to true
        }
    }
    //? ************** UNLOADING AREA ********************/

    StopSoundMulti(); //Stops all sounds in the multichannel
    UnloadTexture(textures.player);  //Unloads the player texture
    UnloadTexture(textures.bullet); //Unloads the bullet texture
    UnloadTexture(textures.explosion); //Unloads the explosion texture
    UnloadTexture(textures.smoke); //Unload the smoke texture
    UnloadTexture(textures.wall); //Unload the wall texture
    UnloadSound(sounds.shoot); //Unload the shoot sound
    UnloadSound(sounds.bulletmiss); //Unload the bullet miss sound
}