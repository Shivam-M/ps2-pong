#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>

#include "game.h"
#include "pad.h"
#include "render.h"
#include "menu.h"

enum State state = STATE_MENU;
struct Menu* current_menu;
GSFONT* font;

const float BALL_INITIAL_POSITION[2] = {0.5f, 0.5f};
const float BALL_INITIAL_VELOCITY[2] = {0.0075f, 0.0075f};
const float BALL_MAX_HORIZONTAL_VELOCITY = 0.015f;
const float BALL_SIZE[2] = {0.025f, 0.025f};
const float PADDLE_SIZE[2] = {0.01f, 0.10f};
const float PADDLE_HORIZONTAL_PADDING = 0.05f;
const float PADDLE_MOVEMENT_SPEED = 0.025f;
const float PADDLE_HIT_EDGE_FACTOR = 0.25f;

float ball_position[2] = {BALL_INITIAL_POSITION[0], BALL_INITIAL_POSITION[1]};
float ball_velocity[2] = {BALL_INITIAL_VELOCITY[0], BALL_INITIAL_VELOCITY[1]};
float paddle_position_1[2] = {PADDLE_HORIZONTAL_PADDING, 0.5f};
float paddle_position_2[2] = {1 - PADDLE_HORIZONTAL_PADDING, 0.5f};
int score[2] = {0, 0};
char formatted_score[16] = "0 - 0";

// #define DEBUG_GAME

#ifdef DEBUG_GAME
void game_debug() {
    scr_setXY(0, 0);
    scr_printf("\nBALL POSITION: %f, %f\n", ball_position[0], ball_position[1]);
    scr_printf("BALL VELOCITY: %f, %f\n", ball_velocity[0], ball_velocity[1]);
    scr_printf("LEFT PADDLE: %f\n", paddle_position_1[1]);
    scr_printf("RIGHT PADDLE: %f\n", paddle_position_2[1]);
    scr_printf("SCORE: %d vs %d\n", score[0], score[1]);
    scr_printf("FONT: %p\n", font);
}
#endif

void game_reset_ball() {
    ball_position[0] = BALL_INITIAL_POSITION[0];
    ball_position[1] = BALL_INITIAL_POSITION[1];

    ball_velocity[0] = BALL_INITIAL_VELOCITY[0];
    ball_velocity[1] = BALL_INITIAL_VELOCITY[1];

    if (rand() % 2) {
        ball_velocity[0] *= -1.f;
    }

    if (rand() % 2) {
        ball_velocity[1] *= -1.f;
    }
}

void game_reset_state() {
    score[0] = 0;
    score[1] = 0;
    snprintf(formatted_score, sizeof(formatted_score), "0 - 0");

    paddle_position_1[1] = 0.5f;
    paddle_position_2[1] = 0.5f;

    game_reset_ball();
}

void game_render_menu(GSGLOBAL* gs_global) {
    const float MENU_VERTICAL_STEP = 0.1f;
    float y = 0.5f - ((current_menu->size / 2) * MENU_VERTICAL_STEP);

    for (int i = 0; i < current_menu->size; i++) {
        const struct MenuItem* item = &current_menu->items[i];

        u64 colour;

        if (!item->enabled) {
            colour = GS_SETREG_RGBAQ(50, 50, 50, 0, 0);
        } else if (current_menu->selected == i) {
            colour = GS_SETREG_RGBAQ(0, 0, 255, 0x80, 0);
        } else {
            colour = GS_SETREG_RGBAQ(255, 255, 255, 0x80, 0);
        }

        render_text(gs_global, 0.5f, y, 2.0f, font, colour, item->name);
        y += MENU_VERTICAL_STEP;
    }
}

void game_render_pause(GSGLOBAL* gs_global) {
    render_text(gs_global, 0.5f, 0.475f, 4.0f, font, GS_SETREG_RGBAQ(255, 0, 0, 0x80, 0), "PAUSED");
    render_text(gs_global, 0.5f, 0.55f, 2.0f, font, GS_SETREG_RGBAQ(255, 0, 0, 0x80, 0), "PRESS START TO RESUME");
}

void game_render_gameplay(GSGLOBAL* gs_global) {
    render_text(gs_global, 0.5f, 0.1f, 4.0f, font, GS_SETREG_RGBAQ(230, 230, 230, 0, 0), formatted_score);

    render_quad(gs_global, paddle_position_1[0], paddle_position_1[1], PADDLE_SIZE[0], PADDLE_SIZE[1], GS_SETREG_RGBAQ(255, 255, 255, 0x40, 0));
    render_quad(gs_global, paddle_position_2[0], paddle_position_2[1], PADDLE_SIZE[0], PADDLE_SIZE[1], GS_SETREG_RGBAQ(255, 255, 255, 0x40, 0));
    render_quad(gs_global, ball_position[0], ball_position[1], BALL_SIZE[0], BALL_SIZE[1], GS_SETREG_RGBAQ(255, 255, 255, 0x40, 0));
}

void game_render(GSGLOBAL* gs_global) {
    switch (state) {
        case STATE_PLAYING:
            game_render_gameplay(gs_global);
            break;
        case STATE_PAUSE:
            game_render_gameplay(gs_global);
            game_render_pause(gs_global);
            break;
        case STATE_MENU:
            game_render_menu(gs_global);
            break;
    }
}

