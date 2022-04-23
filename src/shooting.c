#include "shooting.h"

//Bullet shooting sequence start
void shoot( Obj *Shooter, Obj *Bullet )
{
    //Shooting setup
    Bullet->ammo = false; //Sets ammo to false
    Bullet->health = 1; //Sets playbullet health to 1
    Bullet->rot = Shooter->rot; //Stores the Shooter rotation when Bullet fires
    Bullet->pos.x = Shooter->draw.x; //Stores the Shooter x postion when Bullet
    Bullet->pos.y = Shooter->draw.y; //Stores the Shooter y postion when Bullet
    Bullet->realTime = GetTime(); //Stores the current time when Bullet fires

    switch (Bullet->rot) //Offsets for each rotation to fire from center and end of barrel
    {
    case 0:
        Bullet->pos.y -= Shooter->cen.y;
        Bullet->pos.x -= Bullet->cen.x;
        break;
    case 90:
        Bullet->pos.x += Shooter->cen.x;
        Bullet->pos.y -= Bullet->cen.y;
        break;
    case 180:
        Bullet->pos.y += Shooter->cen.y;
        Bullet->pos.x -= Bullet->cen.x;
        break;
    case 270:
        Bullet->pos.x -= Shooter->cen.x;
        Bullet->pos.y -= Bullet->cen.y;
        break;
    }

    PlaySound(Bullet->soundEffect); //Plays the shoot sound
}

