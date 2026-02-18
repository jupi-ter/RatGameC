#include "input.h"
#include <raylib.h>

int keyboard_check(int key) {
    return (int)(IsKeyDown(key));
}

int keyboard_check_pressed(int key) {
    return (int)(IsKeyPressed(key));
}

int keyboard_check_released(int key) {
    return (int)(IsKeyReleased(key));
}
