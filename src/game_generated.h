#ifndef GAME_GENERATED_H
#define GAME_GENERATED_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "forward.h"

#include "input.h"

#include "entity.h"
#include "transform.h"
#include "renderable.h"
#include "collision.h"
#include "timer.h"

#include "sprite.h"

typedef struct Player {
uint32_t entity_id;
float hsp;
float vsp;
int move;
} Player;

typedef struct PlayerArray {
Player* data;
int count;
int capacity;
} PlayerArray;

typedef struct GameState {
// Engine components
EntityRegistry registry;
TransformArray transforms;
RenderableArray renderables;
CircleArray circles;
RectangleArray rectangles;
TimerArray timers;

    PlayerArray players;
} GameState;

uint32_t player_create(GameState* game, float x, float y);
void player_update(GameState* game, uint32_t entity_id);
void player_destroy(GameState* game, uint32_t entity_id);
void game_init(GameState* game);void game_update(GameState* game);void game_cleanup(GameState* game);
#endif // GAME_GENERATED_H
