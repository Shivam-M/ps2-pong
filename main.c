#include <gsKit.h>
#include <dmaKit.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>
#include <debug.h>

#include "common.h"
#include "tools/pad.h"
#include "tools/render.h"

// #define DEBUG_PADS

Pad pad_1;
Pad pad_2;

float ball_position[2] = {0.45, 0.45};

void render(GSGLOBAL* gs_global) {
    render_quad(gs_global, 0.0, 0.0, 0.5f, 0.5f, GS_SETREG_RGBAQ(128, 0, 0, 0, 0));
    render_quad(gs_global, 0.5f, 0.f, 0.5f, 0.5f, GS_SETREG_RGBAQ(0, 128, 0, 0, 0));
    render_quad(gs_global, 0.f, 0.5f, 0.5f, 0.5f, GS_SETREG_RGBAQ(0, 0, 128, 0, 0));
    render_quad(gs_global, 0.5f, 0.5f, 0.5f, 0.5f, GS_SETREG_RGBAQ(0, 128, 128, 0, 0));
    render_quad(gs_global, ball_position[0], ball_position[1], 0.1f, 0.1f, GS_SETREG_RGBAQ(255, 255, 255, 0, 0));
}

void update() {
    if (pad_button_down(&pad_2, PAD_LEFT)) {
        ball_position[0] += 0.01f;
    }
    if (pad_button_down(&pad_2, PAD_RIGHT)) {
        ball_position[0] -= 0.01f;
    }
    if (pad_button_down(&pad_2, PAD_UP)) {
        ball_position[1] += 0.01f;
    }
    if (pad_button_down(&pad_2, PAD_DOWN)) {
        ball_position[1] -= 0.01f;
    }
}

#ifdef DEBUG_PADS
void debug_pads() {
    scr_setXY(0, 0);
    scr_printf("TEST D-PAD UP IS PRESSED?: %d\n", pad_button_down(&pad_2, PAD_UP));
}
#endif

void update_pads() {
    pad_update(&pad_1);
    pad_update(&pad_2);
}

void initialise_pads() {
    SifInitRpc(0);

    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN", 0, NULL);

    padInit(0);

    pad_open(&pad_1, 0, 0);
    pad_open(&pad_2, 1, 0);
}

GSGLOBAL* initialise_graphics() {
    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC, D_CTRL_STD_OFF, D_CTRL_RCYC_8, 0);
    dmaKit_chan_init(DMA_CHANNEL_GIF);

    GSGLOBAL* gs_global = gsKit_init_global();
    gs_global->Mode = GS_MODE;
    gs_global->Width = GS_WIDTH;
    gs_global->Height = GS_HEIGHT;

    gsKit_init_screen(gs_global);

    return gs_global;
}

int main() {
    init_scr();

    initialise_pads();

    GSGLOBAL* gs_global = initialise_graphics();

    u64 background_colour = GS_SETREG_RGBAQ(128, 128, 128, 0, 0);

    while (true) {
        update_pads();

#ifdef DEBUG_PADS
        debug_pads();
#endif

        gsKit_clear(gs_global, background_colour);

        update();
        render(gs_global);

        gsKit_queue_exec(gs_global);
        gsKit_sync_flip(gs_global);
    }

    return 0;
}
