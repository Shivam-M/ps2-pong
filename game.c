#include <math.h>
#include <stdlib.h>

#include "game.h"
#include "pad.h"
#include "render.h"

enum State state = STATE_PLAYING;

const float INITIAL_BALL_POSITION[2] = {0.5f, 0.5f};
const float INITIAL_BALL_VELOCITY[2] = {0.0075f, 0.0075f};
const float BALL_SIZE[2] = {0.025f, 0.025f};
const float PADDLE_SIZE[2] = {0.01f, 0.10f};
const float PADDLE_HORIZONTAL_PADDING = 0.05f;
const float PADDLE_MOVEMENT_SPEED = 0.025f;
const float PADDLE_HIT_EDGE_FACTOR = 0.5f;

float ball_position[2] = {INITIAL_BALL_POSITION[0], INITIAL_BALL_POSITION[1]};
float ball_velocity[2] = {INITIAL_BALL_VELOCITY[0], INITIAL_BALL_VELOCITY[1]};
float paddle_position_1[2] = {PADDLE_HORIZONTAL_PADDING, 0.5f};
float paddle_position_2[2] = {1 - PADDLE_HORIZONTAL_PADDING, 0.5f};
int score[2] = {0, 0};

void game_render_gameplay(GSGLOBAL* gs_global) {
    render_quad(gs_global, paddle_position_1[0], paddle_position_1[1], PADDLE_SIZE[0], PADDLE_SIZE[1], GS_SETREG_RGBAQ(255, 255, 255, 0, 0));
    render_quad(gs_global, paddle_position_2[0], paddle_position_2[1], PADDLE_SIZE[0], PADDLE_SIZE[1], GS_SETREG_RGBAQ(255, 255, 255, 0, 0));
    render_quad(gs_global, ball_position[0], ball_position[1], BALL_SIZE[0], BALL_SIZE[1], GS_SETREG_RGBAQ(255, 255, 255, 0, 0));
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

void game_reset_ball() {
    ball_position[0] = INITIAL_BALL_POSITION[0];
    ball_position[1] = INITIAL_BALL_POSITION[1];

    ball_velocity[0] = INITIAL_BALL_VELOCITY[0];
    ball_velocity[1] = INITIAL_BALL_VELOCITY[1];

    if (rand() % 2) {
        ball_velocity[0] *= -1.f;
    }

    if (rand() % 2) {
        ball_velocity[1] *= -1.f;
    }
}

void game_update_paddle(Pad* pad, float paddle_position[2]) {
    if (pad_button_down(pad, PAD_UP)) {
        paddle_position[1] += PADDLE_MOVEMENT_SPEED;
    }
    if (pad_button_down(pad, PAD_DOWN)) {
        paddle_position[1] -= PADDLE_MOVEMENT_SPEED;
    }

    paddle_position[1] = fminf(fmaxf(paddle_position[1], 0.0f), 1.0f);
}

void game_update_gameplay(Pad* pad_1, Pad* pad_2) {
    game_update_paddle(pad_1, paddle_position_1);
    game_update_paddle(pad_2, paddle_position_2);

    ball_position[0] += ball_velocity[0];
    ball_position[1] += ball_velocity[1];

    if (ball_position[0] < 0.f || ball_position[0] > 1.f) {
        score[ball_position[0] < 0.f ? 1 : 0]++;
        game_reset_ball();
    }

    if (ball_position[1] < 0.f || ball_position[1] > 1.f) {
        ball_position[1] = fminf(fmaxf(ball_position[1], 0.0f), 1.0f);
        ball_velocity[1] *= -1.f;
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

    game_reset_ball();
}
