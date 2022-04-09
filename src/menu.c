#include "raylib.h"
#include "core.h"
#include <string.h>
#include "saveload.h"


void startscreen(Setti *settings)
{
    /***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false;
    settings->select = 0;
    double time = GetTime();
    
    int optionsnumber = 6;
    char options[6][100] = {
        "Start\0",
        "Continue\0",
        "Load\0",
        "High Scores\0",
        "Settings\0",
        "Quit\0"
    };
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
    //!Bullets
    bool bulletdying = false;
    Vector2 bulletexplosion = {0,0};
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0;
    Rectangle bulletdrawRec = { 0, 0, 0, 0 };
    //!MAP ART
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    char terrainspacestar[ 15 * 41 ];   //15x41 terrain space 
    Rectangle terrainarraystar[ 15 * 41 ];
    //!FOR FAKING LOADING A MAP
    Obj playerstar[1], enemystar[1], energystar;
    loading( "assets/startscreen", settings, playerstar, enemystar, &energystar, terrainarraystar, terrainspacestar, 1 );

    while ( !settings->quit )
    {
        /***************** RESIZABLE MENU BAR *****************************/
        Rectangle Menu[4] = {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50 * (GetScreenHeight()*RATIOY) }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*(GetScreenHeight()*RATIOY) , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5 * (GetScreenWidth()*RATIOX), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5 * (GetScreenWidth()*RATIOX) , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };

        /***************** PLAYER DRAWING *****************************/
        Rectangle playerdrawRec = { 
            GetScreenWidth() / 2 - MeasureText(&options[settings->select][0], 20)/2 * (GetScreenWidth()*RATIOX) - textures.player.width/20 * (GetScreenWidth()*RATIOX), 
            GetScreenHeight() / 4 + 50 * settings->select * (GetScreenHeight()*RATIOY) - 5*(GetScreenHeight()*RATIOY), 
            textures.player.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ), 
            ( textures.player.height * textures.player.width / textures.player.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ) 
        };

        /***************** MENU OPTIONS *****************************/
        if( !selected && !shoot && !bulletdying && GetTime() > time + 0.5)
        {
            if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->select < optionsnumber - 1)
                settings->select += 1;
            if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && settings->select > 0)
                settings->select -= 1;
            if ( IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
                selected = true;
        }
        /****************** MENU ANIMATIONS ************************/
        if ( selected )
        {
            shoot = true;
            PlaySoundMulti(sounds.shoot);
            bulletdrawRec = (Rectangle){ 
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width * ( ( GetScreenWidth() * ( 1.0 / 1010 ) ) / 100 ), 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 100 ), 
                    textures.bullet.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ), 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ) 
                };
            selected = false;
        }
        
        if ( shoot )
        {
            if (bulletsmoke > 20)
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + textures.bullet.width * ( ( GetScreenWidth() * ( 1.0 / 1010 ) ) / 25 ) + playerdrawRec.width / 2 + MeasureText(&options[settings->select][0], GetFontDefault().baseSize) * (GetScreenWidth()*RATIOX) * 2 )
        {
            bulletexplosion.x = bulletdrawRec.x;
            bulletexplosion.y = bulletdrawRec.y + 5;
            bulletdying = true;
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 };
            bulletdeathtimer = 0;
            shoot = false;
            PlaySoundMulti(sounds.bulletmiss);
        }
        if (bulletdeathtimer > 39)
        {
            bulletexplosion.x = GetScreenWidth()*2;
            bulletexplosion.y = 0;
            bulletdying = false;
            bulletdeathtimer = 0;
            break;
        }
        
        BeginDrawing();

        ClearBackground( settings->theme );
        //* Map art
        if (IsWindowResized())
            loading( "assets/startscreen", settings, playerstar, enemystar, &energystar, terrainarraystar, terrainspacestar, 0 );
        for (int i = 0; i < 15 * 41; i++)
                if (terrainspacestar[i] == '#')
                    DrawTexturePro( textures.wall , sourceWall , terrainarraystar[i] , (Vector2){ 0 , 0 } , 0 , WHITE );
        //* Menu bars
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //* Texts
        DrawText( "BATTLEINF", GetScreenWidth() / 2 - MeasureText("BATTLEINF", 40)/2 * (GetScreenHeight()*RATIOY) , 10*(GetScreenHeight()*RATIOY) , 40*(GetScreenHeight()*RATIOY) , LIME );
        
        for (int i = 0; i < optionsnumber; i++)
            DrawText( &options[i][0], GetScreenWidth() / 2 - MeasureText(&options[i][0], 20)/2*(GetScreenHeight()*RATIOY), GetScreenHeight() / 4 + 50 * i * (GetScreenHeight()*RATIOY), 20*(GetScreenHeight()*RATIOY), settings->lettercolor );
        
        /*************** FOR ANIMATIONS ****************/
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        if ( bulletdying )
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        //This was a bug that made the smoke spin but I kept it as a feature becaue it looked nice doubled
        DrawTexturePro( textures.smoke , (Rectangle){ bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.smoke , (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.bullet , (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        
        //*Error displaying
        if ( strcmp(settings->error, " ") )
        {
            DrawText(TextFormat("Error: %s", settings->error), GetScreenWidth() / 2 - MeasureText(TextFormat("Error: %s", settings->error), 25)/2*(GetScreenHeight()*RATIOY), GetScreenHeight() - GetScreenHeight() / 4 + 400*(GetScreenHeight()*(1/655)), 25*(GetScreenHeight()*RATIOY), RED);
            if (GetTime() > time + 2)
                strcpy(settings->error, " ");
        }

        EndDrawing();

        if( WindowShouldClose() )
        {
            settings->select = 5;
            settings->quit = true;
        }
    }
    /************** UNLOADING AREA ********************/
    StopSoundMulti();
    UnloadTexture(textures.player);  //Texture for the player tank
    UnloadTexture(textures.bullet); //Texture for the bullet
    UnloadTexture(textures.explosion); //Texture for the explosion for bullets
    UnloadTexture(textures.smoke); //Smoke texture
    UnloadTexture(textures.wall); //Texture for the wall
    UnloadSound(sounds.shoot); //Sound for shooting
    UnloadSound(sounds.bulletmiss); //Sound for explosion
}

void nome(Setti *settings)
{
     /***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false;
    settings->select = 0;
    double time = GetTime();
    int ranking = 6;
    
    FILE *highscoresread = fopen("assets/highscores.bin", "rb");
    char scores[15][100] = {0};
    fread(scores, sizeof(scores), 1, highscoresread);
    fclose(highscoresread);
    for (int i = 0; i < 5; i++)
        if (settings->score > atoi(scores[i*3]))
        {
            strcpy(scores[(i*3)], scores[((i+1)*3)]);
            strcpy(scores[(i*3)+1], scores[((i+1)*3)+1]);
            strcpy(scores[(i*3)+2], scores[((i+1)*3)+2]);
            ranking--;
        }
    char name[9 + 1] = "\0"; //+1 de espaço para o \0
    int letterCount = 0;

    int optionsnumbernome = 4;
    char optionsnome[4][100] = {
        "\0",
        "Digite seu nome para registrar sua Pontuação!\0",
        "\0",
        "\0"
    };
    strcpy(optionsnome[0], TextFormat("Seu Ranking: %i\0", ranking));
    strcpy(optionsnome[2], TextFormat("Seu Score: %i\0", settings->score));
    strcpy(optionsnome[3], TextFormat("Sua maior fase: %i\0", settings->level));

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
    //!Bullets
    bool bulletdying = false;
    Vector2 bulletexplosion = {0,0};
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0;
    Rectangle bulletdrawRec = { 0, 0, 0, 0 };
    //!MAP ART
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    char terrainspacestar[ 15 * 41 ];   //15x41 terrain space 
    Rectangle terrainarraystar[ 15 * 41 ];
    //!FOR FAKING LOADING A MAP
    Obj playerstar[1], enemystar[1], energystar;
    loading( "assets/namescreen", settings, playerstar, enemystar, &energystar, terrainarraystar, terrainspacestar, 1 );

    while ( !settings->quit )
    {
        /***************** TEXT BAR *****************************/
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
        /***************** RESIZABLE MENU BAR *****************************/
        Rectangle Menu[4] = {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50 * (GetScreenHeight()*RATIOY) }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*(GetScreenHeight()*RATIOY) , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5 * (GetScreenWidth()*RATIOX), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5 * (GetScreenWidth()*RATIOX) , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };

        /***************** PLAYER DRAWING *****************************/
        Rectangle playerdrawRec = { 
            GetScreenWidth() / 2 - MeasureText("Next", 20)/2 *(GetScreenWidth()*RATIOX) - textures.player.width/20 * (GetScreenWidth()*RATIOX), 
            (GetScreenHeight() - GetScreenHeight() / 5) - 5*(GetScreenHeight()*RATIOY), 
            textures.player.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ), 
            ( textures.player.height * textures.player.width / textures.player.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ) 
        };

        /***************** MENU OPTIONS *****************************/
        if( !selected && !shoot && !bulletdying && GetTime() > time + 0.5)
        {
            if ( (IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12)) && letterCount > 0 )
                selected = true;
        }
        /****************** MENU ANIMATIONS ************************/
        if ( selected )
        {
            shoot = true;
            PlaySoundMulti(sounds.shoot);
            bulletdrawRec = (Rectangle){ 
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width * ( ( GetScreenWidth() * ( 1.0 / 1010 ) ) / 100 ), 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 100 ), 
                    textures.bullet.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ), 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ) 
                };
            selected = false;
        }
        
        if ( shoot )
        {
            if (bulletsmoke > 20)
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + textures.bullet.width * ( ( GetScreenWidth() * ( 1.0 / 1010 ) ) / 25 ) + playerdrawRec.width / 2 + MeasureText("Next", GetFontDefault().baseSize) * (GetScreenWidth()*RATIOX) * 2 )
        {
            bulletexplosion.x = bulletdrawRec.x;
            bulletexplosion.y = bulletdrawRec.y + 5;
            bulletdying = true;
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 };
            bulletdeathtimer = 0;
            shoot = false;
            PlaySoundMulti(sounds.bulletmiss);
        }
        if (bulletdeathtimer > 39)
        {
            bulletexplosion.x = GetScreenWidth()*2;
            bulletexplosion.y = 0;
            bulletdying = false;
            bulletdeathtimer = 0;
            break;
        }
        
        BeginDrawing();

        ClearBackground( settings->theme );
        //* Map art
        if (IsWindowResized())
            loading( "assets/namescreen", settings, playerstar, enemystar, &energystar, terrainarraystar, terrainspacestar, 0 );
        for (int i = 0; i < 15 * 41; i++)
                if (terrainspacestar[i] == '#')
                    DrawTexturePro( textures.wall , sourceWall , terrainarraystar[i] , (Vector2){ 0 , 0 } , 0 , WHITE );
        //* Menu bars
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //* Texts
        DrawText( "BATTLEINF", GetScreenWidth() / 2 - MeasureText("BATTLEINF", 40)/2 * (GetScreenHeight()*RATIOY) , 10*(GetScreenHeight()*RATIOY) , 40*(GetScreenHeight()*RATIOY) , LIME );
        
        for (int i = 0; i < optionsnumbernome; i++)
            DrawText( &optionsnome[i][0], GetScreenWidth() / 2 - MeasureText(&optionsnome[i][0], 25)/2*(GetScreenHeight()*RATIOY), GetScreenHeight() / 8 + 50 * i * (GetScreenHeight()*RATIOY), 25*(GetScreenHeight()*RATIOY), GOLD );

        DrawText(name, GetScreenWidth()/2 - MeasureText(name, 40)/2, GetScreenHeight() / 8 + 50 * 5 * (GetScreenHeight()*RATIOY), 40*(GetScreenHeight()*RATIOY), PURPLE);

        if (letterCount < 9)
        {
            DrawText("_", GetScreenWidth()/2 + 8 * (GetScreenWidth()*RATIOX) + MeasureText(name, 40)/2,  GetScreenHeight() / 8 + 50 * 5 * (GetScreenHeight()*RATIOY), 40*(GetScreenHeight()*RATIOY), PURPLE);
        }
        else
            strcpy(settings->error, "Numero Maximo de Caracteres");

        DrawText("Next", GetScreenWidth() / 2 - MeasureText("Next", 20)/2 * (GetScreenHeight()*RATIOY), (GetScreenHeight() - GetScreenHeight() / 5), 20 * (GetScreenHeight()*RATIOY), settings->lettercolor);
        /*************** FOR ANIMATIONS ****************/
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        if ( bulletdying )
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        //This was a bug that made the smoke spin but I kept it as a feature becaue it looked nice doubled
        DrawTexturePro( textures.smoke , (Rectangle){ bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.smoke , (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.bullet , (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        
        //*Error displaying
        if ( strcmp(settings->error, " ") )
        {
            DrawText(TextFormat("%s", settings->error), GetScreenWidth() / 2 - MeasureText(TextFormat("%s", settings->error), 15)/2*(GetScreenHeight()*RATIOY), GetScreenHeight() - GetScreenHeight() / 3 + 100*(GetScreenHeight()*(1/655)), 15*(GetScreenHeight()*RATIOY), RED);
            if (GetTime() > time + 2)
                strcpy(settings->error, " ");
        }

        EndDrawing();

        if( WindowShouldClose() )
        {
            settings->select = 5;
            settings->quit = true;
        }
    }
    /************** UNLOADING AREA ********************/
    if (!settings->quit)
    {
        int id = 5 - ranking;
        FILE *highscoreswrite = fopen("assets/highscores.bin", "wb");
        sprintf(scores[id*3], "%d", settings->score);
        strcpy(scores[id*3+1], name);
        sprintf(scores[id*3+2], "%d", settings->level);
        fwrite(scores, sizeof(scores), 1, highscoreswrite);
        fclose(highscoreswrite);
    }

    StopSoundMulti();
    UnloadTexture(textures.player);  //Texture for the player tank
    UnloadTexture(textures.bullet); //Texture for the bullet
    UnloadTexture(textures.explosion); //Texture for the explosion for bullets
    UnloadTexture(textures.smoke); //Smoke texture
    UnloadTexture(textures.wall); //Texture for the wall
    UnloadSound(sounds.shoot); //Sound for shooting
    UnloadSound(sounds.bulletmiss); //Sound for explosion
}

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}

void settingscreen(Setti *settings)
{
    /***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false, submenu = false;
    settings->select = 0;
    double time = GetTime(), time2 = 0, error = 0;
    
    int optionsnumber = 5, subselect = 0;
    char optionssettings[5][100] = {
        "Resolution\0",
        "Number of Players\0",
        "Theme\0",
        "Extended Play\0",
        "Back\0"
    };
    char resolutions[5][100] = {
        "1010x655\0",
        "800x450\0",
        "1920x1080\0",
        "1280x720\0",
        "Fullscreen\0"
    };
    /***************** EFFECTS *********************************/
    Textus textures;
    textures.player = LoadTexture("assets/player.png");  //Texture for the player tank
    textures.bullet = LoadTexture("assets/bullet.png"); //Texture for the bullet
    textures.explosion = LoadTexture("assets/explosionBullets.png"); //Texture for the explosion for bullets
    textures.smoke = LoadTexture("assets/fire.png"); //Smoke texture
    textures.wall = LoadTexture("assets/wall.png"); //Texture for the wall
    textures.energy = LoadTexture("assets/controller.png");
    SFX sounds;
    sounds.shoot =  LoadSound("assets/BulletShotSFX.wav"); //Sound for the shoot
    sounds.bulletmiss = LoadSound("assets/BulletMissSFX.wav"); //Sound for the bullet miss
    SetSoundVolume(sounds.shoot, 0.02);
    SetSoundVolume(sounds.bulletmiss, 0.02);
    //!Bullets
    bool bulletdying = false;
    Vector2 bulletexplosion = {0,0};
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0;
    Rectangle bulletdrawRec = { 0, 0, 0, 0 };
    //!MAP ART
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    char terrainspaceset[ 15 * 41 ];   //15x41 terrain space 
    Rectangle terrainarrayset[ 15 * 41 ];
    //!FOR FAKING LOADING A MAP
    Obj playerset[1], enemyset[1], energyset;
    loading( "assets/settingsscreen", settings, playerset, enemyset, &energyset, terrainarrayset, terrainspaceset, 1 );

    while ( !settings->quit )
    {
        /***************** RESIZABLE MENU BAR *****************************/
        Rectangle Menu[4] = {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50 * (GetScreenHeight()*RATIOY) }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*(GetScreenHeight()*RATIOY) , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5 * (GetScreenWidth()*RATIOX), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5 * (GetScreenWidth()*RATIOX) , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };

        /***************** PLAYER DRAWING *****************************/
        Rectangle playerdrawRec = { 
            GetScreenWidth() / 2 - MeasureText(&optionssettings[settings->select][0], 20)/2 * (GetScreenWidth()*RATIOX) - textures.player.width/20 * (GetScreenWidth()*RATIOX), 
            GetScreenHeight() / 4 + 50 * settings->select * (GetScreenHeight()*RATIOY) - 5*(GetScreenHeight()*RATIOY), 
            textures.player.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ), 
            ( textures.player.height * textures.player.width / textures.player.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ) 
        };

        /***************** MENU OPTIONS *****************************/
        if( !selected && !shoot && !bulletdying && GetTime() > time + 0.5)
        {
            if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && !submenu && settings->select < optionsnumber - 1)
                settings->select += 1;
            if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && !submenu && settings->select > 0)
                settings->select -= 1;
            if ( IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
            {   
                if (settings->select == 4)
                    selected = true;
                else if (!submenu)
                {
                    submenu = true;
                    subselect = 0;
                    time2 = GetTime();
                }
            }
        }
        /****************** MENU ANIMATIONS ************************/
        if ( selected )
        {
            shoot = true;
            PlaySoundMulti(sounds.shoot);
            bulletdrawRec = (Rectangle){ 
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width * ( ( GetScreenWidth() * ( 1.0 / 1010 ) ) / 100 ), 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 100 ), 
                    textures.bullet.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ), 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ) 
                };
            selected = false;
        }
        
        if ( shoot )
        {
            if (bulletsmoke > 20)
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + textures.bullet.width * ( ( GetScreenWidth() * ( 1.0 / 1010 ) ) / 25 ) + playerdrawRec.width / 2 + MeasureText(&optionssettings[settings->select][0], 20)/2 * (GetScreenWidth()*RATIOX) * 2 )
        {
            bulletexplosion.x = bulletdrawRec.x;
            bulletexplosion.y = bulletdrawRec.y + 5;
            bulletdying = true;
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 };
            bulletdeathtimer = 0;
            shoot = false;
            PlaySoundMulti(sounds.bulletmiss);
        }
        if (bulletdeathtimer > 39)
        {
            bulletexplosion.x = GetScreenWidth()*2;
            bulletexplosion.y = 0;
            bulletdying = false;
            bulletdeathtimer = 0;
            break;
        }
        
        BeginDrawing();

        ClearBackground( settings->theme );
        //* Map art
        if (IsWindowResized())
            loading( "assets/settingsscreen", settings, playerset, enemyset, &energyset, terrainarrayset, terrainspaceset, 0 );
        for (int i = 0; i < 15 * 41; i++)
                if (terrainspaceset[i] == '#')
                    DrawTexturePro( textures.wall , sourceWall , terrainarrayset[i] , (Vector2){ 0 , 0 } , 0 , WHITE );
        //* Menu bars
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //* Texts
        DrawText( "SETTINGS", GetScreenWidth() / 2 - MeasureText("SETTINGS", 40)/2 * (GetScreenHeight()*RATIOY) , 10*(GetScreenHeight()*RATIOY) , 40*(GetScreenHeight()*RATIOY) , LIME );
        
        for (int i = 0; i < optionsnumber; i++)
            DrawText( &optionssettings[i][0], GetScreenWidth() / 2 - MeasureText(&optionssettings[i][0], 20)/2*(GetScreenHeight()*RATIOY), GetScreenHeight() / 4 + 50 * i * (GetScreenHeight()*RATIOY), 20*(GetScreenHeight()*RATIOY), settings->lettercolor );
        
        if (submenu)
        {
            if (settings->select == 0)
            {
                if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && subselect < optionsnumber - 1)
                    subselect += 1;
                if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && subselect > 0)
                    subselect -= 1;
                for (int i = 0; i < 5; i++)
                {
                    if (i == subselect)
                        DrawText( &resolutions[i][0], GetScreenWidth() - GetScreenWidth() / 3 - MeasureText(&resolutions[i][0], 20)/2* (GetScreenWidth()*RATIOX), GetScreenHeight() / 4 + 50 * i * (GetScreenHeight()*RATIOY), 20*(GetScreenHeight()*RATIOY), GOLD );
                    else                
                        DrawText( &resolutions[i][0], GetScreenWidth() - GetScreenWidth() / 3 - MeasureText(&resolutions[i][0], 20)/2* (GetScreenWidth()*RATIOX), GetScreenHeight() / 4 + 50 * i * (GetScreenHeight()*RATIOY), 20*(GetScreenHeight()*RATIOY), settings->lettercolor );
                }
                if ( (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12)) && GetTime() > time2 + 0.5)
                {
                    if (IsWindowFullscreen())
                        ToggleFullscreen();
                    switch (subselect)
                    {
                    case 0:
                        SetWindowSize( 1010, 655 );
                        break;
                    case 1:
                        SetWindowSize( 800, 450 );
                        break;
                    case 2:
                        SetWindowSize( 1920, 1080 );
                        break;
                    case 3:
                        SetWindowSize( 1280, 720 );
                        break;
                    case 4:
                        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()),GetMonitorHeight(GetCurrentMonitor()));
                        ToggleFullscreen();
                        break;
                    }
                    submenu = false;
                }
            }
            if (settings->select == 1)
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
                Color playercolor[5] = { WHITE, RED, GREEN, ORANGE, MAGENTA };
                for (int i = 0; i < settings->players; i++)
                    DrawTextureEx(textures.player, (Vector2){ GetScreenWidth() - GetScreenWidth() / 3, GetScreenHeight() / 4 + 50 * i * (GetScreenHeight()*RATIOY)}, 0, 0.1, playercolor[i]);
                
                if ( (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12)) && GetTime() > time2 + 0.5)
                    submenu = false;
            }
            if (settings->select == 2)
            {
                if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && subselect < 1)
                    subselect += 1;
                if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && subselect > 0)
                    subselect -= 1;
                if ( subselect == 0 )
                    DrawTextureEx(textures.player, (Vector2){ GetScreenWidth() - GetScreenWidth() / 3, GetScreenHeight() / 4 + 50 * 2 * (GetScreenHeight()*RATIOY)}, 0, 0.2, DARKPURPLE);               
                else 
                    DrawTextureEx(textures.player, (Vector2){ GetScreenWidth() - GetScreenWidth() / 3, GetScreenHeight() / 4 + 50 * 2 * (GetScreenHeight()*RATIOY)}, 0, 0.2, GOLD);
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
                    submenu = false;
                }
            }
            if (settings->select == 3)
            {
                if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->extended < 3)
                    settings->extended++;
                if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)|| IsGamepadButtonReleased(0, 1)  ) && settings->extended > 0)
                    settings->extended--;
                if( !settings->extended )
                    DrawText( "Extended Play Disabled", GetScreenWidth() - GetScreenWidth() / 4 - MeasureText("Extended Play Disabled", 20)/2* (GetScreenWidth()*RATIOX), GetScreenHeight() / 4 , 20*(GetScreenHeight()*RATIOY), settings->lettercolor );
                for (int i = 0; i < settings->extended; i++)
                    DrawTextureEx(textures.energy, (Vector2){ GetScreenWidth() - GetScreenWidth() / 3, GetScreenHeight() / 4 + 50 * i * (GetScreenHeight()*RATIOY)}, 0, 0.1, WHITE);
                if ( (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12)) && GetTime() > time2 + 0.5)
                    submenu = false;
            }
        }
        
        /*************** FOR ANIMATIONS ****************/
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        if ( bulletdying )
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        //This was a bug that made the smoke spin but I kept it as a feature becaue it looked nice doubled
        DrawTexturePro( textures.smoke , (Rectangle){ bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.smoke , (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.bullet , (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );

        //*Error displaying
        if ( strcmp(settings->error, " ") )
        {
            DrawText(TextFormat("Error: %s", settings->error), GetScreenWidth() / 2 - MeasureText(TextFormat("Error: %s", settings->error), 25)/2 * (GetScreenHeight()*RATIOY), GetScreenHeight() - GetScreenHeight() / 4 + 400*(GetScreenHeight()*(1/655)), 25*(GetScreenHeight()*RATIOY), RED);
            if (GetTime() > error + 2)
                strcpy(settings->error, " ");
        }
        
        EndDrawing();

        if( WindowShouldClose() )
        {
            settings->select = 5;
            settings->quit = true;
        }
    }
    /************** UNLOADING AREA ********************/
    StopSoundMulti();
    UnloadTexture(textures.player);  //Texture for the player tank
    UnloadTexture(textures.bullet); //Texture for the bullet
    UnloadTexture(textures.explosion); //Texture for the explosion for bullets
    UnloadTexture(textures.smoke); //Smoke texture
    UnloadTexture(textures.wall); //Texture for the wall
    UnloadTexture(textures.energy); //Texture for the energy
    UnloadSound(sounds.shoot); //Sound for shooting
    UnloadSound(sounds.bulletmiss); //Sound for explosion
}

void pausescreen(Setti *settings, char terrainspace[], Obj player[], Obj enemy[] , Obj *energy, Obj bullet[], Rectangle terrainarray[] )
{

    if (( IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_S) || IsGamepadButtonReleased(0, 3)  ) && settings->pauseselect < 3 )
        settings->pauseselect += 1;
    if (( IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W) || IsGamepadButtonReleased(0, 1)  ) && settings->pauseselect > 0)
        settings->pauseselect -= 1;
    if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
    {
        switch (settings->pauseselect)
        {
        case 0:
            settings->pause = false;
            break;
        case 1:
            saving(settings, terrainspace, player, enemy, energy, bullet, terrainarray);
            settings->exitgame = true;
            settings->pause = false;
            break;
        case 2:
            remove("saves/savegame.txt");
            settings->exitgame = true;
            settings->pause = false;
            break;
        case 3:
            remove("saves/savegame.txt");
            settings->quit = true;
            settings->exitgame = true;
            settings->pause = false;
            break;
        }
    }

    char pauseoptions[4][100] = {
        "Resume\0",
        "Save and Exit\0",
        "Main Menu\0",
        "Quit Game\0"
    };
    DrawText("Paused", GetScreenWidth() / 2 - MeasureText("Paused", GetFontDefault().baseSize) * 2 * (GetScreenWidth()*RATIOX), GetScreenHeight() / 4, 40*(GetScreenHeight()*RATIOY), GOLD);
    for (int i = 0; i < 4; i++)
    {
        if (i == settings->pauseselect)
            DrawText( &pauseoptions[i][0], GetScreenWidth() / 2 - MeasureText(&pauseoptions[i][0], GetFontDefault().baseSize)* (GetScreenWidth()*RATIOX), GetScreenHeight() / 4 + 100*(GetScreenHeight()*RATIOY) + 50 * i * (GetScreenHeight()*RATIOY), 20*(GetScreenHeight()*RATIOY), GOLD );
        else                
            DrawText( &pauseoptions[i][0], GetScreenWidth() / 2 - MeasureText(&pauseoptions[i][0], GetFontDefault().baseSize)* (GetScreenWidth()*RATIOX), GetScreenHeight() / 4 + 100*(GetScreenHeight()*RATIOY) + 50 * i * (GetScreenHeight()*RATIOY), 20*(GetScreenHeight()*RATIOY), settings->lettercolor );  
    }
}

