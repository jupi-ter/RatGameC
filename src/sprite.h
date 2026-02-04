#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>

#define BASE_SPRITE_PATH "assets/sprites/"
#define SPRITE_PATH_MAX_LENGTH 64
#define MAX_FRAMES 16

typedef enum SpriteID {
    SPRITE_PLAYER_IDLE,
    SPRITE_PLAYER_WALK,
    SPRITE_PLAYER_JUMP,
    SPRITE_COUNT
} SpriteID;

typedef struct Sprite {
    Texture2D frames[MAX_FRAMES];
    int total_frames;
} Sprite;

Sprite sprite_manager_get_sprite(SpriteID id);
void sprite_manager_unload_all();

#endif
