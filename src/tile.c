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

void tile_array_add(TileArray *arr, int sprite_id, int x, int y) {
    if (arr->count + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Tile) * arr->capacity);
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

    arr->data[arr->count++] = tile;
}

bool check_tile_collision(TileArray *arr, Rectangle test_bbox) {
    for (int i = 0; i < arr->count; i++) {
        Tile tile = arr->data[i];
        if (CheckCollisionRecs(test_bbox, tile.bounds)) {
            return true;
        }
    }
    return false;
}

void tile_array_draw(TileArray *arr) {
    for (int i = 0; i < arr->count; i++) {
        Tile tile = arr->data[i];
        Rectangle rect = tile.bounds;
        Sprite sprite = sprite_manager_get_sprite(arr->data[i].sprite_id);
        DrawTexture(sprite.frames[0], rect.x, rect.y, WHITE);
    }
}