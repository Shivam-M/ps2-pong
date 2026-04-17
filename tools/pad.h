#ifndef __PAD_H__
#define __PAD_H__

#include <libpad.h>
#include <stdbool.h>

typedef struct {
    struct padButtonStatus buttons;
    struct padButtonStatus previous_buttons;
    int state;
    int port;
    int slot;
    unsigned char buffer[256] __attribute__((aligned(64)));
} Pad;

void pad_open(Pad* pad, int port, int slot);
void pad_update(Pad* pad);
bool pad_button_down(Pad* pad, int button);
bool pad_button_pressed(Pad* pad, int button);
bool pad_button_released(Pad* pad, int button);

#endif
