#ifndef player_H_
#define player_H_

#include "core.h"
#include "shooting.h"

void moveplayer( Obj *player , Setti *settings );

void playershoot( Obj *player, Obj *Bullet , Setti *settings );

#endif