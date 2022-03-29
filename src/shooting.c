#include "collision.h"
#include "core.h"

//Uses shooter posint and rotation to start bullet shooting
Obj shoot( Obj Shooter, Obj Bullet )
{
    //Shooting setup
    Bullet.ammo = false; //Sets ammo to false
    Bullet.health = 1; //Sets playbullet health to 1

    Bullet.rot = Shooter.rot; //Stores the Shooter rotation when Bullet fires for next steps
    Bullet.pos.x = Shooter.draw.x; //Stores the Shooter x postion when Bullet fires for next steps
    Bullet.pos.y = Shooter.draw.y; //Stores the Shooter y postion when Bullet fires for next steps

    //Offsets for each rotation to fire from center and end of barrel
    switch (Bullet.rot)
    {
    case 0:
        Bullet.pos.y -= Shooter.cen.y;
        Bullet.pos.x -= Bullet.cen.x;
        break;
    case 90:
        Bullet.pos.x += Shooter.cen.x;
        Bullet.pos.y -= Bullet.cen.y;
        break;
    case 180:
        Bullet.pos.y += Shooter.cen.y;
        Bullet.pos.x -= Bullet.cen.x;
        break;
    case 270:
        Bullet.pos.x -= Shooter.cen.x;
        Bullet.pos.y -= Bullet.cen.y;
        break;
    }
    return Bullet;
}

Obj shooting(Obj Bullet, Obj Other, Rectangle Menu[4], char terrainspace[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)] , Textus *textures )
{
    //Draw position and draw rectangle updates
    //Sets player.draw to be player.pos + offset
    Bullet.draw = (Vector2){ Bullet.pos.x + Bullet.cen.x , Bullet.pos.y + Bullet.cen.y };
    //Rectangle resized and offset for player drawing
    Bullet.drawRec = (Rectangle){ Bullet.draw.x, Bullet.draw.y, Bullet.cen.x*2 , Bullet.cen.y*2 };
    //Bullet collision rectangle
    Bullet.colRec = (Rectangle){ Bullet.pos.x, Bullet.pos.y, Bullet.cen.x*2, Bullet.cen.y*2 };
    //Because Bullet cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size

    if (Bullet.health >= 1) //test to see if should draw Bullet
    {
        //Resets collision detection
        Bullet.colSide = (Vector4){ 0 , 0 , 0 , 0 };
        //Tests collision with sides
        for (int i = 0; i < 4; i++)
            Bullet = collision( Bullet , Menu[i] , 2);
        //Tests collision against other bullets
        Bullet = collision( Bullet , Other.colRec , 2);
        //Tests collision with each rectangle of terrain
        for (int i = 0; i < GetScreenHeight()/(GetScreenHeight()/12); i++)
            for (int j = 0; j < GetScreenWidth()/(GetScreenHeight()/12); j++)
                if (terrainspace[i][j] == '*')
                    Bullet = collision( Bullet, terrainarray[i][j], 2);
        //Kills Bullet if 1 sec passes or it collides with border
        if (Bullet.time == 60*1 || Bullet.colSide.x || Bullet.colSide.y || Bullet.colSide.z || Bullet.colSide.w )
        {   //Reverts the states change when firing Bullet to neutral
            Bullet.deathpos = Bullet.draw;
            Bullet.dying = true;
            Bullet.health = 0;
            Bullet.pos = (Vector2){0,SCREENHEIGHT};
            Bullet.time = 0;
        }
        
        //Moves Bullet based on position and speed
        switch (Bullet.rot)
        {
        case 0:
            Bullet.pos.y -= Bullet.speed;
            break;
        case 90:
            Bullet.pos.x += Bullet.speed;
            break;
        case 180:
            Bullet.pos.y += Bullet.speed;
            break;
        case 270:
            Bullet.pos.x -= Bullet.speed;
            break;
        }

        if (Bullet.death > 20)
            Bullet.death = 0;
        else if (Bullet.time%3 == 0)
            Bullet.death++;

        DrawTexturePro( textures->smoke , (Rectangle){ Bullet.death*textures->smoke.width/20 , 0 , textures->smoke.width/20, textures->smoke.height } , (Rectangle){ Bullet.drawRec.x - Bullet.cen.x * 2 * sin(Bullet.rot*PI/180) , Bullet.drawRec.y + Bullet.cen.y*2*cos(Bullet.rot*PI/180) , Bullet.drawRec.width , Bullet.drawRec.height } , Bullet.cen , Bullet.rot-180 , WHITE);
        DrawTexturePro( textures->bullet , Bullet.sourceRec , Bullet.drawRec , Bullet.cen , Bullet.rot , Bullet.color );

        Bullet.time++;
    }

    if (Bullet.dying)
    { // + Bullet.cen.x * sin(Bullet.rot*PI/180) //- Bullet.cen.y*cos(Bullet.rot*PI/180)
        DrawTexturePro( textures->explosion , (Rectangle){ textures->explosion.width/39*Bullet.deathtimer , 0 , textures->explosion.width/39 , textures->explosion.height } , (Rectangle){ Bullet.deathpos.x , Bullet.deathpos.y  , textures->explosion.width/390, textures->explosion.height/10 } , (Vector2){ (textures->explosion.width/390)/2 , (textures->explosion.height/10)/2 } , Bullet.rot+30 , WHITE );
        Bullet.deathtimer += 2;
        if (Bullet.deathtimer > 38)
        {
            Bullet.ammo = true;
            Bullet.deathtimer = 0;
            Bullet.dying = false;
        };
    }

    return Bullet;
}