void game_invoke_menu_action(enum MenuAction action) {
    switch (action) {
        case MENU_MAIN_PLAY_1P:
            game_reset_state();
            state = STATE_PLAYING;
            break;
        case MENU_MAIN_PLAY_2P:
            game_reset_state();
            state = STATE_PLAYING;
            break;
        case MENU_MAIN_OPTIONS:
            current_menu = get_options_menu();
            break;
        case MENU_OPTIONS_BACK:
            current_menu = get_main_menu();
            break;
    }
}

void game_update_menu(Pad* pad_1, Pad* pad_2) {
    if (pad_button_pressed(pad_1, PAD_CROSS)) {
        return game_invoke_menu_action(current_menu->items[current_menu->selected].action);
    }

    if (pad_button_pressed(pad_1, PAD_CIRCLE)) {
        if (current_menu->back_action != MENU_NULL) {
            return game_invoke_menu_action(current_menu->back_action);
        }
    }

    if (pad_button_pressed(pad_1, PAD_DOWN)) {
        menu_cycle(current_menu, 1);
    }

    if (pad_button_pressed(pad_1, PAD_UP)) {
        menu_cycle(current_menu, -1);
    }
}

void game_update_pause(Pad* pad_1, Pad* pad_2) {
    if (pad_button_pressed(pad_1, PAD_START)) {
        state = STATE_PLAYING;
        return;
    }

    if (pad_button_pressed(pad_1, PAD_SELECT)) {
        state = STATE_MENU;
        return;
    }
}

bool get_paddle_relative_hit(float paddle_position[2], float* relative_hit) {
    float ball_left = ball_position[0] - BALL_SIZE[0] * 0.5f;
    float ball_right = ball_position[0] + BALL_SIZE[0] * 0.5f;
    float ball_top = ball_position[1] - BALL_SIZE[1] * 0.5f;
    float ball_bottom = ball_position[1] + BALL_SIZE[1] * 0.5f;

    float paddle_left = paddle_position[0] - PADDLE_SIZE[0] * 0.5f;
    float paddle_right = paddle_position[0] + PADDLE_SIZE[0] * 0.5f;
    float paddle_top = paddle_position[1] - PADDLE_SIZE[1] * 0.5f;
    float paddle_bottom = paddle_position[1] + PADDLE_SIZE[1] * 0.5f;

    if (ball_left < paddle_right && ball_right > paddle_left && ball_top < paddle_bottom && ball_bottom > paddle_top) {
        *relative_hit = (ball_position[1] - paddle_top) / PADDLE_SIZE[1];
        return true;
    }

    return false;
}

void game_update_paddle(Pad* pad, float paddle_position[2]) {
    if (pad_button_down(pad, PAD_UP)) {
        paddle_position[1] -= PADDLE_MOVEMENT_SPEED;
    }
    if (pad_button_down(pad, PAD_DOWN)) {
        paddle_position[1] += PADDLE_MOVEMENT_SPEED;
    }

    paddle_position[1] = fminf(fmaxf(paddle_position[1], 0.0f), 1.0f);
}

void game_update_gameplay(Pad* pad_1, Pad* pad_2) {
    if (pad_button_pressed(pad_1, PAD_START)) {
        state = STATE_PAUSE;
        return;
    }

    game_update_paddle(pad_1, paddle_position_1);
    game_update_paddle(pad_2, paddle_position_2);

    ball_position[0] += ball_velocity[0];
    ball_position[1] += ball_velocity[1];

    if (ball_position[0] < 0.f || ball_position[0] > 1.f) {
        score[ball_position[0] < 0.f ? 1 : 0]++;
        snprintf(formatted_score, sizeof(formatted_score), "%d - %d", score[0], score[1]);
        game_reset_ball();
        return;
    }

    if (ball_position[1] < 0.f || ball_position[1] > 1.f) {
        ball_position[1] = fminf(fmaxf(ball_position[1], 0.0f), 1.0f);
        ball_velocity[1] *= -1.f;
    }

    float relative_hit = 0.f;

    if (get_paddle_relative_hit(ball_velocity[0] < 0.0f ? paddle_position_1 : paddle_position_2, &relative_hit)) {
        ball_velocity[0] *= -1 - (PADDLE_HIT_EDGE_FACTOR * fabsf(relative_hit - 0.5f));
        ball_velocity[0] = fminf(fmaxf(ball_velocity[0], -BALL_MAX_HORIZONTAL_VELOCITY), BALL_MAX_HORIZONTAL_VELOCITY);
    }

    #ifdef DEBUG_GAME
        game_debug();
    #endif
}

void game_update(Pad* pad_1, Pad* pad_2) {
    switch (state) {
        case STATE_PLAYING:
            game_update_gameplay(pad_1, pad_2);
            break;
        case STATE_PAUSE:
            game_update_pause(pad_1, pad_2);
            break;
        case STATE_MENU:
            game_update_menu(pad_1, pad_2);
            break;
    }
}

void game_initialise(GSGLOBAL* gs_global, u64* background_colour) {
    *background_colour = GS_SETREG_RGBAQ(0, 0, 0, 0, 0);

    font = load_font(gs_global, "fonts/press-start-2p.bmp");

    current_menu = get_main_menu();

    game_reset_ball();
}
