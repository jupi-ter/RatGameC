#include "tile.h"
#include "raylib.h"
#include "sprite.h"
#include "utils.h"
#include <stdlib.h>

TileGrid tile_grid_create(int width, int height) {
    TileGrid grid = {0};
    grid.width = width;
    grid.height = height;
    grid.data = calloc(width * height, sizeof(unsigned char));
    return grid;
}

void tile_grid_free(TileGrid* grid) {
    free(grid->data);
    grid->width = 0;
    grid->height = 0;
    grid->data = NULL;
}

void tile_grid_set(TileGrid *grid, int x, int y, SpriteID type) {
    if (x < 0 || x >= grid->width || y < 0 || y >= grid->height) return;
    grid->data[y * grid->width + x] = type;
}

SpriteID tile_grid_get(TileGrid *grid, int x, int y) {
    if (x < 0 || x >= grid->width || y < 0 || y >= grid->height) return SPRITE_NONE;
    return grid->data[y * grid->width + x];
}

bool tile_grid_check_collision(TileGrid *grid, Rectangle bbox) {
    int start_x = (int)(bbox.x / TILE_SIZE);
    int start_y = (int)(bbox.y / TILE_SIZE);
    int end_x = (int)((bbox.x + bbox.width - 1) / TILE_SIZE);
    int end_y = (int)((bbox.y + bbox.height - 1) / TILE_SIZE);
    
    for (int y = start_y; y <= end_y; y++) {
        for (int x = start_x; x <= end_x; x++) {
            SpriteID type = tile_grid_get(grid, x, y);
            if (type != SPRITE_NONE) {  // any non-air tile is solid
                return true;
            }
        }
    }
    return false;
}

void tile_grid_draw(TileGrid *grid) {
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            SpriteID type = grid->data[y * grid->width + x];
            if (type == SPRITE_NONE) continue;
            
            Sprite sprite = sprite_manager_get_sprite(type);
            DrawTexture(sprite.frames[0], x * TILE_SIZE, y * TILE_SIZE, WHITE);
        }
    }
}

void tile_grid_draw_debug(TileGrid* grid) {
    Color grid_color = BLACK;
    for (int y = 0; y <= grid->height; y++) {
        DrawLine(
            0, y * TILE_SIZE,
            grid->width * TILE_SIZE, y * TILE_SIZE,
            grid_color
        );
    }
    
    for (int x = 0; x <= grid->width; x++) {
        DrawLine(
            x * TILE_SIZE, 0,
            x * TILE_SIZE, grid->height * TILE_SIZE,
            grid_color
        );
    }
}
