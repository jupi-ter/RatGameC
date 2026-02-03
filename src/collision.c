#include "collision.h"
#include "utils.h"
#include <stdlib.h>
#include <raylib.h>
#include <math.h>

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

void collision_manager_update(CollisionArray *cArr, EntityArray *eArr) {
    for (int i = 0; i < cArr->count; i++) {
        Collision*collision = &cArr->data[i];

        if (collision->remove) {
            REMOVE_AT_SWAPBACK(cArr, i);
            i--;
        }

        //process
        for (int i = 0; i < cArr->count; i++) {
            for(int j = i + 1; j < cArr->count; j++) {
                Collision *c1 = &cArr->data[i];
                Collision *c2 = &cArr->data[j];

                if (c1->shape == COLLISION_CIRCLE && c2->shape == COLLISION_CIRCLE) {
                    //too lazy for vector2
                    //todo: macro this tomorrow

                    int ox1 = c1->x + c1->x_offset;
                    int oy1 = c1->y + c1->y_offset;

                    int ox2 = c2->x + c2->x_offset;
                    int oy2 = c2->y + c2->y_offset;

                    int d1 = ox2 - ox1;
                    int d2 = oy2 - oy1;

                    int pow1 = d1 * d1;
                    int pow2 = d2 * d2;

                    float dist = sqrtf(pow1 + pow2);
                    if (dist < (c1->radius + c2->radius)) {
                        Entity* e1 = &eArr->data[c1->owner_id];
                        Entity* e2 = &eArr->data[c2->owner_id];

                        e1->on_collision(e1, e2);
                        e2->on_collision(e2, e1);
                    }
                }
            }
        }
    }
}

void collision_manager_draw(CollisionArray *arr) {
    for (int i = 0; i < arr->count; i++) {
        Collision*collision = &arr->data[i];
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