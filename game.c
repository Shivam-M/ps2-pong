#include "game.h"
#include "pad.h"
#include "render.h"

enum State state = STATE_PLAYING;
float ball_position[2] = {0.45, 0.45};

void game_render_gameplay(GSGLOBAL* gs_global) {
    render_quad(gs_global, 0.0, 0.0, 0.5f, 0.5f, GS_SETREG_RGBAQ(128, 0, 0, 0, 0));
    render_quad(gs_global, 0.5f, 0.f, 0.5f, 0.5f, GS_SETREG_RGBAQ(0, 128, 0, 0, 0));
    render_quad(gs_global, 0.f, 0.5f, 0.5f, 0.5f, GS_SETREG_RGBAQ(0, 0, 128, 0, 0));
    render_quad(gs_global, 0.5f, 0.5f, 0.5f, 0.5f, GS_SETREG_RGBAQ(0, 128, 128, 0, 0));
    render_quad(gs_global, ball_position[0], ball_position[1], 0.1f, 0.1f, GS_SETREG_RGBAQ(255, 255, 255, 0, 0));
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
