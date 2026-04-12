#include "pad.h"

void pad_open(Pad* pad, int port, int slot) {
    pad->port = port;
    pad->slot = slot;
    pad->buttons = (struct padButtonStatus){0};

    padPortOpen(port, slot, pad->buffer);
}

void pad_update(Pad* pad) {
    pad->state = padGetState(pad->port, pad->slot);

    if (pad->state == PAD_STATE_STABLE) {
        padRead(pad->port, pad->slot, &pad->buttons);
    }
}

bool pad_button_down(Pad* pad, int button) {
    return pad->buttons.btns & button;
}
