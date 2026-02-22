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

typedef enum {
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_WALL,
    ENTITY_TYPE_COUNT
} EntityType;

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

typedef struct Wall {
uint32_t entity_id;
} Wall;

typedef struct WallArray {
Wall* data;
int count;
int capacity;
} WallArray;

typedef struct GameState {
// Engine components
EntityRegistry registry;
TransformArray transforms;
RenderableArray renderables;
CircleArray circles;
RectangleArray rectangles;
TimerArray timers;
EntityType* entity_types;

    PlayerArray players;
    WallArray walls;
} GameState;

uint32_t player_create(GameState* game, float x, float y);
void player_update(GameState* game, uint32_t entity_id);
void player_destroy(GameState* game, uint32_t entity_id);
uint32_t wall_create(GameState* game, float x, float y);
void wall_update(GameState* game, uint32_t entity_id);
void wall_destroy(GameState* game, uint32_t entity_id);

// Collision helper
bool place_meeting(GameState* game, uint32_t entity_id, float x, float y, EntityType type);

void game_init(GameState* game);void game_update(GameState* game);void game_cleanup(GameState* game);void dispatch_collision(GameState* game, uint32_t id1, uint32_t id2);
#endif // GAME_GENERATED_H
