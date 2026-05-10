#include "game.h"
#include "pad.h"
#include "render.h"
#include "menu.h"
#include "colours.h"
#include "utils.h"
#include "audio.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#include <time.h>

enum State state = STATE_MENU;
struct Settings settings = {0};
struct Menu* current_menu;
struct Sound* sound_beep;
struct Sound* sound_end;
struct Sound* current_sound;
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
char subtitle[16] = {0};
int remaining_time = 0;
clock_t last_second_tick = 0;

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
    scr_printf("REMAINING TIME: %d\n", remaining_time);
}
#endif

void game_play_audio(struct Sound* sound) {
    if (current_sound) {
        audio_stop(current_sound);
    }

    if (settings.audio && sound) {
        current_sound = sound;
        audio_play(sound);
    }
}

void game_update_audio() {
    if (settings.audio && current_sound) {
        audio_update(current_sound);
    }
}

void game_load_settings() {
    settings.audio = menu_choice_selected_option(&menu_choices_audio)->value == MENU_VALUE_AUDIO_ON;
    settings.game_mode = menu_choice_selected_option(&menu_choices_mode)->value;

    if (settings.game_mode == MENU_VALUE_MODE_TIME) {
        settings.time_limit = get_seconds_from_time(menu_choice_selected_option(&menu_choices_time_limit)->name);

        if (settings.time_limit == -1) {
            settings.time_limit = 90;
        }

    } else if (settings.game_mode == MENU_VALUE_MODE_SCORE) {
        settings.score_limit = atoi(menu_choice_selected_option(&menu_choices_score_limit)->name);

        if (settings.score_limit == 0) {
            settings.score_limit = 5;
        }

        snprintf(subtitle, sizeof(subtitle), "FIRST TO %d", settings.score_limit);

    } else {
        subtitle[0] = '\0';
    }
}

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
    game_load_settings();
    game_reset_ball();
    game_play_audio(NULL);

    remaining_time = settings.time_limit;
    last_second_tick = clock();

    paddle_position_1[1] = 0.5f;
    paddle_position_2[1] = 0.5f;

    score[0] = 0;
    score[1] = 0;

    snprintf(formatted_score, sizeof(formatted_score), "0 - 0");
}

void game_render_menu(GSGLOBAL* gs_global) {
    if (current_menu->flags & MENU_FLAG_RENDER_CUSTOM && current_menu->custom_render_callback) {
        current_menu->custom_render_callback(gs_global);
    }

    const float MENU_ITEM_FONT_SCALE = 2.0f;
    const float MENU_VERTICAL_STEP = 0.1f;
    const float MENU_VERTICAL_INFO_OFFSET = 0.045f;

    float y = 0.5f - (current_menu->size / 2) * MENU_VERTICAL_STEP;

    for (int i = 0; i < current_menu->size; i++) {
        const struct MenuItem* item = &current_menu->items[i];
        bool selected = current_menu->selected == i;

        u64 colour = COLOUR_MENU_ITEM_DEFAULT;
        if (item->disabled) {
            colour = COLOUR_MENU_ITEM_DISABLED;
        } else if (selected) {
            colour = COLOUR_MENU_ITEM_SELECTED;
        }

        render_text(gs_global, 0.5f, y, MENU_ITEM_FONT_SCALE, font, colour, item->name);

        char info_buffer[64] = {0};
        const char* info_text = NULL;

        if (item->choices && !item->disabled) {
            snprintf(info_buffer, sizeof(info_buffer), "<< %s >>", menu_item_selected_option(item)->name);
            info_text = info_buffer;
        } else if (selected) {
            info_text = item->info;
        }

        if (info_text) {
            render_text(gs_global, 0.5f, y + MENU_VERTICAL_INFO_OFFSET, 1.0f, font, selected ? COLOUR_MENU_ITEM_HELP : COLOUR_MENU_ITEM_DISABLED, info_text);
        }

        y += MENU_VERTICAL_STEP;
    }
}

void game_render_pause(GSGLOBAL* gs_global) {
    render_text(gs_global, 0.5f, 0.45f, 4.0f, font, COLOUR_RED, "PAUSED");
    render_text(gs_global, 0.5f, 0.53f, 2.0f, font, COLOUR_DARK_GREY, "[START] RESUME");
    render_text(gs_global, 0.5f, 0.585f, 2.0f, font, COLOUR_DARK_GREY, "[SELECT] RETURN TO MENU");
}

void game_render_gameplay(GSGLOBAL* gs_global) {
    render_text(gs_global, 0.5f, 0.1f, 3.0f, font, COLOUR_GREY, formatted_score);

    if (*subtitle) {
        render_text(gs_global, 0.5f, 0.165f, 1.0f, font, COLOUR_DARK_GREY, subtitle);
    }

    render_quad(gs_global, paddle_position_1[0], paddle_position_1[1], PADDLE_SIZE[0], PADDLE_SIZE[1], COLOUR_GAME_FOREGROUND);
    render_quad(gs_global, paddle_position_2[0], paddle_position_2[1], PADDLE_SIZE[0], PADDLE_SIZE[1], COLOUR_GAME_FOREGROUND);
    render_quad(gs_global, ball_position[0], ball_position[1], BALL_SIZE[0], BALL_SIZE[1], COLOUR_GAME_FOREGROUND);
}

