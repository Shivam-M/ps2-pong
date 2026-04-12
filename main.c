#include <gsKit.h>
#include <dmaKit.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>
#include <debug.h>

#include "tools/pad.h"

// will use PAL 640x512 when testing on real PS2
#define GS_WIDTH 640
#define GS_HEIGHT 448
#define GS_MODE GS_MODE_NTSC

// #define DEBUG_PADS

Pad pad_1;
Pad pad_2;

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

    u64 background_colour = GS_SETREG_RGBAQ(128, 0, 0, 0, 0);

    while (true) {
        update_pads();

#ifdef DEBUG_PADS
        debug_pads();
#endif

        gsKit_clear(gs_global, background_colour);
        gsKit_queue_exec(gs_global);
        gsKit_sync_flip(gs_global);
    }

    return 0;
}
