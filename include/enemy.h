#ifndef enemy_H_
#define enemy_H_

#include "core.h"

Obj spawn(Obj spawn , char terrainspace[8][16], Rectangle terrainarray[8][16]);

Obj enemyspawn( Obj enemy , char terrainspace[8][16], Rectangle terrainarray[8][16] );

Obj enemymove(Obj enemy, Obj player);

#endif