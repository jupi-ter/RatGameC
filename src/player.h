#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "tile.h"
#include "collision.h"
#include <stdbool.h>

typedef struct Player {
    int entity_id;
    int coyote_counter;
    int buffer_counter;
    float hsp;
    float vsp;
    float move_speed;
    bool is_grounded;
} Player;

typedef struct PlayerArray {
    Player *data;
    int count;
    int capacity;
} PlayerArray;

PlayerArray player_array_create(int capacity);
void player_array_add(PlayerArray *players, Player player);
void update_players(PlayerArray* players, EntityArray *entities, TileGrid *tiles, RectangleArray* rectangles);

#endif
