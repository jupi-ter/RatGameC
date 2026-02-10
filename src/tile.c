#include "tile.h"
#include "raylib.h"
#include "sprite.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* TILE_METADATA_NAME[] = {
    [PLAYER_POSITION]="player_position",
};

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

bool tile_is_solid(SpriteID type) {
    switch(type) {
        case SPRITE_WALL: return true;
        default: return false;
    }
}

bool tile_grid_check_collision(TileGrid *grid, Rectangle bbox) {
    int start_x = (int)(bbox.x / TILE_SIZE);
    int start_y = (int)(bbox.y / TILE_SIZE);
    int end_x = (int)((bbox.x + bbox.width - 1) / TILE_SIZE);
    int end_y = (int)((bbox.y + bbox.height - 1) / TILE_SIZE);
    
    for (int y = start_y; y <= end_y; y++) {
        for (int x = start_x; x <= end_x; x++) {
            SpriteID type = tile_grid_get(grid, x, y);
            if (type != SPRITE_NONE && tile_is_solid(type)) {
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

bool tile_grid_load_level(TileGrid *tiles, LevelID level_id) {
    char filename[64];
    sprintf(filename, "./assets/levels/lvl%d.rat", level_id);

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: could not open file for loading: %s\n", filename);
        return false;
    }

    char line[256];
    //int width = 0; int height = 0;
    //bool metadata_read = false;

    while (fgets(line, sizeof(line), file)) {
        //skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') continue;
        
        //remove trailing newline
        line[strcspn(line, "\r\n")] = 0;
        
        //read metadata (WIDTH HEIGHT)
        
        //read tile data
        char tile_name[64];
        int x, y;
        
        if (sscanf(line, "%63s %d %d", tile_name, &x, &y) == 3) {
            // Skip metadata entries
            bool is_metadata = false;
            for (int i = 0; i < TILE_METADATA_COUNT; i++) {
                if (strcmp(tile_name, TILE_METADATA_NAME[i]) == 0) {
                    is_metadata = true;
                    break;
                }
            }
            if (is_metadata) continue;

            SpriteID sprite = sprite_get_id_from_name(tile_name);
            if (sprite == SPRITE_NONE && strcmp(tile_name, "none") != 0) {
                continue;
            }
            tile_grid_set(tiles, x, y, sprite);
        } else {
            printf("Warning: Invalid tile line, skipping: %s\n", line);
        }
    }
    
    fclose(file);
    
    printf("Loaded level from: %s\n", filename);
    return true;
}

Vector2 level_get_player_spawnpoint(LevelID level_id) {
    Vector2 position = { .x = 0, .y = 0};
    char filename[64];
    sprintf(filename, "./assets/levels/lvl%d.rat", level_id);
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: could not open file for loading: %s\n", filename);
        return position;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        line[strcspn(line, "\n")] = 0;
        
        char tile_name[64];
        int x, y;
        if (sscanf(line, "%63s %d %d", tile_name, &x, &y) == 3) {
            if (strcmp(tile_name, TILE_METADATA_NAME[PLAYER_POSITION]) == 0) {
                position = (Vector2){ .x = x * TILE_SIZE, .y = y * TILE_SIZE };
                break;
            }
        }
    }
    
    fclose(file);
    return position;
}