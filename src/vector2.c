#include "vector2.h"
#include <math.h>

vec2_t vec2_add(vec2_t v1, vec2_t v2) {
    vec2_t r = { .x = v1.x + v2.x, .y = v1.y + v2.y};
    return r;
}

vec2_t vec2_sub(vec2_t v1, vec2_t v2) {
    vec2_t r = { .x = v1.x - v2.x, .y = v1.y - v2.y};
    return r;
}

float vec2_distance(vec2_t v1, vec2_t v2) {
    float s1 = v2.x - v1.x;
    float s2 = v2.y - v1.y;

    return sqrtf((s1 * s1) + (s2 * s2));
}