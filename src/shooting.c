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

Obj shooting(Obj Bullet, Obj Other, Rectangle Menu[4], char terrainspace[MAPY][MAPX], Rectangle terrainarray[MAPY][MAPX] )
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
            Bullet = collision( Bullet , Menu[i] );
        //Tests collision against other bullets
        Bullet = collision( Bullet , Other.colRec );
        //Tests collision with each rectangle of terrain
        for (int i = 0; i < MAPY; i++)
            for (int j = 0; j < MAPX; j++)
                if (terrainspace[i][j] == '*')
                    Bullet = collision( Bullet, terrainarray[i][j]);
        //Kills Bullet if 1 sec passes or it collides with border
        if (Bullet.time == 60*1 || Bullet.colSide.x || Bullet.colSide.y || Bullet.colSide.z || Bullet.colSide.w )
        {   //Reverts the states change when firing Bullet to neutral
            Bullet.ammo = true;
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
        Bullet.time++;
    }
    return Bullet;
}