#ifndef TRANSFORM_H
#define TRANSFORM_H

typedef struct transform_t {
    float x;
    float y;
    float image_xscale;
    float image_yscale;
    int up; //vertical flip (-1, 1)
    int right; //horizontal flip (-1, 1)
    float rotation_rad;
} transform_t;

typedef struct TransformArray {
    transform_t* data;
    int count;
    int capacity;
} TransformArray;

TransformArray transform_manager_create(int capacity);
void transform_manager_add(TransformArray *transforms, transform_t transform);

#endif
