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
    arr.data = malloc(sizeof(Rectangle) * capacity);
    return arr;
}

void circ_array_add(CircleArray* arr, Circle circ) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Circle) * arr->capacity);
    }

    arr->data[arr->count++] = circ;
}

void rect_array_add(RectangleArray* arr, Rectangle rect) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Rectangle) * arr->capacity);
    }

    arr->data[arr->count++] = rect;
}

void check_collisions(RectangleArray* rect_arr, CircleArray* circ_arr) {
    //loop both separately against each themselves and once against each other.
    
    //CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec);
    //CheckCollisionRecs(Rectangle rec1, Rectangle rec2);
}