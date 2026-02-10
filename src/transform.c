#include "transform.h"
#include <stdlib.h>

TransformArray transform_manager_create(int capacity) {
    TransformArray transforms = {0};
    transforms.count = 0;
    transforms.capacity = capacity;
    transforms.data = malloc(sizeof(transform_t) * capacity);
    return transforms;
}

void transform_manager_add(TransformArray* transforms, transform_t transform) {
    if (transforms->count + 1 > transforms->capacity) {
        transforms->capacity *= 2;
        transforms->data = realloc(transforms->data, sizeof(transform_t) *  transforms->capacity);
    }

    transforms->data[transforms->count++] = transform;
}