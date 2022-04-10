#ifndef shooting_H_
#define shooting_H_

#include "core.h"
#include "collision.h"

void shoot( Obj *Shooter , Obj *Bullet );

void shooting(Setti *settings , Obj *Bullet, Obj otherBullets[], Obj Player[] , Obj enemy[] , Rectangle Menu[] , char terrainspace[], Rectangle terrainarray[] , Textus *textures );

#endif