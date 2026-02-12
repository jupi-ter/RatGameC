#ifndef RENDERER_H
#define RENDERER_H

#include <raylib.h>

void begin_render(RenderTexture2D renderTarget);
void end_render(int screen_w, int screen_h, int game_w, int game_h, RenderTexture2D renderTarget);

#endif