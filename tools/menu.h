#ifndef __MENU_H__
#define __MENU_H__

#include <stdbool.h>

enum MenuAction {
    MENU_NULL,

    MENU_MAIN_PLAY_1P,
    MENU_MAIN_PLAY_2P,
    MENU_MAIN_OPTIONS,

    MENU_OPTIONS_MODE,
    MENU_OPTIONS_TIME_LIMIT,
    MENU_OPTIONS_END_SCORE,
    MENU_OPTIONS_DIFFICULTY,
    MENU_OPTIONS_OFFSETS,
    MENU_OPTIONS_BACK
};

struct MenuItem {
    const char* name;
    enum MenuAction action;
    bool enabled;
};

struct Menu {
    int selected;
    int size;
    enum MenuAction back_action;
    struct MenuItem items[];
};

void menu_cycle(struct Menu* menu, int direction);

struct Menu* get_main_menu();

struct Menu* get_options_menu();

#endif
