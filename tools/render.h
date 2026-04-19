#ifndef __RENDER_H__
#define __RENDER_H__

#include <gsKit.h>
#include <gsToolkit.h>

#include "common.h"

void render_quad(GSGLOBAL* gs_global, float x, float y, float width, float height, u64 colour);

void render_text(GSGLOBAL* gs_global, float x, float y, float scale, GSFONT* font, u64 colour, const char* text);

GSFONT* load_font(GSGLOBAL* gs_global, char* path);

#endif