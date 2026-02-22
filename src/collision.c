#include "collision.h"
#include "entity.h"
#include "forward.h"
#include "game_generated.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

CircleArray circ_array_create(int capacity) {
    CircleArray circles = {0};
    circles.count = 0;
    circles.capacity = capacity;
    circles.data = malloc(sizeof(Circle) * capacity);
    return circles;
}

RectangleArray rect_array_create(int capacity) {
    RectangleArray rectangles = {0};
    rectangles.count = 0;
    rectangles.capacity = capacity;
    rectangles.data = malloc(sizeof(RectWrapper) * capacity);
    return rectangles;
}

void circ_array_add(CircleArray* circles, Circle circ) {
    if (circles->count + 1 > circles->capacity) {
        circles->capacity *= 2;
        circles->data = realloc(circles->data, sizeof(Circle) * circles->capacity);
    }
    circles->data[circles->count++] = circ;
}

void rect_array_add(RectangleArray* rectangles, RectWrapper rect) {
    if (rectangles->count + 1 > rectangles->capacity) {
        rectangles->capacity *= 2;
        rectangles->data = realloc(rectangles->data, sizeof(RectWrapper) * rectangles->capacity);
    }
    rectangles->data[rectangles->count++] = rect;
}

void sync_collisions_with_transforms(GameState* game) {
    printf("Syncing %d rectangles\n", game->rectangles.count);
    for (int i = 0; i < game->rectangles.count; i++) {
        uint32_t owner = game->rectangles.data[i].owner_id;
        if (owner < game->registry.count) {
            game->rectangles.data[i].rect.x = game->transforms.data[owner].x;
            game->rectangles.data[i].rect.y = game->transforms.data[owner].y;
            printf("Entity %d collision at (%.1f, %.1f) size (%.1f, %.1f)\n",
                   owner,
                   game->rectangles.data[i].rect.x,
                   game->rectangles.data[i].rect.y,
                   game->rectangles.data[i].rect.width,
                   game->rectangles.data[i].rect.height);
        }
    }

    // Sync circles
    for (int i = 0; i < game->circles.count; i++) {
        uint32_t owner = game->circles.data[i].owner_id;
        if (owner < game->registry.count) {
            game->circles.data[i].position.x = game->transforms.data[owner].x;
            game->circles.data[i].position.y = game->transforms.data[owner].y;
        }
    }
}

void check_collisions(GameState* game, CollisionCallback on_collision) {
    //FIXME: this is good but we need to add a spatial grid on top so we don't have issues with games with too many entities.
    int rect_ids[1024];
    int rect_count = 0;
    int circ_ids[1024];
    int circ_count = 0;
    EntityRegistry* reg = &game->registry;
    RectangleArray* rectangles = &game->rectangles;
    CircleArray* circles = &game->circles;

    for (int i = 0; i < (int)reg->count; i++) {
        CollisionShape shape = entity_get_collision(reg, i);
        if (shape == COLLISION_RECT) {
            rect_ids[rect_count++] = i;
        } else if (shape == COLLISION_CIRC) {
            circ_ids[circ_count++] = i;
        }
    }

    // rect-rect
    for (int i = 0; i < rect_count; i++) {
        for (int j = i + 1; j < rect_count; j++) {
            int id1 = rect_ids[i];
            int id2 = rect_ids[j];
            RectWrapper c1 = rectangles->data[id1];
            RectWrapper c2 = rectangles->data[id2];

            if (CheckCollisionRecs(c1.rect, c2.rect)) {
                if (on_collision) {
                    on_collision(game, id1, id2);
                }
            }
        }
    }

    // circ-circ
    for (int i = 0; i < circ_count; i++) {
        for (int j = i + 1; j < circ_count; j++) {
            int id1 = circ_ids[i];
            int id2 = circ_ids[j];
            Circle c1 = circles->data[id1];
            Circle c2 = circles->data[id2];

            if (CheckCollisionCircles(c1.position, c1.radius, c2.position, c2.radius)) {
                if (on_collision) {
                    on_collision(game, id1, id2);
                }
            }
        }
    }

    // circ-rect
    for (int i = 0; i < rect_count; i++) {
        for (int j = 0; j < circ_count; j++) {
            int rect_id = rect_ids[i];
            int circ_id = circ_ids[j];
            Circle c1 = circles->data[circ_id];
            RectWrapper c2 = rectangles->data[rect_id];

            if (CheckCollisionCircleRec(c1.position, c1.radius, c2.rect)) {
                if (on_collision) {
                    on_collision(game, circ_id, rect_id);
                }
            }
        }
    }
}

void draw_collisions(RectangleArray* rectangles, CircleArray* circles) {
    Color fuchsia = {
        .r = 255,
        .g = 0,
        .b = 255,
        .a = 120
    };

    for (int i = 0; i < rectangles->count; i++) {
        RectWrapper c1 = rectangles->data[i];
        DrawRectangleRec(c1.rect, fuchsia);
    }

    for (int i = 0; i < circles->count; i++) {
        Circle c1 = circles->data[1];
        DrawCircle(c1.position.x, c1.position.y ,c1.radius, fuchsia);
    }
}

bool place_meeting(GameState* game, uint32_t entity_id, float x, float y, EntityType type) {
    // Get the collision shape of the checking entity
    CollisionShape shape = game->registry.collision_types[entity_id];

    if (shape == COLLISION_RECT) {
        RectWrapper temp_rect = game->rectangles.data[entity_id];
        temp_rect.rect.x = x;
        temp_rect.rect.y = y;

        // Check against all entities of the specified type
        for (uint32_t i = 0; i < game->registry.count; i++) {
            if (i == entity_id) continue;  // Don't collide with self
            if (game->entity_types[i] != type) continue;  // Wrong type

            CollisionShape other_shape = game->registry.collision_types[i];

            if (other_shape == COLLISION_RECT) {
                // Rect vs Rect
                if (CheckCollisionRecs(temp_rect.rect, game->rectangles.data[i].rect)) {
                    return true;
                }
            } else if (other_shape == COLLISION_CIRC) {
                // Rect vs Circle
                if (CheckCollisionCircleRec(game->circles.data[i].position,
                                           game->circles.data[i].radius,
                                           temp_rect.rect)) {
                    return true;
                }
            }
        }
    } else if (shape == COLLISION_CIRC) {
        Circle temp_circ = game->circles.data[entity_id];
        temp_circ.position.x = x;
        temp_circ.position.y = y;

        // Check against all entities of the specified type
        for (uint32_t i = 0; i < game->registry.count; i++) {
            if (i == entity_id) continue;  // Don't collide with self
            if (game->entity_types[i] != type) continue;  // Wrong type

            CollisionShape other_shape = game->registry.collision_types[i];

            if (other_shape == COLLISION_CIRC) {
                // Circle vs Circle
                if (CheckCollisionCircles(temp_circ.position, temp_circ.radius,
                                         game->circles.data[i].position,
                                         game->circles.data[i].radius)) {
                    return true;
                }
            } else if (other_shape == COLLISION_RECT) {
                // Circle vs Rect
                if (CheckCollisionCircleRec(temp_circ.position,
                                           temp_circ.radius,
                                           game->rectangles.data[i].rect)) {
                    return true;
                }
            }
        }
    }

    return false;
}