void game_render_end(GSGLOBAL* gs_global) {
    const char* winner_text = "TIE";

    if (score[0] > score[1]) {
        winner_text = "PLAYER 1 WINS!";
    } else if (score[1] > score[0]) {
        winner_text = "PLAYER 2 WINS!";
    }

    render_text(gs_global, 0.5f, 0.45f, 4.0f, font, COLOUR_GREEN, winner_text);
    render_text(gs_global, 0.5f, 0.53f, 2.0f, font, COLOUR_DARK_GREY, "[START] PLAY AGAIN");
    render_text(gs_global, 0.5f, 0.585f, 2.0f, font, COLOUR_DARK_GREY, "[SELECT] RETURN TO MENU");
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
        case STATE_END:
            game_render_gameplay(gs_global);
            game_render_end(gs_global);
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
            current_menu = &menu_options;
            break;
        case MENU_OPTIONS_BACK:
            current_menu = &menu_main;
            break;
        case MENU_OPTIONS_OFFSETS:
            current_menu = &menu_offsets;
            break;
        case MENU_OFFSETS_BACK:
            current_menu = &menu_options;
            break;
        default:
            break;
    }
}

void game_update_menu_dependencies(Pad* pad_1, Pad* pad_2) {
    if (current_menu == &menu_options) {
        enum MenuValue game_mode = menu_choice_selected_option(&menu_choices_mode)->value;

        menu_find_item(current_menu, MENU_OPTIONS_TIME_LIMIT)->disabled = game_mode != MENU_VALUE_MODE_TIME;
        menu_find_item(current_menu, MENU_OPTIONS_END_SCORE)->disabled = game_mode != MENU_VALUE_MODE_SCORE;

    } else if (current_menu == &menu_main) {
        menu_find_item(current_menu, MENU_MAIN_PLAY_2P)->disabled = pad_2->state != PAD_STATE_STABLE;
    }
}

void game_update_menu(Pad* pad_1, Pad* pad_2) {
    game_update_menu_dependencies(pad_1, pad_2);

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

    if (pad_button_pressed(pad_1, PAD_LEFT)) {
        menu_cycle_choice(&current_menu->items[current_menu->selected], -1);
    }

    if (pad_button_pressed(pad_1, PAD_RIGHT)) {
        menu_cycle_choice(&current_menu->items[current_menu->selected], 1);
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

    if (settings.game_mode == MENU_VALUE_MODE_TIME) {
        clock_t now = clock();

        if (now - last_second_tick >= CLOCKS_PER_SEC) {
            last_second_tick += CLOCKS_PER_SEC;

            if (--remaining_time <= 0) {
                game_play_audio(sound_end);
                state = STATE_END;
                return;
            }
        }

        snprintf(subtitle, sizeof(subtitle), "%s", get_time_from_seconds(remaining_time));
    }

    game_update_paddle(pad_1, paddle_position_1);
    game_update_paddle(pad_2, paddle_position_2);

    ball_position[0] += ball_velocity[0];
    ball_position[1] += ball_velocity[1];

    if (ball_position[0] < 0.f || ball_position[0] > 1.f) {
        int scoring_player = ball_position[0] < 0.f ? 1 : 0;

        score[scoring_player]++;

        snprintf(formatted_score, sizeof(formatted_score), "%d - %d", score[0], score[1]);

        game_reset_ball();

        if (settings.game_mode == MENU_VALUE_MODE_SCORE && score[scoring_player] >= settings.score_limit) {
            game_play_audio(sound_end);
            state = STATE_END;
        }

        return;
    }

    if (ball_position[1] < 0.f || ball_position[1] > 1.f) {
        ball_position[1] = fminf(fmaxf(ball_position[1], 0.0f), 1.0f);
        ball_velocity[1] *= -1.f;
    }

    float relative_hit = 0.f;

    if (get_paddle_relative_hit(ball_velocity[0] < 0.0f ? paddle_position_1 : paddle_position_2, &relative_hit)) {
        game_play_audio(sound_beep);

        ball_velocity[0] *= -1 - (PADDLE_HIT_EDGE_FACTOR * fabsf(relative_hit - 0.5f));
        ball_velocity[0] = fminf(fmaxf(ball_velocity[0], -BALL_MAX_HORIZONTAL_VELOCITY), BALL_MAX_HORIZONTAL_VELOCITY);
    }

    #ifdef DEBUG_GAME
        game_debug();
    #endif
}

void game_update_end(Pad* pad_1, Pad* pad_2) {
    if (pad_button_pressed(pad_1, PAD_START)) {
        game_reset_state();
        state = STATE_PLAYING;
        return;
    }

    if (pad_button_pressed(pad_1, PAD_SELECT)) {
        state = STATE_MENU;
        return;
    }
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
        case STATE_END:
            game_update_end(pad_1, pad_2);
            break;
    }

    game_update_audio();
}

void game_initialise(GSGLOBAL* gs_global, u64* background_colour) {
    *background_colour = COLOUR_BLACK;

    font = load_font(gs_global, "fonts/press-start-2p.bmp");
    sound_beep = audio_load("sounds/beep.wav");
    sound_end = audio_load("sounds/evil-laughter.wav");

    current_menu = &menu_main;
}

void game_shutdown() {
    audio_free(sound_beep);
    audio_free(sound_end);
}
