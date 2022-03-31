/* Ian Kersz - Cartão UFRGS: 00338368
18.03.22
BattleInf Jogo de tanquezinho lá do NES
*/
#include "raylib.h"
#include "core.h"
#include "jogo.h"
#include "menu.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
    Setti settings = { 1 , 0 , 0 , 0 , false , false, BLACK, 0, false, 0 };
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Game");
    InitAudioDevice();
    SetMasterVolume(1);
    SetWindowIcon(LoadImage("assets/player.png"));
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

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


    while ( !settings.quit )
    {
        startscreen( &settings );
        switch ( settings.select )
        {
        case 0:
            do{
                settings.level++;
                jogo( &settings , &textures , &sounds );
            }while ( settings.won );
            
            if (!IsWindowFullscreen())
                SetWindowState(FLAG_WINDOW_RESIZABLE);
            
            if (settings.score > LoadStorageValue(0) && !settings.quit)
                nome( &settings );
            settings.exitgame = false;
            settings.level = 0;
            settings.score = 0;
            break;
        case 2:
            highscorescreen( &settings );
            break;
        case 3:
            settingscreen( &settings );
            break;
        default:
            break;
        }
    }

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

    CloseAudioDevice();
    CloseWindow();
    return 0;
}