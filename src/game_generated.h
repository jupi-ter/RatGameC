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
} Player;

typedef struct PlayerArray {
Player* data;
int count;
int capacity;
} PlayerArray;

typedef struct Enemy {
uint32_t entity_id;
float hsp;
int direction;
} Enemy;

typedef struct EnemyArray {
Enemy* data;
int count;
int capacity;
} EnemyArray;

typedef struct GameState {
    // Engine components
    EntityRegistry registry;
    TransformArray transforms;
    RenderableArray renderables;
    CircleArray circles;
    RectangleArray rectangles;
    TimerArray timers;
    PlayerArray players;
    EnemyArray enemys;
} GameState;

uint32_t player_create(GameState* game, float x, float y);
void player_update(GameState* game, uint32_t entity_id);
void player_destroy(GameState* game, uint32_t entity_id);
uint32_t enemy_create(GameState* game, float x, float y);
void enemy_update(GameState* game, uint32_t entity_id);
void enemy_destroy(GameState* game, uint32_t entity_id);
void game_init(GameState* game);
void game_init(GameState* game);
void game_update(GameState* game);
void game_cleanup(GameState* game);
void dispatch_collision(GameState* game, uint32_t id1, uint32_t id2);

#endif // GAME_GENERATED_H
