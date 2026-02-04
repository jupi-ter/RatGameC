#include "sprite.h"
#include <stdbool.h>
#include <stdio.h>

static const struct {
    const char* path;
    int total_frames;
} SPRITE_DATA[] = {
    [SPRITE_PLAYER_IDLE] = {"guy_idle", 5},
    [SPRITE_PLAYER_WALK] = {"guy_walk", 5},
    [SPRITE_PLAYER_JUMP] = {"guy_jump", 3},
};

static Sprite sprites[SPRITE_COUNT];
static bool is_loaded[SPRITE_COUNT] = {false};

Sprite sprite_manager_get_sprite(SpriteID id) {
    if (!is_loaded[id]) {
        Sprite* spr = &sprites[id];
        spr->total_frames = SPRITE_DATA[id].total_frames;

        for (int i = 0; i < spr->total_frames; i++) {
            char buffer[SPRITE_PATH_MAX_LENGTH];
            if (spr->total_frames > 1) {
                sprintf(buffer, "%s%s_%d.png", BASE_SPRITE_PATH, SPRITE_DATA[id].path, i);
            } else {
                sprintf(buffer, "%s%s.png", BASE_SPRITE_PATH, SPRITE_DATA[id].path);
            }
            spr->frames[i] = LoadTexture(buffer);
        }
        is_loaded[id] = true;
    }
    
    return sprites[id];
}

void sprite_manager_unload_all() {
    for (int i = 0; i < SPRITE_COUNT; i++) {
        if (is_loaded[i]) {
            Sprite *spr = &sprites[i];
            for (int j = 0; j < spr->total_frames; j++) {
                UnloadTexture(spr->frames[i]);
            }
            spr->total_frames = 0;
            is_loaded[i] = false;
        }
    }
}
