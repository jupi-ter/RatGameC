#include "player.h"
#include "entity.h"
#include "sprite.h"
#include "tile.h"
#include "timer.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

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

/*static void change_animations(Player *player, Renderable *renderable, int move_hor) {
    if (player->is_grounded) {
        if (move_hor == 0) {
            renderable_change_sprite_data(renderable, SPRITE_PLAYER_IDLE, 0.3);
        } else {
            renderable_change_sprite_data(renderable, SPRITE_PLAYER_WALK, 0.3);
        }
    }// else {
    //    entity_change_sprite_data(entity, SPRITE_PLAYER_JUMP, 0.5);
    //}
}*/

static void update_horizontal_movement(Player* player, transform_t *transform, int move_hor) {
    player->hsp = move_hor * player->move_speed;

    if (move_hor != 0) {
        transform->right = move_hor;
    }
}

//callback
static void stretch(void *context) {
    Player *player = (Player*)context;
    player->target_xscale = 1.0f;
    player->target_yscale = 1.0f;
}

static void jump(Player *player, TimerArray *timers) {
    // squash
    player->target_xscale = 0.7f;
    player->target_yscale = 1.3f;

    player->vsp = -jump_height;
    player->buffer_counter = 0;
    Timer timer = {
        .frame_target = 15,
        .completed = false,
        .counter = 0,
        .callback = stretch,
        .context = player
    };
    timer_manager_add(timers, timer);

}

static void update_vertical_movement(Player *player, Entity *entity, TileGrid *tiles, RectangleArray *rectangles, TimerArray *timers) {
    bool jump_key = IsKeyDown(KEY_UP);
    bool jump_key_released = IsKeyReleased(KEY_UP);

    if (!player->is_grounded) {
        player->vsp += gravity;

        if (player->coyote_counter > 0) {
            player->coyote_counter--;
            if (jump_key)
                jump(player, timers);
        }
    } else {
        player->coyote_counter = coyote_max;
    }

    RectWrapper *ptr_rect = &rectangles->data[entity->id];
    RectWrapper copy_rectwrapper = *ptr_rect;
    copy_rectwrapper.rect.y -= jump_height;
    if (jump_key && !tile_grid_check_collision(tiles, copy_rectwrapper.rect)) {
        player->buffer_counter = max_jump_buffer;
    }

    if (player->buffer_counter > 0) {
        player->buffer_counter--;
        if (player->is_grounded)
            jump(player, timers);
    }

    if (jump_key_released && player->vsp > 0) {
        player->vsp *= 0.5f;
    }
}

static void check_collisions_and_move(Player *player, Entity *entity, transform_t *transform, TileGrid* tiles, RectangleArray *rectangles) {
    RectWrapper *rect = &rectangles->data[entity->id];
    
    transform->x += player->hsp;
    rect->rect.x = (int)transform->x;
    
    if (tile_grid_check_collision(tiles, rect->rect)) {
        float push_dir = (player->hsp > 0) ? -1.0f : 1.0f;
        
        while (tile_grid_check_collision(tiles, rect->rect)) {
            transform->x += push_dir;
            rect->rect.x = (int)transform->x;
        }
        
        player->hsp = 0;
    }
    
    transform->y += player->vsp;
    rect->rect.y = (int)transform->y;
    
    if (tile_grid_check_collision(tiles, rect->rect)) {
        float push_dir = (player->vsp > 0) ? -1.0f : 1.0f;
        
        while (tile_grid_check_collision(tiles, rect->rect)) {
            transform->y += push_dir;
            rect->rect.y = (int)transform->y;
        }
        
        player->vsp = 0;
    }
    
    Rectangle ground_check = {
        .x = rect->rect.x,
        .y = rect->rect.y + 1,
        .width = rect->rect.width,
        .height = rect->rect.height
    };
    
    if (tile_grid_check_collision(tiles, ground_check)) {
        player->is_grounded = true;
        player->move_speed = ground_speed;
    } else {
        player->is_grounded = false;
        player->move_speed = air_speed;
    }
}

static void update_scale(Player *player, transform_t* transform) {
    if (transform->image_xscale != player->target_xscale || transform->image_yscale != player->target_yscale) {
        transform->image_xscale = LERP(transform->image_xscale, player->target_xscale, 0.1f);
        transform->image_yscale = LERP(transform->image_yscale, player->target_yscale, 0.1f);
    }
}

static void player_update(Player* player, Entity *entity, transform_t *transform, Renderable *renderable, TileGrid *tiles, RectangleArray *rectangles, TimerArray *timers) {
    int move_hor = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
    update_horizontal_movement(player, transform, move_hor);
    update_vertical_movement(player, entity, tiles, rectangles, timers);
    check_collisions_and_move(player, entity, transform, tiles, rectangles);
    update_scale(player, transform);
}

void update_players(PlayerArray* players, EntityArray *entities, TransformArray *transforms, RenderableArray *renderables, TileGrid *tiles, RectangleArray *rectangles, TimerArray *timers) {
    for (int i = 0; i < players->count; i++) {
        Player *player = &players->data[i];
        Entity *entity = &entities->data[i];
        transform_t *transform = &transforms->data[i];
        Renderable *renderable = &renderables->data[i];
        player_update(player, entity, transform, renderable, tiles, rectangles, timers);
    }
}