//Bullet shooting sequence 
void shooting(Setti *settings , Obj *Bullet, Obj otherBullets[], Obj Player[] , Obj enemy[] , Rectangle Menu[], char terrainspace[], Rectangle terrainarray[] , Textus *textures )
{
    //*Drawing (Because cen is the center(1/2) of the image scaled, multiply by 2 to get the full size)
    Bullet->draw = (Vector2){ Bullet->pos.x + Bullet->cen.x, Bullet->pos.y + Bullet->cen.y }; //Draw position and draw rectangle update / Sets player.draw to be player.pos + offset
    Bullet->drawRec = (Rectangle){ Bullet->draw.x, Bullet->draw.y, Bullet->cen.x*2, Bullet->cen.y*2 }; //Rectangle resized and offset for Bullet drawing
    Bullet->colRec = (Rectangle){ Bullet->pos.x, Bullet->pos.y, Bullet->cen.x*2, Bullet->cen.y*2 }; //Bullet collision rectangle update

    if ( Bullet->health > 0 ) //If the bullet is alive
    {
        //*Collision
        Bullet->colSide = (Vector4){ 0, 0, 0, 0 }; //Resets collision detection
        for (int i = 0; i < 4; i++) //Checks collision with sides
            collision( Bullet , Menu[i] , 2);
        for (int i = 0; i < settings->players + settings->level; i++) //Checks collision with other bullets
            collision( Bullet , otherBullets[i].colRec , 2);
        for (int i = 0; i < MAPSIZE; i++) //Checks collision with terrain
            if (terrainspace[i] == '#')
                collision( Bullet, terrainarray[i], 2);
        for (int p = 0; p < settings->players; p++) //Checks collision with players
            if ( Bullet->id != Player[p].id ) //Checks if the bullet is not from players
                if ( CheckCollisionRecs( Player[p].colRec, Bullet->colRec ) )
                {
                    Bullet->colSide.x = 1; //Makes the bullet collide
                    Bullet->score = p + 1; //Sets the bullet score
                }
        for (int k = 0; k < settings->level; k++) //Checks collision with enemies
            if ( enemy[k].id != Bullet->id - settings->players || Player->id == Bullet->id ) //Checks if the bullet is not from enemies
                if ( CheckCollisionRecs( Bullet->colRec, enemy[k].colRec ) ) 
                {
                    Bullet->colSide.x = 1; //Makes the bullet collide
                    Bullet->score = settings->players + k + 1; //Sets the bullet score
                }

        if (Bullet->time == 60*1 || Bullet->colSide.x || Bullet->colSide.y || Bullet->colSide.z || Bullet->colSide.w ) //Reverts the states change when firing Bullet to neutral
        {   
            Bullet->deathpos = Bullet->draw;
            Bullet->dying = true; 
            Bullet->health = 0;
            Bullet->pos = (Vector2){0,GetScreenHeight()*2};
            Bullet->time = 0;
            PlaySound(Bullet->soundEffect2); //Plays the bullet miss sound
            if (Bullet->score != 0 && Bullet->score - 1 < settings->players)
                if (Bullet->id >= settings->players )
                        Player[ Bullet->score - 1 ].health--;
            if ( Bullet->score - 1 >= settings->players )
            {
                int k = Bullet->score - settings->players - 1;
                if ( Bullet->id < settings->players )
                    enemy[k].health--;
                if (enemy[k].health == 0)
                {
                    enemy[k].deathpos = enemy[k].draw;
                    enemy[k].dying = true;
                    Player->score += 800;
                    enemy[k].pos = (Vector2){ GetScreenWidth() , GetScreenHeight()*2 };
                    PlaySound(enemy[k].soundEffect);
                }
            }
        }
        Bullet->score = 0; //Resets the bullet score

        //*Movement
        if (!settings->pause) //If the game is not paused
            switch (Bullet->rot) //Switch for speed based on rotation
            {
            case 0: //Up
                Bullet->pos.y -= Bullet->speed;
                break;
            case 90: //Right
                Bullet->pos.x += Bullet->speed;
                break;
            case 180: //Down
                Bullet->pos.y += Bullet->speed;
                break;
            case 270: //Left
                Bullet->pos.x -= Bullet->speed;
                break;
            }
        //*Drawing 
        //Smoke trails
        DrawTexturePro( textures->smoke, (Rectangle){ Bullet->death*textures->smoke.width/20, 0, textures->smoke.width/20, textures->smoke.height }, (Rectangle){ Bullet->drawRec.x - Bullet->cen.x * 2 * sin(Bullet->rot*PI/180), Bullet->drawRec.y + Bullet->cen.y*2*cos(Bullet->rot*PI/180), Bullet->drawRec.width, Bullet->drawRec.height }, Bullet->cen, Bullet->rot-180, WHITE);
        DrawTexturePro( textures->smoke, (Rectangle){ textures->smoke.width - textures->smoke.width/20 - Bullet->death*textures->smoke.width/20, 0, textures->smoke.width/20, textures->smoke.height }, (Rectangle){ Bullet->drawRec.x - Bullet->cen.x * 2 * sin(Bullet->rot*PI/180), Bullet->drawRec.y + Bullet->cen.y*2*cos(Bullet->rot*PI/180), Bullet->drawRec.width, Bullet->drawRec.height }, Bullet->cen, Bullet->rot-180, WHITE);
        //Bullet sprite
        DrawTexturePro( textures->bullet, Bullet->sourceRec, Bullet->drawRec, Bullet->cen, Bullet->rot, Bullet->color );
        //Time of flight
        if (!settings->pause)
            Bullet->time++;
        //Smoke Animation (uses the death timer for other objects)
        if (Bullet->death > 20) 
            Bullet->death = 0;
        else if (Bullet->time%3 == 0 && !settings->pause)
            Bullet->death++;
    }

    if (Bullet->dying) //If the bullet is dying
    {
        //*Explosion animation drawing
        DrawTexturePro( textures->explosion, (Rectangle){ textures->explosion.width/39*Bullet->deathtimer, 0, textures->explosion.width/39, textures->explosion.height }, (Rectangle){ Bullet->deathpos.x, Bullet->deathpos.y, textures->explosion.width/390, textures->explosion.height/10 }, (Vector2){ (textures->explosion.width/390)/2, (textures->explosion.height/10)/2 }, Bullet->rot+30, WHITE );
        //*Death timer
        if (!settings->pause) //If the game is not paused
            Bullet->deathtimer += 2; //Increments the death timer by 2 (faster animation)
        if (Bullet->deathtimer > 38) //If the death timer is greater than 38
        {
            Bullet->ammo = true; //Sets the bullet to be able to be fired again
            Bullet->deathtimer = 0; //Resets the death timer
            Bullet->dying = false; //Sets the bullet to not be dying
        }
    }
}