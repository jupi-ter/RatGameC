#include <raylib.h>
#include "entity.h"
#include <stdlib.h>
#include "sprite.h"

EntityArray entity_manager_create(int capacity) {
    EntityArray arr = {0};
    arr.count = 0;
    arr.capacity = capacity;
    arr.data = malloc(sizeof(Entity) * capacity);

    return arr;
}

void entity_manager_add(EntityArray* entities, Entity entity) {
    if (entities->count + 1 > entities->capacity) {
        entities->capacity *= 2;
        entities->data = realloc(entities->data, sizeof(Entity) * entities->capacity);
    }

    entities->data[entities->count++] = entity;
}

// deprecated, entity is simply a component now.
// removal still has to be done though, with moving dead entities to another array to be cleared.
/*
void entity_manager_update(EntityArray* entities, TileArray *tiles) {
    for (int i = 0; i < entities->count; i++) {
        Entity* entity = &entities->data[i];
        
        // run updates
        switch (entity->type)
        {
            case ENTITY_TYPE_PLAYER:
                //player_update(entity, tiles);
            break;

            default: break;
        }   
    }
}*/

void entity_manager_draw(EntityArray* arr) {
    /*for (int i = 0; i < arr->count; i++) {
        Entity* e = &arr->data[i];
        DrawRectangle(e->x, e->y, e->size, e->size, GREEN);
    }*/

    for (int i = 0; i < arr->count; i++) {
        Entity* e = &arr->data[i];
        Sprite sprite_array = sprite_manager_get_sprite(e->current_sprite_id);
        Texture2D current_texture = sprite_array.frames[e->image_index];
        
        Vector2 origin = {
            .x = 0.0f,
            .y = 0.0f
        };

        Rectangle source = {
            .x = 0,
            .y = 0,
            .width = current_texture.width * e->right,
            .height = current_texture.height * e->up,
        };

        Rectangle dest = {
            .x = (int)e->x,
            .y = (int)e->y,
            .width = current_texture.width * e->image_xscale,
            .height = current_texture.height * e->image_yscale,
        };

        DrawTexturePro(current_texture, source, dest, origin, 0 , WHITE);
    }
}

void entity_change_sprite_data(Entity *entity, int sprite_id, float image_speed) {
    if (entity->current_sprite_id != sprite_id) {
        entity->current_sprite_id = sprite_id;
        entity->image_speed = image_speed;

        //reset
        entity->frame_counter = 0.0f;
        entity->image_index = 0;
    }
}
