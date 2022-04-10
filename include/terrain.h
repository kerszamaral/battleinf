#ifndef terrain_H_
#define terrain_H_

#include "core.h"
#include "collision.h"

void terraincreate(char terrainspace[]);

void terrainplace(  Rectangle terrainarray[] , char terrainspace[] );

void terraindestruct( Obj bullet , Rectangle *terrain, SFX *sounds );

#endif