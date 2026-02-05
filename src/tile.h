#ifndef TILE_H
#define TILE_H

#include <stdbool.h>
#include <raylib.h>

typedef struct Tile {
    int sprite_id;
    Rectangle bounds;
} Tile;

typedef struct TileArray {
    Tile* data;
    int count;
    int capacity;
} TileArray;

TileArray tile_array_create(int capacity);
void tile_array_add(TileArray *arr, int sprite_id, int x, int y);
bool check_tile_collision(TileArray *arr, Rectangle test_bbox);
void tile_array_draw(TileArray *arr);

#endif 
