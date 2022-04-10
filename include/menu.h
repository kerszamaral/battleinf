#ifndef menu_H_
#define menu_H_

#include "core.h"
#include "saveload.h"

void startscreen(Setti *settings);

void namescreen(Setti *settings);

void settingscreen(Setti *settings);

void pausescreen(Setti *settings, char terrainspace[], Obj player[], Obj enemy[] , Obj *energy, Rectangle terrainarray[] );

void highscorescreen(Setti *settings);

void loadscreen(Setti *settings);

#endif