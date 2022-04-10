#ifndef enemy_H_
#define enemy_H_

#include "core.h"
#include "collision.h"

void spawn( Setti *settings , Obj *spawn , char terrainspace[], Rectangle terrainarray[] , Obj player[] , Obj enemy[]);

void enemyspawn( Setti *settings , Obj *enemy , char terrainspace[], Rectangle terrainarray[] , Obj player[] , Obj otherenemy[] );

void enemymove(Setti *settings , Obj *enemy, Obj player[]);

#endif