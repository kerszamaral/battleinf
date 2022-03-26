#ifndef enemy_H_
#define enemy_H_

#include "core.h"

Obj spawn( Obj spawn , int level , char terrainspace[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)] , Rectangle playerCol , Obj enemy[level]);

Obj enemyspawn( Obj enemy , int level , char terrainspace[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)] , Rectangle playerCol , Obj otherenemy[level] );

Obj enemymove(Obj enemy, Obj player);

#endif