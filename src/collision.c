#include "collision.h"
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

void check_collisions(RectangleArray* rectangles, CircleArray* circles, CollisionCallback on_collision) {
    //loop both separately against each themselves and once against each other.
    
    for (int i = 0; i < rectangles->count; i++) {
        for (int j = i + 1; j < rectangles->count; j++) {
            RectWrapper c1 = rectangles->data[i];
            RectWrapper c2 = rectangles->data[j];
            if (CheckCollisionRecs(c1.rect, c2.rect)) {
                if (on_collision) {
                    on_collision(c1.owner_id, c2.owner_id);
                }
                return;
            }
        }
    }

    for (int i = 0; i < circles->count; i++) {
        for (int j = i + 1; j < circles->count; j++) {
            Circle c1 = circles->data[i];
            Circle c2 = circles->data[j];
            
            if (CheckCollisionCircles(c1.position, c1.radius, c2.position, c2.radius)) {
                if (on_collision) {
                    on_collision(c1.owner_id, c2.owner_id);
                }
                return;
            }
        }
    }

    for (int i = 0; i < rectangles->count; i++) {
        for (int j = 0; j < circles->count; j++) {
            Circle c1 = circles->data[j];
            RectWrapper c2 = rectangles->data[i];

            if (CheckCollisionCircleRec(c1.position, c1.radius, c2.rect)) {
                if (on_collision) {
                    on_collision(c1.owner_id, c2.owner_id);
                }
                return;
            }
        }
    }
}
