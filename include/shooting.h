#ifndef shooting_H_
#define shooting_H_

#include "core.h"

void shoot( Obj *Shooter , Obj *Bullet , SFX *Sounds );

void shooting(Setti *settings , Obj *Bullet, Obj otherBullets[], Obj Player[] , Obj enemy[] , Rectangle Menu[], SFX *Sounds , char terrainspace[][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[][GetScreenWidth()/(GetScreenHeight()/12)] , Textus *textures );

#endif