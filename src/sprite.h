#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>
#include "sprite_ids.h"

typedef SpriteAtlasEntry Sprite;

void sprite_manager_load_atlas(const char* atlas_path);
const Sprite* sprite_manager_get_sprite(SpriteID id);
Texture2D sprite_manager_get_atlas(void);
void sprite_manager_unload_all(void);
SpriteID sprite_get_id_from_name(const char* name);

#endif
