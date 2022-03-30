#ifndef terrain_H_
#define terrain_H_

#include "core.h"

void terraincreate(char terrainspace[][GetScreenWidth()/(GetScreenHeight()/12)]);

void terrainplace(  Rectangle terrainarray[][ GetScreenWidth()/(GetScreenHeight()/12) ] , char terrainspace[][ GetScreenWidth()/(GetScreenHeight()/12) ] );

Rectangle terraindestruct( Obj bullet , Rectangle terrain );

#endif