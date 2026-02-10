#ifndef TILE_H
#define TILE_H

#include <stdbool.h>
#include <raylib.h>
#include "sprite.h"

typedef struct TileGrid {
    unsigned char* data;
    int width;
    int height;
} TileGrid;

typedef enum LevelID {
    LEVEL_NONE,
    LEVEL_ONE,
    LEVEL_TWO,
} LevelID;

typedef enum TileMetadata {
    PLAYER_POSITION,
    TILE_METADATA_COUNT
} TileMetadata;

TileGrid tile_grid_create(int width, int height);
void tile_grid_free(TileGrid* grid);
void tile_grid_set(TileGrid *grid, int x, int y, SpriteID type);
SpriteID tile_grid_get(TileGrid *grid, int x, int y);
bool tile_grid_check_collision(TileGrid *grid, Rectangle bbox);
void tile_grid_draw(TileGrid *grid);
void tile_grid_draw_debug(TileGrid *grid);
bool tile_grid_load_level(TileGrid *grid, LevelID level_id);
Vector2 level_get_player_spawnpoint(LevelID level_id);
extern const char* TILE_METADATA_NAME[];

#endif 