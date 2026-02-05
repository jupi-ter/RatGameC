#include "tile.h"
#include "raylib.h"
#include "utils.h"
#include <stdlib.h>
#include "sprite.h"

TileArray tile_array_create(int capacity) {
    TileArray arr = {0};
    arr.count = 0;
    arr.capacity = capacity;
    arr.data = malloc(sizeof(Tile) * capacity);
    return arr;
}

void tile_array_add(TileArray *tiles, int sprite_id, int x, int y) {
    if (tiles->count + 1 > tiles->capacity) {
        tiles->capacity *= 2;
        tiles->data = realloc(tiles->data, sizeof(Tile) * tiles->capacity);
    }

    Rectangle rect = {
        .x = x,
        .y = y,
        .width = TILE_SIZE,
        .height = TILE_SIZE
    };

    Tile tile = {
        .sprite_id = sprite_id,
        .bounds = rect
    };

    tiles->data[tiles->count++] = tile;
}

bool check_tile_collision(TileArray *tiles, Rectangle test_bbox) {
    for (int i = 0; i < tiles->count; i++) {
        Tile tile = tiles->data[i];
        if (CheckCollisionRecs(test_bbox, tile.bounds)) {
            return true;
        }
    }
    return false;
}

void tile_array_draw(TileArray *tiles) {
    for (int i = 0; i < tiles->count; i++) {
        Tile tile = tiles->data[i];
        Rectangle rect = tile.bounds;
        Sprite sprite = sprite_manager_get_sprite(tiles->data[i].sprite_id);
        DrawTexture(sprite.frames[0], rect.x, rect.y, WHITE);
    }
}