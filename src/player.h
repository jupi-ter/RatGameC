#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "tile.h"

typedef struct Player {
    int entity_id;
    int coyoteCounter;
    int bufferCounter;
    float hsp;
    float vsp;
} Player;

typedef struct PlayerArray {
    Player *data;
    int count;
    int capacity;
} PlayerArray;

PlayerArray player_array_create(int capacity);
void player_array_add(PlayerArray *players, Player player);
void update_players(PlayerArray* players, EntityArray *entities, TileArray *tiles);

#endif
