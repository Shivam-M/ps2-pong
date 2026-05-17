#ifndef __GAME_H__
#define __GAME_H__

#include <gsKit.h>

#include "pad.h"
#include "menu.h"

struct AIDifficulty {
    float speed_multiplier;
    float detection_range;
};

struct Settings {
    enum MenuValue game_mode;
    struct AIDifficulty difficulty;
    union {
        int time_limit;
        int score_limit;
    };
    int offsets[2];
    bool audio;
    bool singleplayer;
};

enum State {
    STATE_MENU,
    STATE_PAUSE,
    STATE_PLAYING,
    STATE_END
};

void game_render(GSGLOBAL* gs_global);

void game_update(Pad* pad_1, Pad* pad_2);

void game_initialise(GSGLOBAL* gs_global, u64* background_colour);

void game_shutdown();

#endif
