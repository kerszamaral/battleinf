#ifndef terrain_H_
#define terrain_H_

#include "core.h"

void terraincreate(char terrainspace[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)]);

void terrainplace(  Rectangle terrainarray[ GetScreenHeight()/(GetScreenHeight()/12) ][ GetScreenWidth()/(GetScreenHeight()/12) ] , char terrainspace[ GetScreenHeight()/(GetScreenHeight()/12) ][ GetScreenWidth()/(GetScreenHeight()/12) ] );

Rectangle terraindestruct( Obj bullet , Rectangle terrain );

#endif