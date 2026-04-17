#include "pad.h"

void pad_open(Pad* pad, int port, int slot) {
    pad->port = port;
    pad->slot = slot;
    pad->buttons = (struct padButtonStatus){0};
    pad->previous_buttons = (struct padButtonStatus){0};

    padPortOpen(port, slot, pad->buffer);
}

void pad_update(Pad* pad) {
    pad->state = padGetState(pad->port, pad->slot);

    if (pad->state == PAD_STATE_STABLE) {
        pad->previous_buttons = pad->buttons;
        padRead(pad->port, pad->slot, &pad->buttons);
    }
}

bool pad_button_down(Pad* pad, int button) {
    return !(pad->buttons.btns & button);
}

bool pad_button_pressed(Pad* pad, int button) {
    return !(pad->buttons.btns & button) && (pad->previous_buttons.btns & button);
}

bool pad_button_released(Pad* pad, int button) {
    return (pad->buttons.btns & button) && !(pad->previous_buttons.btns & button);
}
