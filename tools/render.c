#include "common.h"
#include "render.h"

void render_quad(GSGLOBAL* gs_global, float x, float y, float width, float height, u64 colour) {
    gsKit_prim_sprite(
        gs_global,
        (x - width * 0.5f) * GS_WIDTH,
        (y - height * 0.5f) * GS_HEIGHT,
        (x + width * 0.5f) * GS_WIDTH,
        (y + height * 0.5f) * GS_HEIGHT,
        0,
        colour
    );
}
