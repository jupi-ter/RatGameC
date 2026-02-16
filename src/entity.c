#include "entity.h"
#include "transform.h"
#include "collision.h"
#include "renderable.h"
#include <stdlib.h>

EntityRegistry entity_registry_create(int initial_capacity) {
    EntityRegistry reg = {0};
    reg.collision_types = calloc(initial_capacity, sizeof(CollisionShape));
    reg.count = 0;
    reg.capacity = initial_capacity;
    return reg;
}

void entity_registry_free(EntityRegistry* reg) {
    free(reg->collision_types);
    reg->collision_types = NULL;
    reg->count = 0;
    reg->capacity = 0;
}

static void grow_arrays(EntityRegistry* reg,
    TransformArray* transforms,
    RenderableArray* renderables,
    CircleArray* circles,
    RectangleArray* rectangles) {
    int new_capacity = reg->capacity * 2;
    
    reg->collision_types = realloc(reg->collision_types, sizeof(CollisionShape) * new_capacity);
    transforms->data = realloc(transforms->data, sizeof(transform_t) * new_capacity);
    renderables->data = realloc(renderables->data, sizeof(Renderable) * new_capacity);
    circles->data = realloc(circles->data, sizeof(Circle) * new_capacity);
    rectangles->data = realloc(rectangles->data, sizeof(RectWrapper) * new_capacity);
    
    // Initialize new slots to COLLISION_NONE
    for (int i = reg->capacity; i < new_capacity; i++) {
        reg->collision_types[i] = COLLISION_NONE;
    }
    
    reg->capacity = new_capacity;
    transforms->capacity = new_capacity;
    renderables->capacity = new_capacity;
    circles->capacity = new_capacity;
    rectangles->capacity = new_capacity;
}

uint32_t entity_create(EntityRegistry* reg,
    TransformArray* transforms,
    RenderableArray* renderables,
    CircleArray* circles,
    RectangleArray* rectangles) {
    if (reg->count >= reg->capacity) {
        grow_arrays(reg, transforms, renderables, circles, rectangles);
    }
    
    uint32_t id = reg->count;
    reg->collision_types[id] = COLLISION_NONE;
    reg->count++;
    return id;
}

int entity_destroy(EntityRegistry* reg, uint32_t entity_id,
    TransformArray* transforms,
    RenderableArray* renderables,
    CircleArray* circles,
    RectangleArray* rectangles) {
    if (entity_id >= reg->count) return -1;
    
    int last_index = reg->count - 1;
    
    // If we're not deleting the last entity, swap last into this slot
    if (entity_id != last_index) {
        CollisionShape last_collision = reg->collision_types[last_index];
        
        // Swap all component data
        reg->collision_types[entity_id] = last_collision;
        transforms->data[entity_id] = transforms->data[last_index];
        renderables->data[entity_id] = renderables->data[last_index];
        
        // Update collision owner_id
        if (last_collision == COLLISION_RECT) {
            rectangles->data[entity_id] = rectangles->data[last_index];
            rectangles->data[entity_id].owner_id = entity_id;
        } else if (last_collision == COLLISION_CIRC) {
            circles->data[entity_id] = circles->data[last_index];
            circles->data[entity_id].owner_id = entity_id;
        }
        
        reg->count--;
        return last_index;  // Game code must update references from last_index -> entity_id
    }
    
    reg->collision_types[last_index] = COLLISION_NONE;
    reg->count--;
    return -1;  // No entity was moved
}

void entity_set_collision(EntityRegistry* reg, uint32_t entity_id, CollisionShape shape) {
    if (entity_id < reg->count) {
        reg->collision_types[entity_id] = shape;
    }
}

CollisionShape entity_get_collision(EntityRegistry* reg, uint32_t entity_id) {
    if (entity_id >= reg->count) return COLLISION_NONE;
    return reg->collision_types[entity_id];
}
