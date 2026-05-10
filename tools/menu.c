#include "menu.h"
#include "render.h"
#include "colours.h"
#include "common.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

struct MenuChoice menu_choices_offsets_horizontal = {
    .selected = 5,
    .options = {
        {"-10", MENU_NULL},
        {"-8", MENU_NULL},
        {"-6", MENU_NULL},
        {"-4", MENU_NULL},
        {"-2", MENU_NULL},
        {"0", MENU_NULL},
        {"2", MENU_NULL},
        {"4", MENU_NULL},
        {"6", MENU_NULL},
        {"8", MENU_NULL},
        {"10", MENU_NULL},
    },
    .size = 11
};

struct MenuChoice menu_choices_offsets_vertical = {
    .selected = 5,
    .options = {
        {"-10", MENU_NULL},
        {"-8", MENU_NULL},
        {"-6", MENU_NULL},
        {"-4", MENU_NULL},
        {"-2", MENU_NULL},
        {"0", MENU_NULL},
        {"2", MENU_NULL},
        {"4", MENU_NULL},
        {"6", MENU_NULL},
        {"8", MENU_NULL},
        {"10", MENU_NULL},
    },
    .size = 11
};

void render_offsets_menu(GSGLOBAL* gs_global) {
    render_quad(gs_global, 0.5f, 0.5f, 1.00f, 1.00f, COLOUR_RED);
    render_quad(gs_global, 0.5f, 0.5f, 0.95f, 0.95f, COLOUR_GREEN);
    render_quad(gs_global, 0.5f, 0.5f, 0.90f, 0.90f, COLOUR_BLUE);
    render_quad(gs_global, 0.5f, 0.5f, 0.85f, 0.85f, COLOUR_BLACK);
}

void on_change_offsets_menu(struct MenuItem* item) {
    if (item->action == MENU_OFFSETS_HORIZONTAL) {
        render_offsets[0] = atoi(menu_item_selected_option(item)->name);
    } else if (item->action == MENU_OFFSETS_VERTICAL) {
        render_offsets[1] = atoi(menu_item_selected_option(item)->name);
    }
}

static struct Menu offsets_menu = {
    .selected = 0,
    .items = {
        {"HORIZONTAL", MENU_OFFSETS_HORIZONTAL, .choices = &menu_choices_offsets_horizontal, .on_change = &on_change_offsets_menu, .flags = MENU_FLAG_NO_CYCLE},
        {"VERTICAL", MENU_OFFSETS_VERTICAL, .choices = &menu_choices_offsets_vertical, .on_change = &on_change_offsets_menu, .flags = MENU_FLAG_NO_CYCLE},
        {"BACK", MENU_OFFSETS_BACK, .info = "Return to the options menu"}
    },
    .back_action = MENU_OFFSETS_BACK,
    .flags = MENU_FLAG_RENDER_CUSTOM,
    .custom_render_callback = &render_offsets_menu,
    .size = 3
};

const struct MenuOption NULL_OPTION = {"NULL", MENU_VALUE_NULL};

const struct MenuOption* menu_item_selected_option(const struct MenuItem* item) {
    return menu_choice_selected_option(item->choices);
}

const struct MenuOption* menu_choice_selected_option(const struct MenuChoice* choice) {
    if (!choice) return &NULL_OPTION;

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

    if (item->flags & MENU_FLAG_NO_CYCLE) {
        item->choices->selected = MAX(0, MIN(item->choices->selected + direction, item->choices->size - 1));
    } else {
        item->choices->selected = (item->choices->selected + direction + item->choices->size) % item->choices->size;
    }

    if (item->on_change) {
        item->on_change(item);
    }
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

struct Menu* get_offsets_menu() {
    return &offsets_menu;
}
