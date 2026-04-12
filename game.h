#ifndef __GAME_H__
#define __GAME_H__

#include <gsKit.h>

#include "pad.h"

void game_render(GSGLOBAL* gs_global);

void game_update(Pad* pad_1, Pad* pad_2);

#endif
