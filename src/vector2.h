#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct vec2_t {
    float x;
    float y;
} vec2_t;

typedef struct vec2int_t {
    int x;
    int y;
} vec2int_t;

vec2_t vec2_add(vec2_t v1, vec2_t v2);
vec2_t vec2_sub(vec2_t v1, vec2_t v2);
float vec2_distance(vec2_t v1, vec2_t v2);

#endif
