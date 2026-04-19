#include "common.h"
#include "render.h"

#include <gsKit.h>
#include <gsToolkit.h>
#include <string.h>

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

void render_text(GSGLOBAL* gs_global, float x, float y, float scale, GSFONT* font, u64 colour, const char* text) {
    float width = strlen(text) * ((float)font->CharWidth / GS_WIDTH) * scale / 2;
    float height = ((float)font->CharHeight / GS_HEIGHT) * scale;

    gsKit_font_print_scaled(
        gs_global,
        font,
        (x - width * 0.5f) * GS_WIDTH,
        (y - height * 0.5f) * GS_HEIGHT,
        0,
        scale,
        colour,
        text
    );
}

GSFONT* load_font(GSGLOBAL* gs_global, char* path) {
    GSFONT* font = gsKit_init_font(GSKIT_FTYPE_BMP_DAT, path);
    gsKit_font_upload(gs_global, font);
    return font;
}