void highscorescreen(Setti *settings)
{
    /***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false;
    settings->select = 4;
    double time3 = GetTime();
    
    int displaynumber = 6;
    char optionshighscores[4][100] = {
        "Rº\0",
        "Score\0",
        "Name\0",
        "Level\0"
    };
    FILE *highscoresdisplay = fopen("assets/highscores.bin", "rb");
    char scoresdisplay[15][100] = {0};
    fread(scoresdisplay, sizeof(scoresdisplay), 1, highscoresdisplay);
    fclose(highscoresdisplay);

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
    //!Bullets
    bool bulletdying = false;
    Vector2 bulletexplosion = {0,0};
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0;
    Rectangle bulletdrawRec = { 0, 0, 0, 0 };
    //!MAP ART
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    char terrainspacesco[ 15 * 41 ];   //15x41 terrain space 
    Rectangle terrainarraysco[ 15 * 41 ];
    //!FOR FAKING LOADING A MAP
    Obj playersco[1], enemysco[1], energysco;
    loading( "assets/highscorescreen", settings, playersco, enemysco, &energysco, terrainarraysco, terrainspacesco, 1 );

    while ( !settings->quit )
    {
        /***************** RESIZABLE MENU BAR *****************************/
        Rectangle Menu[4] = {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50 * (GetScreenHeight()*RATIOY) }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*(GetScreenHeight()*RATIOY) , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5 * (GetScreenWidth()*RATIOX), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5 * (GetScreenWidth()*RATIOX) , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };

        /***************** PLAYER DRAWING *****************************/
        Rectangle playerdrawRec = { 
            GetScreenWidth() / 2 - MeasureText("Back", 20)/2 *(GetScreenWidth()*RATIOX) - textures.player.width/20 * (GetScreenWidth()*RATIOX), 
            (GetScreenHeight() - GetScreenHeight() / 5) - 5*(GetScreenHeight()*RATIOY), 
            textures.player.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ), 
            ( textures.player.height * textures.player.width / textures.player.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ) 
        };

        /***************** MENU OPTIONS *****************************/
        if( !selected && !shoot && !bulletdying && GetTime() > time3 + 0.5)
        {
            if ( IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
                selected = true;
        }
        /****************** MENU ANIMATIONS ************************/
        if ( selected )
        {
            shoot = true;
            PlaySoundMulti(sounds.shoot);
            bulletdrawRec = (Rectangle){ 
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width * ( ( GetScreenWidth() * ( 1.0 / 1010 ) ) / 100 ), 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 100 ), 
                    textures.bullet.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ), 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ) 
                };
            selected = false;
        }
        
        if ( shoot )
        {
            if (bulletsmoke > 20)
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + textures.bullet.width * ( ( GetScreenWidth() * ( 1.0 / 1010 ) ) / 25 ) + playerdrawRec.width / 2 + MeasureText("Back", GetFontDefault().baseSize) * 1.25 * (GetScreenWidth()*RATIOX) * 2 )
        {
            bulletexplosion.x = bulletdrawRec.x;
            bulletexplosion.y = bulletdrawRec.y + 5;
            bulletdying = true;
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 };
            bulletdeathtimer = 0;
            shoot = false;
            PlaySoundMulti(sounds.bulletmiss);
        }
        if (bulletdeathtimer > 39)
        {
            bulletexplosion.x = GetScreenWidth()*2;
            bulletexplosion.y = 0;
            bulletdying = false;
            bulletdeathtimer = 0;
            break;
        }
        
        BeginDrawing();

        ClearBackground( settings->theme );
        //* Map art
        if (IsWindowResized())
            loading( "assets/highscorescreen", settings, playersco, enemysco, &energysco, terrainarraysco, terrainspacesco, 0 );
        for (int i = 0; i < 15 * 41; i++)
                if (terrainspacesco[i] == '#')
                    DrawTexturePro( textures.wall , sourceWall , terrainarraysco[i] , (Vector2){ 0 , 0 } , 0 , WHITE );
        //* Menu bars
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //* Texts
        DrawText( "HIGHSCORES", GetScreenWidth() / 2 - (MeasureText("HIGHSCORES", 40) * (GetScreenHeight()*RATIOY))/2 , 10*(GetScreenHeight()*RATIOY) , 40*(GetScreenHeight()*RATIOY) , LIME );
        
        for (int i = 0; i < displaynumber; i++)
        {
            if ( i == 0 )
            {
                DrawText(optionshighscores[0], (GetScreenWidth() / 5 ) - MeasureText(optionshighscores[0], 25)/2 * (GetScreenHeight()*RATIOY), GetScreenHeight() / 4  + i * 50 * (GetScreenHeight()*RATIOY), 25 * (GetScreenHeight()*RATIOY), GOLD);
                DrawText(optionshighscores[1], (GetScreenWidth() / 5 *2) - MeasureText(optionshighscores[1], 25)/2 * (GetScreenHeight()*RATIOY), GetScreenHeight() / 4 + i * 50 * (GetScreenHeight()*RATIOY), 25 * (GetScreenHeight()*RATIOY), GREEN);
                DrawText(optionshighscores[2], (GetScreenWidth() / 5 *3) - MeasureText(optionshighscores[2], 25)/2 * (GetScreenHeight()*RATIOY), GetScreenHeight() / 4 + i * 50 * (GetScreenHeight()*RATIOY), 25 * (GetScreenHeight()*RATIOY), VIOLET);
                DrawText(optionshighscores[3], (GetScreenWidth() / 5 *4) - MeasureText(optionshighscores[3], 25)/2 * (GetScreenHeight()*RATIOY), GetScreenHeight() / 4 + i * 50 * (GetScreenHeight()*RATIOY), 25 * (GetScreenHeight()*RATIOY), BLUE);
            }
            else
            {
                DrawText(TextFormat("%d", i), (GetScreenWidth() / 5 ) - MeasureText(TextFormat("%d", i), 25)/2 * (GetScreenHeight()*RATIOY), GetScreenHeight() / 4  + i * 50 * (GetScreenHeight()*RATIOY), 25 * (GetScreenHeight()*RATIOY), settings->lettercolor);
                DrawText(scoresdisplay[12 - ((i-1)*3)], (GetScreenWidth() / 5 *2) - MeasureText(scoresdisplay[12 - ((i-1)*3)], 25)/2 * (GetScreenHeight()*RATIOY), GetScreenHeight() / 4  + i * 50 * (GetScreenHeight()*RATIOY), 25 * (GetScreenHeight()*RATIOY), settings->lettercolor);
                DrawText(scoresdisplay[13 - ((i-1)*3)], (GetScreenWidth() / 5 *3) - MeasureText(scoresdisplay[13 - ((i-1)*3)], 25)/2 * (GetScreenHeight()*RATIOY), GetScreenHeight() / 4  + i * 50 * (GetScreenHeight()*RATIOY), 25 * (GetScreenHeight()*RATIOY), settings->lettercolor);
                DrawText(scoresdisplay[14 - ((i-1)*3)], (GetScreenWidth() / 5 *4) - MeasureText(scoresdisplay[14 - ((i-1)*3)], 25)/2 * (GetScreenHeight()*RATIOY), GetScreenHeight() / 4  + i * 50 * (GetScreenHeight()*RATIOY), 25 * (GetScreenHeight()*RATIOY), settings->lettercolor);
            }
        }
        
        DrawText("Back", GetScreenWidth() / 2 - MeasureText("Back", 20)/2 * (GetScreenHeight()*RATIOY), (GetScreenHeight() - GetScreenHeight() / 5), 20 * (GetScreenHeight()*RATIOY), settings->lettercolor);
        
        /*************** FOR ANIMATIONS ****************/
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        if ( bulletdying )
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        //This was a bug that made the smoke spin but I kept it as a feature becaue it looked nice doubled
        DrawTexturePro( textures.smoke , (Rectangle){ bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.smoke , (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.bullet , (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        
        EndDrawing();

        if( WindowShouldClose() )
        {
            settings->select = 5;
            settings->quit = true;
        }
    }
    /************** UNLOADING AREA ********************/
    StopSoundMulti();
    UnloadTexture(textures.player);  //Texture for the player tank
    UnloadTexture(textures.bullet); //Texture for the bullet
    UnloadTexture(textures.explosion); //Texture for the explosion for bullets
    UnloadTexture(textures.smoke); //Smoke texture
    UnloadTexture(textures.wall); //Texture for the wall
    UnloadSound(sounds.shoot); //Sound for shooting
    UnloadSound(sounds.bulletmiss); //Sound for explosion
}

void loadscreen(Setti *settings)
{
     /***************** MENU OPTIONS *****************************/
    bool selected = false, shoot = false;
    settings->select = 0;
    double time = GetTime(), error = 0;
    
    char name[20] = {0};
    int letterCount = 0;
    settings->filenamefound = false;

    int optionsnumberload = 2;
    char optionsload[2][100] = {
        "Next\0",
        "Back\0"
    };

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
    //!Bullets
    bool bulletdying = false;
    Vector2 bulletexplosion = {0,0};
    int bulletdeathtimer = 0, bulletsmoke = 0, bullettimer = 0;
    Rectangle bulletdrawRec = { 0, 0, 0, 0 };
    //!MAP ART
    Rectangle sourceWall = { 0 , 0 , textures.wall.width , textures.wall.height }; //Rectangle with size of original image
    char terrainspacestar[ 15 * 41 ];   //15x41 terrain space 
    Rectangle terrainarraystar[ 15 * 41 ];
    //!FOR FAKING LOADING A MAP
    Obj playerstar[1], enemystar[1], energystar;
    loading( "assets/loadscreen", settings, playerstar, enemystar, &energystar, terrainarraystar, terrainspacestar, 1 );

    while ( !settings->quit )
    {
        /***************** TEXT BAR *****************************/
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
            name[letterCount] = 0; // Add null terminator at the end of the string.
        }
        /***************** RESIZABLE MENU BAR *****************************/
        Rectangle Menu[4] = {
            (Rectangle){ 0 , 0 , GetScreenWidth() , 50 * (GetScreenHeight()*RATIOY) }, //Rectangle for the ingame menu
            (Rectangle){ 0 , GetScreenHeight() - 5*(GetScreenHeight()*RATIOY) , GetScreenWidth(), GetScreenHeight()}, //Rectangle for bottom border
            (Rectangle){ 0 , 0 , 5 * (GetScreenWidth()*RATIOX), GetScreenHeight() }, //Rectangle for left border
            (Rectangle){ GetScreenWidth() - 5 * (GetScreenWidth()*RATIOX) , 0 , GetScreenWidth() , GetScreenHeight()}   //Rectangle for right border
        };

        /***************** PLAYER DRAWING *****************************/
        Rectangle playerdrawRec = { 
            GetScreenWidth() / 2 - MeasureText(optionsload[settings->select], 20)/2 *(GetScreenWidth()*RATIOX) - textures.player.width/20 * (GetScreenWidth()*RATIOX), 
            (GetScreenHeight() - GetScreenHeight() / 5) + 50 * settings->select * (GetScreenHeight()*RATIOY), 
            textures.player.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ), 
            ( textures.player.height * textures.player.width / textures.player.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 10 ) 
        };

        /***************** MENU OPTIONS *****************************/
        if( !selected && !shoot && !bulletdying && GetTime() > time + 0.5)
        {
            if (( IsKeyReleased(KEY_DOWN) || IsGamepadButtonReleased(0, 3)  ) && settings->select < optionsnumberload - 1)
                settings->select += 1;
            if (( IsKeyReleased(KEY_UP) || IsGamepadButtonReleased(0, 1)  ) && settings->select > 0)
                settings->select -= 1;
            if ( IsKeyReleased(KEY_ENTER) || IsGamepadButtonReleased(0, 7) || IsGamepadButtonReleased(0, 12) )
                selected = true;
        }
        /****************** MENU ANIMATIONS ************************/
        if ( selected )
        {
            shoot = true;
            PlaySoundMulti(sounds.shoot);
            bulletdrawRec = (Rectangle){ 
                    playerdrawRec.x + playerdrawRec.width / 2 - textures.bullet.width * ( ( GetScreenWidth() * ( 1.0 / 1010 ) ) / 100 ), 
                    playerdrawRec.y + playerdrawRec.height / 2 - ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 100 ), 
                    textures.bullet.width * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ), 
                    ( textures.bullet.height * textures.bullet.width / textures.bullet.height ) * ( ( GetScreenHeight() * ( 1.0 / 655 ) ) / 50 ) 
                };
            selected = false;
        }
        
        if ( shoot )
        {
            if (bulletsmoke > 20)
                bulletsmoke = 0;
            else if (bullettimer%3 == 0)
                bulletsmoke++;
            bullettimer++;
            bulletdrawRec.x += 2;
        }
        if( bulletdrawRec.x >= playerdrawRec.x + textures.bullet.width * ( ( GetScreenWidth() * ( 1.0 / 1010 ) ) / 25 ) + playerdrawRec.width / 2 + MeasureText(optionsload[settings->select], GetFontDefault().baseSize) * (GetScreenWidth()*RATIOX) * 2 )
        {
            bulletexplosion.x = bulletdrawRec.x;
            bulletexplosion.y = bulletdrawRec.y + 5;
            bulletdying = true;
            bulletdrawRec = (Rectangle){ 0 , 0 , 0 , 0 };
            bulletdeathtimer = 0;
            shoot = false;
            PlaySoundMulti(sounds.bulletmiss);
        }
        if (bulletdeathtimer > 39)
        {
            bulletexplosion.x = GetScreenWidth()*2;
            bulletexplosion.y = 0;
            bulletdying = false;
            bulletdeathtimer = 0;
            if (settings->select == 0)
            {
                if(FileExists(TextFormat("saves/%s.txt",name)))
                {
                    strcpy(settings->loadgamename, name);
                    settings->filenamefound = true;
                    break; 
                }
                else
                {
                    strcpy(settings->error, "Arquivo nao existe\0");
                    error = GetTime();
                }
            }
            else
                break;
        }
        
        BeginDrawing();

        ClearBackground( settings->theme );
        //* Map art
        if (IsWindowResized())
            loading( "assets/loadscreen", settings, playerstar, enemystar, &energystar, terrainarraystar, terrainspacestar, 0 );
        for (int i = 0; i < 15 * 41; i++)
                if (terrainspacestar[i] == '#')
                    DrawTexturePro( textures.wall , sourceWall , terrainarraystar[i] , (Vector2){ 0 , 0 } , 0 , WHITE );
        //* Menu bars
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //* Texts
        DrawText( "LOADGAME", GetScreenWidth() / 2 - MeasureText("LOADGAME", 40)/2 * (GetScreenHeight()*RATIOY) , 10*(GetScreenHeight()*RATIOY) , 40*(GetScreenHeight()*RATIOY) , LIME );
        
        DrawText( "Digite o nome do arquivo que quer carregar:", GetScreenWidth() / 2 - MeasureText("Digite o nome do arquivo que quer carregar:", 25)/2*(GetScreenHeight()*RATIOY), GetScreenHeight() / 8 + 50 * 2 * (GetScreenHeight()*RATIOY), 25*(GetScreenHeight()*RATIOY), GOLD );
        
        DrawText(name, GetScreenWidth()/2 - MeasureText(name, 40)/2, GetScreenHeight() / 8 + 50 * 5 * (GetScreenHeight()*RATIOY), 40*(GetScreenHeight()*RATIOY), settings->lettercolor);
        
        if (letterCount < 20)
        {
            DrawText("_", GetScreenWidth()/2 + 8 * (GetScreenWidth()*RATIOX) + MeasureText(name, 40)/2,  GetScreenHeight() / 8 + 50 * 5 * (GetScreenHeight()*RATIOY), 40*(GetScreenHeight()*RATIOY), settings->lettercolor);
        }
        else
            strcpy(settings->error, "Numero Maximo de Caracteres");
        
        for (int i = 0; i < optionsnumberload; i++)
            DrawText( &optionsload[i][0], GetScreenWidth() / 2 - MeasureText(&optionsload[i][0], 25)/2*(GetScreenHeight()*RATIOY), (GetScreenHeight() - GetScreenHeight() / 5) + 50 * i * (GetScreenHeight()*RATIOY), 25*(GetScreenHeight()*RATIOY), settings->lettercolor );

        /*************** FOR ANIMATIONS ****************/
        DrawTexturePro( textures.player , (Rectangle){ 0 , 0 , textures.player.width , textures.player.height } , playerdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        if ( bulletdying )
        {
            DrawTexturePro( textures.explosion , (Rectangle){ textures.explosion.width/39*bulletdeathtimer , 0 , textures.explosion.width/39 , textures.explosion.height } , (Rectangle){ bulletexplosion.x , bulletexplosion.y  , textures.explosion.width/390, textures.explosion.height/10 } , (Vector2){ (textures.explosion.width/390)/2 , (textures.explosion.height/10)/2 } , 90+30 , WHITE );
            bulletdeathtimer += 1;
        }
        //This was a bug that made the smoke spin but I kept it as a feature becaue it looked nice doubled
        DrawTexturePro( textures.smoke , (Rectangle){ bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y + bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.smoke , (Rectangle){ textures.smoke.width - textures.smoke.width/20 - bulletsmoke*textures.smoke.width/20 , 0 , textures.smoke.width/20, textures.smoke.height } , (Rectangle){ bulletdrawRec.x - bulletdrawRec.width * sin(90*PI/180) - 3, bulletdrawRec.y +bulletdrawRec.height*cos(90*PI/180) + 5, bulletdrawRec.width , bulletdrawRec.height } , (Vector2){ bulletdrawRec.width/2 , bulletdrawRec.height/2 }  , 90-180 , WHITE);
        DrawTexturePro( textures.bullet , (Rectangle){ 0 , 0 , textures.bullet.width , textures.bullet.height } , bulletdrawRec , (Vector2){ 0 , 0 } , 90 , WHITE );
        
        //*Error displaying
        if ( strcmp(settings->error, " ") )
        {
            DrawText(TextFormat("%s", settings->error), GetScreenWidth() / 2 - MeasureText(TextFormat("%s", settings->error), 15)/2*(GetScreenHeight()*RATIOY), GetScreenHeight() - GetScreenHeight() / 3 + 100*(GetScreenHeight()*(1/655)), 15*(GetScreenHeight()*RATIOY), RED);
            if (GetTime() > error + 2)
                strcpy(settings->error, " ");
        }

        EndDrawing();

        if( WindowShouldClose() )
        {
            settings->select = 5;
            settings->quit = true;
        }
    }
    /************** UNLOADING AREA ********************/

    StopSoundMulti();
    UnloadTexture(textures.player);  //Texture for the player tank
    UnloadTexture(textures.bullet); //Texture for the bullet
    UnloadTexture(textures.explosion); //Texture for the explosion for bullets
    UnloadTexture(textures.smoke); //Smoke texture
    UnloadTexture(textures.wall); //Texture for the wall
    UnloadSound(sounds.shoot); //Sound for shooting
    UnloadSound(sounds.bulletmiss); //Sound for explosion
}