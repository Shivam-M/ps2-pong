#ifndef __GAME_H__
#define __GAME_H__

#include <gsKit.h>

#include "pad.h"

enum State {
    STATE_MENU,
    STATE_PAUSE,
    STATE_PLAYING
};

void game_render(GSGLOBAL* gs_global);

void game_update(Pad* pad_1, Pad* pad_2);

void game_initialise(u64* background_colour);

#endif
