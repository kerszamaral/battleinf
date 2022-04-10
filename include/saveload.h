#ifndef saveload_H_
#define saveload_H_

#include "core.h"

void saving(Setti *settings, char terrainspace[], Obj player[], Obj enemy[] , Obj *energy, Rectangle terrainarray[] );

void loading(char filename[100], Setti *settings, Obj player[], Obj enemy[] , Obj *energy, Rectangle terrainarray[], char terrainspace[], bool print );

#endif