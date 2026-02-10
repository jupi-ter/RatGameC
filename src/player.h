#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "tile.h"
#include "collision.h"
#include "timer.h"
#include <stdbool.h>
#include "transform.h"
#include "renderable.h"

typedef struct Player {
    int entity_id;
    int coyote_counter;
    int buffer_counter;
    float hsp;
    float vsp;
    float move_speed;
    bool is_grounded;
    float target_xscale;
    float target_yscale;
} Player;

typedef struct PlayerArray {
    Player *data;
    int count;
    int capacity;
} PlayerArray;

PlayerArray player_array_create(int capacity);
void player_array_add(PlayerArray *players, Player player);
void update_players(PlayerArray* players, EntityArray *entities, TransformArray *transforms, RenderableArray *renderables, TileGrid *tiles, RectangleArray* rectangles, TimerArray *timers);

#endif
