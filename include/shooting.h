#ifndef shooting_H_
#define shooting_H_

#include "core.h"

Obj shoot( Obj Shooter, Obj Bullet );

Obj shooting(Obj Bullet, Obj Other, Rectangle Menu[4], char terrainspace[8][16], Rectangle terrainarray[8][16] , Textus *textures );

#endif