#ifndef player_H_
#define player_H_
#include "core.h"

void moveplayer( Obj *player , Setti *settings );

void playershoot( Obj *player, Obj *Bullet , Setti *settings, SFX *Sounds);

#endif