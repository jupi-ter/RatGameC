#ifndef GAME_GENERATED_H
#define GAME_GENERATED_H

#include "forward.h"
#include "entity.h"
#include "transform.h"
#include "renderable.h"
#include "collision.h"
#include "timer.h"
#include "sprite.h"

typedef struct GameState {
    EntityRegistry registry;
    TransformArray transforms;
    RenderableArray renderables;
    CircleArray circles;
    RectangleArray rectangles;
    TimerArray timers;
} GameState;

// Generic interface - engine calls these
void game_init(GameState* game);
void game_update(GameState* game);
void game_cleanup(GameState* game);

#endif
