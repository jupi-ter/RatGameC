#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "transform.h"

typedef struct Renderable {
    int current_sprite_id;
    int image_index;
    float frame_counter;
    float image_speed;
} Renderable;

typedef struct RenderableArray {
    Renderable *data;
    int count;
    int capacity;
} RenderableArray;

RenderableArray renderable_manager_create(int capacity);
void renderable_manager_add(RenderableArray* renderables, Renderable renderable);
void renderable_manager_update(RenderableArray *renderables, EntityRegistry* reg);
void renderable_manager_draw(RenderableArray* renderables, TransformArray *transforms, EntityRegistry* reg);
void renderable_change_sprite_data(Renderable* renderable, int sprite_id, float image_speed);
#endif
