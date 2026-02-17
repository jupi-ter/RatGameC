#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdbool.h>
#include "forward.h"
#include "collision.h"

typedef struct EntityRegistry {
    CollisionShape* collision_types;
    int count;
    int capacity;
} EntityRegistry;

// Core functions
EntityRegistry entity_registry_create(int initial_capacity);
void entity_registry_free(EntityRegistry* reg);

// Creates entity and ensures all arrays have capacity
uint32_t entity_create(EntityRegistry* reg,
    TransformArray* transforms,
    RenderableArray* renderables,
    CircleArray* circles,
    RectangleArray* rectangles);

// Destroys entity and performs swap-and-pop on all engine arrays
// Returns the entity_id that was moved (last_index), or -1 if none moved
// Game code must update references from moved_id -> entity_id
int entity_destroy(EntityRegistry* reg, uint32_t entity_id,
    TransformArray* transforms,
    RenderableArray* renderables,
    CircleArray* circles,
    RectangleArray* rectangles);

void entity_set_collision(EntityRegistry* reg, uint32_t entity_id, CollisionShape shape);
CollisionShape entity_get_collision(EntityRegistry* reg, uint32_t entity_id);

#endif
