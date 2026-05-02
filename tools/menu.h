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
    MENU_OPTIONS_AUDIO,
    MENU_OPTIONS_BACK
};

enum MenuValue {
    MENU_VALUE_NULL,

    MENU_VALUE_MODE_SCORE,
    MENU_VALUE_MODE_TIME,
    MENU_VALUE_MODE_UNLIMITED,

    MENU_VALUE_SCORE_LIMIT_1,
    MENU_VALUE_SCORE_LIMIT_2,
    MENU_VALUE_SCORE_LIMIT_3,
    MENU_VALUE_SCORE_LIMIT_4,
    MENU_VALUE_SCORE_LIMIT_5,

    MENU_VALUE_TIME_LIMIT_1,
    MENU_VALUE_TIME_LIMIT_2,
    MENU_VALUE_TIME_LIMIT_3,
    MENU_VALUE_TIME_LIMIT_4,
    MENU_VALUE_TIME_LIMIT_5,

    MENU_VALUE_DIFFICULTY_EASY,
    MENU_VALUE_DIFFICULTY_MEDIUM,
    MENU_VALUE_DIFFICULTY_HARD,

    MENU_VALUE_AUDIO_ON,
    MENU_VALUE_AUDIO_OFF
};

struct MenuItem {
    const char* name;
    enum MenuAction action;
    bool disabled;
    const char* info;
    struct MenuChoice* choices;
};

struct MenuOption {
    const char* name;
    enum MenuValue value;
};

struct MenuChoice {
    int selected;
    int size;
    struct MenuOption options[];
};

struct Menu {
    int selected;
    int size;
    enum MenuAction back_action;
    struct MenuItem items[];
};

extern struct MenuChoice menu_choices_mode;
extern struct MenuChoice menu_choices_difficulty;
extern struct MenuChoice menu_choices_audio;

const struct MenuOption* menu_item_selected_option(const struct MenuItem* item);

enum MenuValue menu_choice_selected_value(const struct MenuChoice* choice);

struct MenuItem* menu_find_item(struct Menu* menu, enum MenuAction action);

void menu_cycle_choice(struct MenuItem* item, int direction);

void menu_cycle(struct Menu* menu, int direction);

struct Menu* get_main_menu();

struct Menu* get_options_menu();

#endif
