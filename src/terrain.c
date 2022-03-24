#include "core.h"
#include "collision.h"
 
Rectangle terraindestruct( Obj bullet , Rectangle terrain )
{
    bullet.colSide.x = 0;
    bullet.colSide.y = 0;
    bullet.colSide.z = 0;
    bullet.colSide.w = 0;
    bullet = collision( bullet , terrain , 3);
    if (bullet.colSide.x && terrain.height > 0)
        terrain.height -= 10;
    if (bullet.colSide.y && terrain.width > 0)
    {
        terrain.x += 10;
        terrain.width -= 10;
    }
    if (bullet.colSide.z && terrain.height > 0)
    {
        terrain.y += 10;
        terrain.height -= 10;
    }
    if (bullet.colSide.w && terrain.width > 0)
        terrain.width -= 10;

    if (terrain.height < 10 || terrain.height < 10)
    {
        terrain.height = 0;
        terrain.width = 0;
        terrain.x = SCREENWIDTH;
        terrain.y = 0;
    }

    return terrain;
}