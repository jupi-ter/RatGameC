#include "player.h"
#include "entity.h"
#include "sprite.h"
#include "tile.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

const float air_speed = 2.5f;
const float ground_speed = 2.0f;
const float gravity = 0.3f;
const float jump_height = 4.0f;
const int coyote_max = 6;
const int max_jump_buffer = 4;

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
        players->data = realloc(players->data, sizeof(Player) * players->capacity);
    }
    players->data[players->count++] = player;
}

// this needs to be moved onto a renderables parallel array because it's ugly and because of code reusability
static void animation(Entity *entity) {
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

static void change_animations(Player *player, Entity *entity, int move_hor) {
    if (player->is_grounded) {
        if (move_hor == 0) {
            entity_change_sprite_data(entity, SPRITE_PLAYER_IDLE, 0.3);
        } else {
            entity_change_sprite_data(entity, SPRITE_PLAYER_WALK, 0.3);
        }
    } else {
        entity_change_sprite_data(entity, SPRITE_PLAYER_JUMP, 0.5);
    }
}

static void update_horizontal_movement(Player* player, Entity* entity, TileArray *tiles, int move_hor) {
    // update grounded state
    Rectangle ground_rect = entity->collision.rect_collision.rect;
    ground_rect.y += 1;
    if (check_tile_collision(tiles, ground_rect)) {
        player->is_grounded = true;
        player->move_speed = ground_speed;
    } else {
        player->is_grounded = false;
        player->move_speed = air_speed;
    }

    player->hsp = move_hor * player->move_speed; // + recoil
    
    //this is the strafe and recoil snippet, implement later
    /*if (move_hor != 0) {
        if (!IsKeyDown(KEY_SPACE) || !IsKeyPressed(KEY_SPACE)) {
            
        }
    }*/

    //CLAMP(entity->x, 0, GAME_WIDTH - entity->size);
    //CLAMP(entity->y, 0, GAME_HEIGHT - entity->size);
}

static void jump(Player *player, Entity *entity) {
    player->vsp = -jump_height;
    player->buffer_counter = 0;
}

static void update_vertical_movement(Player *player, Entity *entity, TileArray *tiles) {
    bool jump_key = IsKeyDown(KEY_UP);
    bool jump_key_released = IsKeyReleased(KEY_UP);

    if (!player->is_grounded) {
        player->vsp += gravity;

        if (player->coyote_counter > 0) {
            player->coyote_counter--;
            if (jump_key)
                jump(player, entity);
        }
    } else {
        player->coyote_counter = coyote_max;
    }

    Rectangle ground_rect = entity->collision.rect_collision.rect;
    ground_rect.y -= jump_height;
    if (jump_key && !check_tile_collision(tiles, ground_rect)) {
        player->buffer_counter = max_jump_buffer;
    }

    if (player->buffer_counter > 0) {
        player->buffer_counter--;
        if (player->is_grounded)
            jump(player, entity);
    }

    if (jump_key_released && player->vsp > 0) {
        player->vsp *= 0.5f;
    }
}

static void check_collisions_and_move(Player *player, Entity *entity, TileArray* tiles) {
    float px_x = (player->hsp > 0) ? 1 : -1;

    Rectangle test_hor_rect = entity->collision.rect_collision.rect;
    test_hor_rect.x += px_x;
    if (check_tile_collision(tiles, test_hor_rect)) {
        while (!check_tile_collision(tiles, test_hor_rect)) {
            entity->x+=px_x;
        }
        player->hsp = 0;
    }

    entity->x += player->hsp;

    //this is ugly as fuck
    Rectangle player_rect_x = {
        .x = (int)entity->x,
        .y = (int)entity->y,
        .width = 10,
        .height = 10
    };

    RectWrapper player_collision_x = {
        .owner_id = entity->id,
        .rect = player_rect_x
    };

    entity->collision.rect_collision = player_collision_x;

    float px_y = (player->vsp > 0) ? 1 : -1;
    Rectangle test_ver_rect = entity->collision.rect_collision.rect;
    test_ver_rect.y += px_y;
    if (check_tile_collision(tiles, test_ver_rect)) {
        while (!check_tile_collision(tiles, test_ver_rect)) {
            entity->y+=px_y;
        }
        player->vsp = 0;
    }

    entity->y += player->vsp;

    //this is ugly as fuck
    Rectangle player_rect_y = {
        .x = (int)entity->x,
        .y = (int)entity->y,
        .width = 10,
        .height = 10
    };

    RectWrapper player_collision_y = {
        .owner_id = entity->id,
        .rect = player_rect_x
    };

    entity->collision.rect_collision = player_collision_y;
}

static void player_update(Player* player, Entity *entity, TileArray *tiles) {
    int move_hor = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
    animation(entity);
    update_horizontal_movement(player, entity, tiles, move_hor);
    change_animations(player, entity, move_hor);
    update_vertical_movement(player, entity, tiles);
    check_collisions_and_move(player, entity, tiles);
}

void update_players(PlayerArray* players, EntityArray *entities, TileArray *tiles) {
    for (int i = 0; i < players->count; i++) {
        Player *player = &players->data[i];
        Entity *entity = &entities->data[i];
        player_update(player, entity, tiles);
    }
}
