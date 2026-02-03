#include "vector2.h"

vec2_t vec2_add(vec2_t v1, vec2_t v2) {
    vec2_t r = { .x = v1.x + v2.x, .y = v1.y + v2.y};
    return r;
}

vec2_t vec2_sub(vec2_t v1, vec2_t v2) {
    vec2_t r = { .x = v1.x - v2.x, .y = v1.y - v2.y};
    return r;
}