#include "menu.h"

static struct Menu main_menu = {
    .selected = 1,
    .items = {
        {"SINGLEPLAYER", MENU_MAIN_PLAY_1P, .enabled = false},
        {"MULTIPLAYER", MENU_MAIN_PLAY_2P, .enabled = true},
        {"OPTIONS", MENU_MAIN_OPTIONS, .enabled = true}
    },
    .back_action = MENU_NULL,
    .size = 3
};

static struct Menu options_menu = {
    .selected = 0,
    .items = {
        {"GAME MODE", MENU_OPTIONS_MODE, .enabled = true},
        {"TIME LIMIT", MENU_OPTIONS_TIME_LIMIT, .enabled = false},
        {"SCORE LIMIT", MENU_OPTIONS_END_SCORE, .enabled = true},
        {"DIFFICULTY", MENU_OPTIONS_DIFFICULTY, .enabled = false},
        {"OFFSETS", MENU_OPTIONS_OFFSETS, .enabled = true},
        {"BACK", MENU_OPTIONS_BACK, .enabled = true}
    },
    .back_action = MENU_OPTIONS_BACK,
    .size = 6
};

// should really validate that all items aren't disabled (even if it is very unlikely)
void menu_cycle(struct Menu* menu, int direction) {
    do {
        menu->selected = (menu->selected + direction + menu->size) % menu->size;
    } while (!menu->items[menu->selected].enabled);
}

struct Menu* get_main_menu() {
    return &main_menu;
}

struct Menu* get_options_menu() {
    return &options_menu;
}
