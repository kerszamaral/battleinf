#ifndef shooting_H_
#define shooting_H_

#include "core.h"

void shoot( Obj *Shooter , Obj *Bullet );

void shooting(Setti *settings , Obj *Bullet, Obj otherBullets[ settings->players + settings->level ], Obj Player[ settings->players ] , Obj enemy[ settings->level ] , Rectangle Menu[4], char terrainspace[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)] , Textus *textures );

#endif