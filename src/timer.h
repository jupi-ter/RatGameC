#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include "utils.h"

typedef void (*CallbackWithContext)(void* context);

typedef struct Timer {
    int frame_target;
    int counter;
    bool completed;
    CallbackWithContext callback;
    void* context;
} Timer;

typedef struct TimerArray {
    Timer *data;
    int count;
    int capacity;
} TimerArray;

TimerArray timer_manager_create(int capacity);
void timer_manager_add(TimerArray *arr, Timer timer);
void timer_manager_update(TimerArray *arr);

#endif
