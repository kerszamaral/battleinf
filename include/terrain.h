#ifndef terrain_H_
#define terrain_H_

#include "core.h"

void terraincreate(char terrainspace[]);

void terrainplace(  Rectangle terrainarray[] , char terrainspace[] );

Rectangle terraindestruct( Obj bullet , Rectangle terrain, SFX *sounds );

#endif