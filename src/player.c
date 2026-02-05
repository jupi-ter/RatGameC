#include "player.h"
#include "entity.h"
#include "utils.h"
#include "sprite.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

const float air_speed = 2.5f;
const float ground_speed = 2.0f;
const float gravity = 0.3f;
const float jumpHeight = 4.0f;
const int coyoteMax = 6;
const int maxBuffer = 4;

PlayerArray player_array_create(int capacity) {
    PlayerArray arr = {0};
    arr.count = 0;
    arr.capacity = capacity;
    arr.data = malloc(sizeof(Player) * capacity);
    return arr;
}

void player_array_add(PlayerArray *players, Player player) {
    if (players->count + 1 > players->capacity) {
        players->capacity *= 2;
        players->data = realloc(players, sizeof(Player) * players->capacity);
    }
    players->data[players->count++] = player;
}

// this needs to be moved onto a renderables parallel array because it's ugly and because of code reusability
static void animation(Player* player, Entity *entity) {
    entity->frame_counter += entity->image_speed;

    if (entity->frame_counter >= 1.0f) {
        entity->frame_counter -= 1.0f;
        entity->image_index++;
        Sprite currentSprite = sprite_manager_get_sprite(entity->current_sprite_id);
        if (entity->image_index >= currentSprite.total_frames) {
            entity->image_index = 0;
        }
    }
}

static void movement(Player* player, Entity* entity, TileArray *tiles) {
    int moveHor = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
    bool jump_key = IsKeyDown(KEY_UP);
    bool jump_key_released = IsKeyReleased(KEY_UP);
    
    bool is_grounded;

    Rectangle ground_rect = entity->collision.rect_collision.rect;
    ground_rect.y += 1;
    if (check_tile_collision(tiles, ground_rect)) {
        is_grounded = true;
        entity->speed = ground_speed;
    } else {
        is_grounded = false;
        entity->speed = air_speed;
    }

    entity->x += moveHor * entity->speed;
    //e->y += moveVer * e->speed;

    CLAMP(entity->x, 0, GAME_WIDTH - entity->size);
    CLAMP(entity->y, 0, GAME_HEIGHT - entity->size);
}

static void player_update(Player* player, Entity *entity, TileArray *tiles) {
    animation(player, entity);
    movement(player, entity, tiles);
}

void update_players(PlayerArray* players, EntityArray *entities, TileArray *tiles) {
    for (int i = 0; i < players->count; i++) {
        Player *player = &players->data[i];
        Entity *entity = &entities->data[i];
        player_update(player, entity, tiles);
    }
}
