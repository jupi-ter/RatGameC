#include "sprite.h"
#include "stdlib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const struct {
    const char* path;
    int total_frames;
} SPRITE_DATA[] = {
    [SPRITE_WALL] = {"wall", 1},
    [SPRITE_PLAYER] = {"rat", 2},
};

static const char* TILE_NAMES[] = {
    [SPRITE_NONE] = "none",
    [SPRITE_WALL] = "wall",
    [SPRITE_PLAYER] = NULL,
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

void sprite_manager_unload_all(void) {
    for (int i = 0; i < SPRITE_COUNT; i++) {
        if (is_loaded[i]) {
            Sprite *spr = &sprites[i];
            for (int j = 0; j < spr->total_frames; j++) {
                UnloadTexture(spr->frames[j]);
            }
            spr->total_frames = 0;
            is_loaded[i] = false;
        }
    }
}

SpriteID sprite_get_id_from_name(char* name) {
    for (int i = 0; i < SPRITE_COUNT; i++) {
        if (strcmp(TILE_NAMES[i], name) == 0) {
            return i;
        }
    }
    return SPRITE_NONE;
}
