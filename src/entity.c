#include <raylib.h>
#include "entity.h"
#include <stdlib.h>

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

/*void entity_manager_draw(EntityArray* arr) {
    for (int i = 0; i < arr->count; i++) {
        
    }
}*/