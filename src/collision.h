#ifndef COLLISION_H
#define COLLISION_H

#include <raylib.h>
#include "forward.h"

typedef enum CollisionShape {
    COLLISION_NONE,
    COLLISION_RECT,
    COLLISION_CIRC
} CollisionShape;

typedef void (*CollisionCallback)(int id1, int id2);

typedef struct RectWrapper {
    int owner_id;
    Rectangle rect;
} RectWrapper;

// why doesn't raylib have a Circle struct
typedef struct Circle {
    int owner_id;
    Vector2 position;
    float radius;
} Circle;

typedef struct RectangleArray {
    RectWrapper* data;
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
void rect_array_add(RectangleArray* arr, RectWrapper rect);
void check_collisions(EntityRegistry* reg, RectangleArray* rectangles, CircleArray* circles, CollisionCallback on_collision);
void draw_collisions(RectangleArray* rectangles, CircleArray* circles);

#endif
