#ifndef terrain_H_
#define terrain_H_

#include "core.h"

void terraincreate(char terrainspace[GetScreenHeight()/TERRAINSIZE][GetScreenWidth()/TERRAINSIZE]);

void terrainplace(  Rectangle terrainarray[ GetScreenHeight()/TERRAINSIZE ][ GetScreenWidth()/TERRAINSIZE ] , char terrainspace[ GetScreenHeight()/TERRAINSIZE ][ GetScreenWidth()/TERRAINSIZE ] );

Rectangle terraindestruct( Obj bullet , Rectangle terrain );

#endif