#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdbool.h>
#include "collision.h"
#include "tile.h"

typedef enum {
    ENTITY_TYPE_PLAYER,
} EntityType;

typedef struct Entity {
    uint32_t id;
    int x;
    int y;
    int up; //vertical flip
    int right; //horizontal flip
    
    // are these necessary?
    EntityType type;
    int size;

    // these need to be removed
    bool remove;
    int speed;

    // these need to go into their own component
    int current_sprite_id;
    int image_index;
    float frame_counter;
    float image_speed;
    
    // entities shouldn't directly own their collisions
    // neither should they hold a pointer to them
    // we need to use SoA here, but that requires refactoring the collision management a little.
    CollisionShape collision_shape;
    union collision {
        RectWrapper rect_collision;
        Circle circle_collision;
    } collision;
} Entity;

typedef struct EntityArray {
    Entity* data;
    int count;
    int capacity;
} EntityArray;

EntityArray entity_manager_create(int capacity);
void entity_manager_add(EntityArray* arr, Entity entity, RectangleArray *rect_arr, CircleArray *circ_arr);
// maybe this could exist in the game loop for the sole purpose of checking deletion.
// for now, it's deprecated.
//void entity_manager_update(EntityArray* arr, TileArray *tile_arr);
void entity_manager_draw(EntityArray* arr);

#endif