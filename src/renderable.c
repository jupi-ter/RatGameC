#include "renderable.h"
#include "entity.h"
#include "sprite.h"
#include "transform.h"
#include <stdlib.h>

RenderableArray renderable_manager_create(int capacity) {
    RenderableArray renderables = {0};
    renderables.count = 0;
    renderables.capacity = capacity;
    renderables.data = malloc(sizeof(Renderable) * capacity);
    return renderables;
}

void renderable_manager_add(RenderableArray* renderables, Renderable renderable) {
    if (renderables->count + 1 > renderables->capacity) {
        renderables->capacity *= 2;
        renderables->data = realloc(renderables->data, sizeof(Renderable) *  renderables->capacity);
    }

    renderables->data[renderables->count++] = renderable;
}

void renderable_manager_update(RenderableArray* renderables, EntityRegistry* reg) {
    // Only update active entities
    for (int i = 0; i < reg->count; i++) {
        Renderable* renderable = &renderables->data[i];
        renderable->frame_counter += renderable->image_speed;

        if (renderable->frame_counter >= 1.0f) {
            renderable->frame_counter -= 1.0f;
            renderable->image_index++;
            Sprite currentSprite = sprite_manager_get_sprite(renderable->current_sprite_id);
            if (renderable->image_index >= currentSprite.total_frames) {
                renderable->image_index = 0;
            }
        }
    }
}

void renderable_manager_draw(RenderableArray* renderables, TransformArray* transforms, EntityRegistry* reg) {
    // Only draw active entities
    for (int i = 0; i < reg->count; i++) {
        Renderable renderable = renderables->data[i];
        transform_t transform = transforms->data[i];
        Sprite sprite_array = sprite_manager_get_sprite(renderable.current_sprite_id);
        Texture2D current_texture = sprite_array.frames[renderable.image_index];
        
        Vector2 origin = {
            .x = 0.0f,
            .y = 0.0f
        };

        Rectangle source = {
            .x = 0,
            .y = 0,
            .width = current_texture.width * transform.right,
            .height = current_texture.height * transform.up,
        };

        Rectangle dest = {
            .x = (int)transform.x,
            .y = (int)transform.y,
            .width = current_texture.width * transform.image_xscale,
            .height = current_texture.height * transform.image_yscale,
        };

        DrawTexturePro(current_texture, source, dest, origin, 0, WHITE);
    }
}

void renderable_change_sprite_data(Renderable *renderable, int sprite_id, float image_speed) {
    if (renderable->current_sprite_id != sprite_id) {
        renderable->current_sprite_id = sprite_id;
        renderable->image_speed = image_speed;

        //reset
        renderable->frame_counter = 0.0f;
        renderable->image_index = 0;
    }
}