#ifndef enemy_H_
#define enemy_H_

#include "core.h"

void spawn( Setti *settings , Obj *spawn , char terrainspace[][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[][GetScreenWidth()/(GetScreenHeight()/12)] , Obj player[] , Obj enemy[]);

void enemyspawn( Setti *settings , Obj *enemy , char terrainspace[][GetScreenWidth()/(GetScreenHeight()/12)], Rectangle terrainarray[][GetScreenWidth()/(GetScreenHeight()/12)] , Obj player[] , Obj otherenemy[] );

void enemymove(Setti *settings , Obj *enemy, Obj player[]);

#endif