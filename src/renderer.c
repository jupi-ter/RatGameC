#include "renderer.h"
#include "raylib.h"
#include "utils.h"

void begin_render(RenderTexture2D renderTarget) {
    BeginTextureMode(renderTarget);
    ClearBackground(WHITE);
}

void end_render(int screen_w, int screen_h, int game_w, int game_h ,RenderTexture2D renderTarget) {
    EndTextureMode();
        
    float scale_x = (float)screen_w / game_w;
    float scale_y = (float)screen_h / game_h;
    float scale = MIN(scale_x, scale_y);
    
    float scaled_w = game_w * scale;
    float scaled_h = game_h * scale;
    
    float offset_x = (screen_w - scaled_w) / 2;
    float offset_y = (screen_h - scaled_h) / 2;
    
    Rectangle source = {
        .x = 0,
        .y = 0,
        .width = game_w,
        .height = -game_h
    };

    Rectangle dest = {
        .x = offset_x,
        .y = offset_y,
        .width = scaled_w,
        .height = scaled_h
    };        

    Vector2 origin = {
        .x = 0.0f,
        .y = 0.0f
    };
    DrawTexturePro(renderTarget.texture, source, dest, origin, 0.0f, WHITE);
}
