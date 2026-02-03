#ifndef COLLISION_H
#define COLLISION_H

#include "entity.h"
#include <stdbool.h>

typedef enum CollisionShape {
    COLLISION_CIRCLE,
    COLLISION_RECT
} CollisionShape;

typedef struct Collision {
    CollisionShape shape;
    float radius;
    int width;
    int height;
    float deg_rotation;
    int x;
    int y;
    //for centering/origin
    int x_offset;
    int y_offset;
    int owner_id;
    bool remove;
} Collision;

typedef struct CollisionArray {
    Collision* data;
    int count;
    int capacity;
} CollisionArray;

CollisionArray collision_manager_create(int capacity);
void collision_manager_add(CollisionArray *arr, Collision collision);
void collision_manager_update(CollisionArray *cArr, EntityArray *eArr);
void collision_manager_draw(CollisionArray *arr);

#endif