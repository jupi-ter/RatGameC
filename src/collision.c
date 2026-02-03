#include "collision.h"
#include <stdlib.h>
#include <raylib.h>
#include "utils.h"

CollisionArray collision_manager_create(int capacity) {
    CollisionArray arr = {0};
    arr.count = 0;
    arr.capacity = capacity;
    arr.data = malloc(sizeof(Collision) * capacity);
    return arr;
}

void collision_manager_add(CollisionArray *arr, Collision collision) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Collision) * arr->capacity);
    }

    arr->data[arr->count++] = collision;
}

void collision_manager_update(CollisionArray *arr) {
    for (int i = 0; i < arr->count; i++) {
        Collision*collision = &arr->data[i];

        if (collision->remove) {
            REMOVE_AT_SWAPBACK(arr, i);
            i--;
        }

        //process
    }
}

void collision_manager_draw(CollisionArray *arr) {
    for (int i = 0; i < arr->count; i++) {
        Collision*collision = &arr[i];
        Color f = {.r = 255, .b =0, .g=255, .a = 122 };

        if (collision->shape == COLLISION_CIRCLE) {
            DrawCircle(collision->x, collision->y, collision->radius, f);
        }

        if (collision->shape == COLLISION_RECT) {
            DrawRectangle(collision->x, collision->y, collision->width, collision->height, f);
        }
    }
}

void collide_rect_rect() {}
void collide_rect_circle() {}
void collide_circle_circle() {}