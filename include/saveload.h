#ifndef saveload_H_
#define saveload_H_

#include "core.h"

void saving(Setti *settings, char terrainspace[], Obj player[], Obj enemy[] , Obj *energy, Obj bullet[], Rectangle terrainarray[] );

void loading(char filename[20], Setti *settings, Obj player[], Obj enemy[] , Obj *energy, Obj bullet[], Rectangle terrainarray[], char terrainspace[] );

#endif