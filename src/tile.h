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

TileGrid tile_grid_create(int width, int height);
void tile_grid_free(TileGrid* grid);
void tile_grid_set(TileGrid *grid, int x, int y, SpriteID type);
SpriteID tile_grid_get(TileGrid *grid, int x, int y);
bool tile_grid_check_collision(TileGrid *grid, Rectangle bbox);
void tile_grid_draw(TileGrid *grid);
void tile_grid_draw_debug(TileGrid *grid);

#endif 