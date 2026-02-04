#ifndef COLLISION_H
#define COLLISION_H

#include <raylib.h>

// why doesn't raylib have a Circle struct
typedef struct Circle {
    int x;
    int y;
    float radius;
} Circle;

typedef struct RectangleArray {
    Rectangle* data;
    int count;
    int capacity;
} RectangleArray;

typedef struct CircleArray {
    Circle* data;
    int count;
    int capacity;
} CircleArray;

CircleArray circ_array_create(int capacity);
RectangleArray rect_array_create(int capacity);
void circ_array_add(CircleArray* arr, Circle circ);
void rect_array_add(RectangleArray* arr, Rectangle rect);
void check_collisions(RectangleArray* rect_arr, CircleArray* circ_arr);

#endif