#include "player.h"
#include "utils.h"
#include "sprite.h"
#include <raylib.h>
#include <stdbool.h>

//const float ground_speed = 2.5f;
//const float move_speed = 2.0f;

static void animation(Entity* e) {
    e->frame_counter += e->image_speed;

    if (e->frame_counter >= 1.0f) {
        e->frame_counter -= 1.0f;
        e->image_index++;
        Sprite currentSprite = sprite_manager_get_sprite(e->current_sprite_id);
        if (e->image_index >= currentSprite.total_frames) {
            e->image_index = 0;
        }
    }
}

static void movement(Entity* e, TileArray *tile_arr) {
    int moveHor = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
    bool jump_key = IsKeyDown(KEY_UP);
    bool jump_key_released = IsKeyReleased(KEY_UP);
    
    /*
    bool is_grounded;

    Rectangle ground_rect = e->collision.rect_collision.rect;
    ground_rect.y += 1;
    if (check_tile_collision(tile_arr, ground_rect)) {
        is_grounded = true;
        e->move_speed = 
    } else {
        is_grounded = false;
    }
    */

    e->x += moveHor * e->speed;
    //e->y += moveVer * e->speed;

    CLAMP(e->x, 0, SCREEN_WIDTH - e->size);
    CLAMP(e->y, 0, SCREEN_HEIGHT - e->size);
}

void player_update(Entity* e, TileArray *tile_arr) {
    animation(e);
    movement(e, tile_arr);
}
