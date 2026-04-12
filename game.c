#include "game.h"
#include "pad.h"
#include "render.h"

enum State state = STATE_PLAYING;

// would be cleaner to centre-align...
float ball_position[2] = {0.45, 0.45};
float ball_velocity[2] = {0.015, 0.015};
float ball_size[2] = {0.025, 0.025};
float paddle_position_1[2] = {0.05, 0.5f};
float paddle_position_2[2] = {0.94, 0.5f};
float paddle_size[2] = {0.01, 0.10};

void game_render_gameplay(GSGLOBAL* gs_global) {
    render_quad(gs_global, paddle_position_1[0], paddle_position_1[1], paddle_size[0], paddle_size[1], GS_SETREG_RGBAQ(255, 255, 255, 0, 0));
    render_quad(gs_global, paddle_position_2[0], paddle_position_2[1], paddle_size[0], paddle_size[1], GS_SETREG_RGBAQ(255, 255, 255, 0, 0));
    render_quad(gs_global, ball_position[0], ball_position[1], ball_size[0], ball_size[1], GS_SETREG_RGBAQ(255, 255, 255, 0, 0));
}

void game_render(GSGLOBAL* gs_global) {
    switch (state) {
        case STATE_PLAYING:
            game_render_gameplay(gs_global);
            break;
        case STATE_PAUSE:
            break;
        case STATE_MENU:
            break;
    }
}

void game_update_gameplay(Pad* pad_1, Pad* pad_2) {
    if (pad_button_down(pad_2, PAD_LEFT)) {
        ball_position[0] += 0.01f;
    }
    if (pad_button_down(pad_2, PAD_RIGHT)) {
        ball_position[0] -= 0.01f;
    }
    if (pad_button_down(pad_2, PAD_UP)) {
        ball_position[1] += 0.01f;
    }
    if (pad_button_down(pad_2, PAD_DOWN)) {
        ball_position[1] -= 0.01f;
    }

    ball_position[0] += ball_velocity[0];
    ball_position[1] += ball_velocity[1];

    if (ball_position[0] < 0 || ball_position[0] > 1) {
        ball_velocity[0] *= -1;
    }

    if (ball_position[1] < 0 || ball_position[1] > 1) {
        ball_velocity[1] *= -1;
    }
}

void game_update(Pad* pad_1, Pad* pad_2) {
    switch (state) {
        case STATE_PLAYING:
            game_update_gameplay(pad_1, pad_2);
            break;
        case STATE_PAUSE:
            break;
        case STATE_MENU:
            break;
    }
}

void game_initialise(u64* background_colour) {
    *background_colour = GS_SETREG_RGBAQ(40, 40, 40, 0, 0);
}
