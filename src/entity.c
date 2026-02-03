#include "entity.h"
#include <stdlib.h>
#include <raylib.h>
#include "utils.h"

EntityArray entity_manager_create(int capacity) {
    EntityArray arr = {0};
    arr.count = 0;
    arr.capacity = capacity;
    arr.data = malloc(sizeof(Entity) * capacity);

    return arr;
}

void entity_manager_add(EntityArray* arr, Entity entity) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Entity) * arr->capacity);
    }

    arr->data[arr->count++] = entity;
}

void entity_manager_update(EntityArray* arr) {
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
        Sprite* currentSprite = sprite_manager_get_sprite(e->current_sprite_id);
        
        DrawTexture(currentSprite->frames[e->image_index], e->x, e->y, WHITE);    
    }
}

void player_update(Entity* e) {
    int moveHor = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
    int moveVer = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);

    e->x += moveHor * e->speed;
    e->y += moveVer * e->speed;

    CLAMP(e->x, 0, SCREEN_WIDTH - e->size);
    CLAMP(e->y, 0, SCREEN_HEIGHT - e->size);

    // animation
    e->frame_counter += e->image_speed;

    if (e->frame_counter >= 1.0f) {
        e->frame_counter -= 1.0f;
        e->image_index++;
        Sprite* currentSprite = sprite_manager_get_sprite(e->current_sprite_id);
        if (e->image_index >= currentSprite->total_frames) {
            e->image_index = 0;
        }
    }
}