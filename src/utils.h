#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define Clamp(value, min, max) { if (value < min) value = min; if (value > max) value = max; }
#define RandomInt(min, max) ((rand() % (max - min + 1)) + min)
#define Choose(opt1, opt2) (RandomInt(0,1) == 0 ? opt1 : opt2)


#endif