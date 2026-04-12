#include "common.h"
#include "render.h"

void render_quad(GSGLOBAL* gs_global, float x, float y, float width, float height, u64 colour) {
    gsKit_prim_sprite(gs_global, x * GS_WIDTH, y * GS_HEIGHT, (x + width) * GS_WIDTH, (y + height) * GS_HEIGHT, 0, colour);
}
