#ifndef menu_H_
#define menu_H_

#include "core.h"

void startscreen(Setti *settings);

void nome(Setti *settings);

bool IsAnyKeyPressed();

void settingscreen(Setti *settings);

void pausescreen(Setti *settings, char terrainspace[], Obj player[], Obj enemy[] , Obj *energy, Obj bullet[], Rectangle terrainarray[] );

void highscorescreen(Setti *settings);

#endif