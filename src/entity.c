#include <raylib.h>
#include "entity.h"
#include <stdlib.h>
#include "sprite.h"
#include "utils.h"
#include "player.h"

EntityArray entity_manager_create(int capacity) {
    EntityArray arr = {0};
    arr.count = 0;
    arr.capacity = capacity;
    arr.data = malloc(sizeof(Entity) * capacity);

    return arr;
}

void entity_manager_add(EntityArray* arr, Entity entity, RectangleArray *rect_arr, CircleArray *circ_arr) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Entity) * arr->capacity);
    }

    if (entity.collision_shape == COLLISION_RECT) {
        rect_array_add(rect_arr, entity.collision.rect_collision);
    } else if (entity.collision_shape == COLLISION_CIRC) {
        circ_array_add(circ_arr, entity.collision.circle_collision);
    }

    arr->data[arr->count++] = entity;
}

void entity_manager_update(EntityArray* arr, TileArray *tile_arr) {
    for (int i = 0; i < arr->count; i++) {
        Entity* entity = &arr->data[i];
        //run update
        switch (entity->type)
        {
            case ENTITY_TYPE_PLAYER:
                player_update(entity);
            break;

            default: break;
        }

        //removal
        if (entity->remove) {
            REMOVE_AT_SWAPBACK(arr, i);
            i--;
        }
    }
}

void entity_manager_draw(EntityArray* arr) {
    /*for (int i = 0; i < arr->count; i++) {
        Entity* e = &arr->data[i];
        DrawRectangle(e->x, e->y, e->size, e->size, GREEN);
    }*/

    for (int i = 0; i < arr->count; i++) {
        Entity* e = &arr->data[i];
        Sprite currentSprite = sprite_manager_get_sprite(e->current_sprite_id);
    
        DrawTexture(currentSprite.frames[e->image_index], e->x, e->y, WHITE);    
    }
}
