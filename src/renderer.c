#include "renderer.h"

void begin_render(RenderTexture2D renderTarget) {
    BeginTextureMode(renderTarget);
    ClearBackground(WHITE);
}

void end_render(RenderTexture2D renderTarget) {
    EndTextureMode();
        
    float scale_x = (float)SCREEN_WIDTH / GAME_WIDTH;
    float scale_y = (float)SCREEN_HEIGHT / GAME_HEIGHT;
    float scale = MIN(scale_x, scale_y);
    
    float scaled_w = GAME_WIDTH * scale;
    float scaled_h = GAME_HEIGHT * scale;
    
    float offset_x = (SCREEN_WIDTH - scaled_w) / 2;
    float offset_y = (SCREEN_HEIGHT - scaled_h) / 2;
    
    Rectangle source = {
        .x = 0,
        .y = 0,
        .width = GAME_WIDTH,
        .height = -GAME_HEIGHT
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
