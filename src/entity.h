#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdbool.h>
#include "collision.h"

typedef enum EntityType {
    ENTITY_TYPE_PLAYER,
} EntityType;

typedef struct Entity {
    uint32_t id;
    
    // rethink removal
    //bool remove;

    // entities shouldn't directly own their collisions
    // neither should they hold a pointer to them
    // we need to use SoA here, but that requires refactoring the collision management a little.
    CollisionShape collision_shape;
} Entity;

typedef struct EntityArray {
    Entity* data;
    int count;
    int capacity;
} EntityArray;

EntityArray entity_manager_create(int capacity);
void entity_manager_add(EntityArray* arr, Entity entity);

// maybe this could exist in the game loop for the sole purpose of checking deletion.
// for now, it's deprecated.
//void entity_manager_update(EntityArray* arr, TileArray *tile_arr);

#endif