#include "collision.h"
#include "entity.h"
#include "raylib.h"
#include <stdlib.h>

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

void check_collisions(EntityRegistry* reg, RectangleArray* rectangles, CircleArray* circles, CollisionCallback on_collision) {
    //FIXME: this is good but we need to add a spatial grid on top so we don't have issues with games with too many entities.
    int rect_ids[1024];
    int rect_count = 0;
    int circ_ids[1024];
    int circ_count = 0;
    
    for (int i = 0; i < reg->count; i++) {
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
                    on_collision(id1, id2);
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
                    on_collision(id1, id2);
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
                    on_collision(circ_id, rect_id);
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
}
