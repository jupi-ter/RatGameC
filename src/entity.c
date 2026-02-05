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

void entity_manager_add(EntityArray* entities, Entity entity, RectangleArray *rectangles, CircleArray *circles) {
    if (entities->count + 1 > entities->capacity) {
        entities->capacity *= 2;
        entities->data = realloc(entities->data, sizeof(Entity) * entities->capacity);
    }

    if (entity.collision_shape == COLLISION_RECT) {
        rect_array_add(rectangles, entity.collision.rect_collision);
    } else if (entity.collision_shape == COLLISION_CIRC) {
        circ_array_add(circles, entity.collision.circle_collision);
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
        Sprite currentSprite = sprite_manager_get_sprite(e->current_sprite_id);
    
        DrawTexture(currentSprite.frames[e->image_index], e->x, e->y, WHITE);    
    }
}
