#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define GAME_WIDTH 128
#define GAME_HEIGHT 128

//square
#define TILE_SIZE 8

#define CLAMP(value, min, max) { if (value < min) value = min; if (value > max) value = max; }

#define MIN(val1, val2) (val1 < val2) ? val1 : val2

#define GET_RANDOM_INT(min, max) ((rand() % (max - min + 1)) + min)

#define CHOOSE(opt1, opt2) (GET_RANDOM_INT(0,1) == 0 ? opt1 : opt2)

#define REMOVE_AT_SWAPBACK(arr, index) \
    do { \
        if ((index) >= (arr)->count || (index) < 0) break; \
        (arr)->count--; \
        (arr)->data[index] = (arr)->data[(arr)->count]; \
    } while(0)

#define FREE_ARRAY(arr) \
    do { \
        free((arr)->data); \
        (arr)->data = NULL; \
        (arr)->count = 0; \
        (arr)->capacity = 0; \
    } while(0)

#endif