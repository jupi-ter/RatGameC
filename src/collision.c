#include "collision.h"
#include "raylib.h"
#include <stdlib.h>

CircleArray circ_array_create(int capacity) {
    CircleArray arr = {0};
    arr.count = 0;
    arr.capacity = capacity;
    arr.data = malloc(sizeof(Circle) * capacity);
    return arr;
}

RectangleArray rect_array_create(int capacity) {
    RectangleArray arr = {0};
    arr.count = 0;
    arr.capacity = capacity;
    arr.data = malloc(sizeof(RectWrapper) * capacity);
    return arr;
}

void circ_array_add(CircleArray* arr, Circle circ) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Circle) * arr->capacity);
    }
    arr->data[arr->count++] = circ;
}

void rect_array_add(RectangleArray* arr, RectWrapper rect) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(RectWrapper) * arr->capacity);
    }
    arr->data[arr->count++] = rect;
}

void check_collisions(RectangleArray* rect_arr, CircleArray* circ_arr, CollisionCallback on_collision) {
    //loop both separately against each themselves and once against each other.
    
    for (int i = 0; i < rect_arr->count; i++) {
        for (int j = i + 1; j < rect_arr->count; j++) {
            RectWrapper c1 = rect_arr->data[i];
            RectWrapper c2 = rect_arr->data[j];
            if (CheckCollisionRecs(c1.rect, c2.rect)) {
                if (on_collision) {
                    on_collision(c1.owner_id, c2.owner_id);
                }
                return;
            }
        }
    }

    for (int i = 0; i < circ_arr->count; i++) {
        for (int j = i + 1; j < circ_arr->count; j++) {
            Circle c1 = circ_arr->data[i];
            Circle c2 = circ_arr->data[j];
            
            if (CheckCollisionCircles(c1.position, c1.radius, c2.position, c2.radius)) {
                if (on_collision) {
                    on_collision(c1.owner_id, c2.owner_id);
                }
                return;
            }
        }
    }

    for (int i = 0; i < rect_arr->count; i++) {
        for (int j = 0; j < circ_arr->count; j++) {
            Circle c1 = circ_arr->data[j];
            RectWrapper c2 = rect_arr->data[i];

            if (CheckCollisionCircleRec(c1.position, c1.radius, c2.rect)) {
                if (on_collision) {
                    on_collision(c1.owner_id, c2.owner_id);
                }
                return;
            }
        }
    }
}
