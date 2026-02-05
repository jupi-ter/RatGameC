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

void timer_manager_add(TimerArray* timers, Timer timer) {
    if (timers->count + 1 > timers->capacity) {
        timers->capacity *= 2;
        timers->data = realloc(timers->data, sizeof(Timer) * timers->capacity);
    }

    timers->data[timers->count++] = timer;
}

void timer_manager_update(TimerArray* timers) {
    for (int i = 0; i < timers->count; i++) {
        Timer* timer = &timers->data[i];
        timer->counter++;

        if (timer->counter >= timer->frame_target) {
            timer->callback(timer->context);
            REMOVE_AT_SWAPBACK(timers, i);
            i--;
        }
    }
}