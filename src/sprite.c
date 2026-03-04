#include "sprite.h"
#include <string.h>

static Texture2D atlas_texture;
static int atlas_loaded = 0;

void sprite_manager_load_atlas(const char* atlas_path) {
    atlas_texture = LoadTexture(atlas_path);
    atlas_loaded = 1;
}

const Sprite* sprite_manager_get_sprite(SpriteID id) {
    if (id <= SPRITE_NONE || id >= SPRITE_COUNT) return &SPRITE_ATLAS_DATA[SPRITE_NONE];
    return &SPRITE_ATLAS_DATA[id];
}

Texture2D sprite_manager_get_atlas(void) {
    return atlas_texture;
}

void sprite_manager_unload_all(void) {
    if (atlas_loaded) {
        UnloadTexture(atlas_texture);
        atlas_loaded = 0;
    }
}

SpriteID sprite_get_id_from_name(const char* name) {
    for (int i = 0; i < SPRITE_COUNT; i++) {
        if (SPRITE_NAMES[i] && strcmp(SPRITE_NAMES[i], name) == 0) {
            return i;
        }
    }
    return SPRITE_NONE;
}
