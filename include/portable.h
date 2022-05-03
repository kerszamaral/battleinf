#ifndef portable_H_
#define portable_H_

#include "core.h"
#include <sys/stat.h>
#include "assets.h"
#include "saves.h"

void makeFile(const char *name, const char *ext, unsigned int len, unsigned char data[])
{
    if (!FileExists(TextFormat("assets/%s.%s", name, ext)))
    {
        FILE *file = fopen(TextFormat("assets/%s.%s", name, ext), "wb");

        for (int i = 0; i < len; i++)
        {
            fwrite(&data[i], sizeof(char), 1, file);
        }

        fclose(file);
    }
}

void makeLevel(const char *name, const char mapname[])
{
    if (!FileExists(TextFormat("saves/%s.txt", name)))
    {
        FILE *file = fopen(TextFormat("saves/%s.txt", name), "w");
        fprintf(file, "%s", mapname);
        fclose(file);
    }
}

void createPortableAssets(void)
{
    if (!DirectoryExists("assets"))
        mkdir("assets");

    makeFile("bullet", "png", bullet_png_len, bullet_png);
    makeFile("BulletMissSFX", "wav", BulletMissSFX_wav_len, BulletMissSFX_wav);
    makeFile("BulletShotSFX", "wav", BulletShotSFX_wav_len, BulletShotSFX_wav);
    makeFile("controller", "png", controller_png_len, controller_png);
    makeFile("enemy", "png", enemy_png_len, enemy_png);
    makeFile("energy", "png", energy_png_len, energy_png);
    makeFile("explosionBullets", "png", explosionBullets_png_len, explosionBullets_png);
    makeFile("explosionVehicles", "png", explosionVehicles_png_len, explosionVehicles_png);
    makeFile("explosionVehicleSFX", "wav", explosionVehicleSFX_wav_len, explosionVehicleSFX_wav);
    makeFile("fire", "png", fire_png_len, fire_png);
    makeFile("gameendSFX", "wav", gameendSFX_wav_len, gameendSFX_wav);
    makeFile("gamestartSFX", "wav", gamestartSFX_wav_len, gamestartSFX_wav);
    makeFile("health", "png", health_png_len, health_png);
    makeFile("player", "png", player_png_len, player_png);
    makeFile("TerrainHitSFX", "wav", TerrainHitSFX_wav_len, TerrainHitSFX_wav);
    makeFile("wall", "png", wall_png_len, wall_png);
}

void createPortableSaves(void)
{
    if (!DirectoryExists("saves"))
        mkdir("saves");

    makeLevel("highscorescreen", highscorescreenMap);
    makeLevel("level1", level1Map);
    makeLevel("level2", level2Map);
    makeLevel("loadscreen", loadscreenMap);
    makeLevel("namescreen", namescreenMap);
    makeLevel("settingsscreen", settingsscreenMap);
    makeLevel("startscreen", startscreenMap);
}

#endif