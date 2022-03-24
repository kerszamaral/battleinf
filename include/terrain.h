#ifndef terrain_H_
#define terrain_H_

#include "core.h"

void terraincreate(char terrainspace[MAPY][MAPX]);

void terrainplace(  Rectangle terrainarray[ MAPY ][ MAPX ] , char terrainspace[ MAPY ][ MAPX ] );

Rectangle terraindestruct( Obj bullet , Rectangle terrain );

#endif