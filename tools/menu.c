#include "menu.h"

#include <stdio.h>

static struct Menu main_menu = {
    .selected = 1,
    .items = {
        {"SINGLEPLAYER", MENU_MAIN_PLAY_1P, .disabled = true, .info = "Play vs AI - adjust difficulty in options"},
        {"MULTIPLAYER", MENU_MAIN_PLAY_2P, .info = "Local 1v1 - requires two controllers"},
        {"OPTIONS", MENU_MAIN_OPTIONS, .info = "Change game mode, difficulty, and more"}
    },
    .back_action = MENU_NULL,
    .size = 3
};

struct MenuChoice menu_choices_mode = {
    .selected = 0,
    .options = {
        {"SCORE LIMIT", MENU_VALUE_MODE_SCORE},
        {"TIME LIMIT", MENU_VALUE_MODE_TIME},
        {"FREE PLAY", MENU_VALUE_MODE_UNLIMITED}
    },
    .size = 3
};

struct MenuChoice menu_choices_time_limit = {
    .selected = 0,
    .options = {
        {"00:30", MENU_VALUE_TIME_LIMIT_1},
        {"01:00", MENU_VALUE_TIME_LIMIT_2},
        {"01:45", MENU_VALUE_TIME_LIMIT_3},
        {"02:30", MENU_VALUE_TIME_LIMIT_4},
        {"05:00", MENU_VALUE_TIME_LIMIT_5},
    },
    .size = 5
};

struct MenuChoice menu_choices_score_limit = {
    .selected = 0,
    .options = {
        {"5", MENU_VALUE_SCORE_LIMIT_1},
        {"10", MENU_VALUE_SCORE_LIMIT_2},
        {"15", MENU_VALUE_SCORE_LIMIT_3},
        {"20", MENU_VALUE_SCORE_LIMIT_4},
        {"25", MENU_VALUE_SCORE_LIMIT_5},
    },
    .size = 5
};

struct MenuChoice menu_choices_difficulty = {
    .selected = 1,
    .options = {
        {"EASY", MENU_VALUE_DIFFICULTY_EASY},
        {"MEDIUM", MENU_VALUE_DIFFICULTY_MEDIUM},
        {"HARD", MENU_VALUE_DIFFICULTY_HARD}
    },
    .size = 3
};

struct MenuChoice menu_choices_audio = {
    .selected = 0,
    .options = {
        {"ENABLED", MENU_VALUE_AUDIO_ON},
        {"DISABLED", MENU_VALUE_AUDIO_OFF}
    },
    .size = 2
};

static struct Menu options_menu = {
    .selected = 0,
    .items = {
        {"GAME MODE", MENU_OPTIONS_MODE, .choices = &menu_choices_mode},
        {"TIME LIMIT", MENU_OPTIONS_TIME_LIMIT, .disabled = true, .choices = &menu_choices_time_limit},
        {"SCORE LIMIT", MENU_OPTIONS_END_SCORE, .choices = &menu_choices_score_limit},
        {"DIFFICULTY", MENU_OPTIONS_DIFFICULTY, .choices = &menu_choices_difficulty},
        {"OFFSETS", MENU_OPTIONS_OFFSETS, .info = "Adjust screen horizontal/vertical offsets"},
        {"AUDIO", MENU_OPTIONS_AUDIO, .choices = &menu_choices_audio},
        {"BACK", MENU_OPTIONS_BACK, .info = "Return to the main menu"}
    },
    .back_action = MENU_OPTIONS_BACK,
    .size = 7
};

const struct MenuOption NULL_OPTION = {"NULL", MENU_VALUE_NULL};

const struct MenuOption* menu_item_selected_option(const struct MenuItem* item) {
    return menu_choice_selected_option(item->choices);
}

const struct MenuOption* menu_choice_selected_option(const struct MenuChoice* choice) {
    if (!choice) return MENU_VALUE_NULL;

    return &choice->options[choice->selected];
}

struct MenuItem* menu_find_item(struct Menu* menu, enum MenuAction action) {
    for (int i = 0; i < menu->size; i++) {
        if (menu->items[i].action == action) {
            return &menu->items[i];
        }
    }

    return NULL;
}

void menu_cycle_choice(struct MenuItem* item, int direction) {
    if (item->choices == NULL) return;

    item->choices->selected = (item->choices->selected + direction + item->choices->size) % item->choices->size;
}

// should really validate that all items aren't disabled (even if it is very unlikely)
void menu_cycle(struct Menu* menu, int direction) {
    do {
        menu->selected = (menu->selected + direction + menu->size) % menu->size;
    } while (menu->items[menu->selected].disabled);
}

struct Menu* get_main_menu() {
    return &main_menu;
}

struct Menu* get_options_menu() {
    return &options_menu;
}
