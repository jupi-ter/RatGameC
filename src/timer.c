#include "timer.h"
#include <stdlib.h>
#include "utils.h"

TimerArray timer_manager_create(int capacity) {
    TimerArray arr = {0};
    arr.count = 0;
    arr.capacity = capacity;
    arr.data = malloc(sizeof(Timer) * capacity);
    return arr;
}

void timer_manager_add(TimerArray* arr, Timer timer) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Timer) * arr->capacity);
    }

    arr->data[arr->count++] = timer;
}

void timer_manager_update(TimerArray* arr) {
    for (int i = 0; i < arr->count; i++) {
        Timer* timer = &arr->data[i];
        timer->counter++;

        if (timer->counter >= timer->frame_target) {
            timer->callback(timer->context);
            REMOVE_AT_SWAPBACK(arr, i);
            i--;
        }
    }
}