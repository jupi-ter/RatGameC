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
    
    // these should go into a transform parallel array
    float x;
    float y;
    int up; //vertical flip (-1, 1)
    int right; //horizontal flip (-1, 1)
    float image_xscale;
    float image_yscale;
    
    // are these necessary?
    EntityType type;
    int size;
    
    // rethink removal
    bool remove;
    
    // these need to go into their own component
    int current_sprite_id;
    int image_index;
    float frame_counter;
    float image_speed;
    
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

//renderables...
void entity_manager_draw(EntityArray* arr);

// im only leaving this here to not lose them
// these values need to go to another struct
void entity_change_sprite_data(Entity* entity, int sprite_id, float image_speed);

#endif