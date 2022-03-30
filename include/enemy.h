#ifndef enemy_H_
#define enemy_H_

#include "core.h"

void spawn( Setti *settings , Obj *spawn , char terrainspace[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)] , Obj player[settings->players] , Obj enemy[settings->level]);

void enemyspawn( Setti *settings , Obj *enemy , char terrainspace[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[GetScreenHeight()/(GetScreenHeight()/12)][GetScreenWidth()/(GetScreenHeight()/12)] , Obj player[settings->players] , Obj otherenemy[settings->level] );

void enemymove(Setti *settings , Obj *enemy, Obj player[settings->players]);

#endif