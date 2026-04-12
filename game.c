#include "game.h"
#include "pad.h"
#include "render.h"

float ball_position[2] = {0.45, 0.45};

void game_render(GSGLOBAL* gs_global) {
    render_quad(gs_global, 0.0, 0.0, 0.5f, 0.5f, GS_SETREG_RGBAQ(128, 0, 0, 0, 0));
    render_quad(gs_global, 0.5f, 0.f, 0.5f, 0.5f, GS_SETREG_RGBAQ(0, 128, 0, 0, 0));
    render_quad(gs_global, 0.f, 0.5f, 0.5f, 0.5f, GS_SETREG_RGBAQ(0, 0, 128, 0, 0));
    render_quad(gs_global, 0.5f, 0.5f, 0.5f, 0.5f, GS_SETREG_RGBAQ(0, 128, 128, 0, 0));
    render_quad(gs_global, ball_position[0], ball_position[1], 0.1f, 0.1f, GS_SETREG_RGBAQ(255, 255, 255, 0, 0));
}

void game_update(Pad* pad_1, Pad* pad_2) {
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
