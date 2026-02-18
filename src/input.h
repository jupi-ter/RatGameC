#ifndef INPUT_H
#define INPUT_H

#include <raylib.h>

// define all keys as ints here.
// int value should be equal to raylib internal value.
#define KEY_RIGHT 262
#define KEY_LEFT 263
#define KEY_UP 265
#define KEY_DOWN 264

int keyboard_check(int key);
int keyboard_check_pressed(int key);
int keyboard_check_released(int key);

#endif
