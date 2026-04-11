#include <gsKit.h>
#include <dmaKit.h>

// will use PAL 640x512 when testing on real PS2
#define GS_WIDTH 640
#define GS_HEIGHT 448
#define GS_MODE GS_MODE_NTSC

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
    GSGLOBAL* gs_global = initialise_graphics();

    u64 background_colour = GS_SETREG_RGBAQ(128, 0, 0, 0, 0);

    while (true) {
        gsKit_clear(gs_global, background_colour);
        gsKit_queue_exec(gs_global);
        gsKit_sync_flip(gs_global);
    }

    return 0;
}
