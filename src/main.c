#include <raylib.h>
#include "utils.h"
#include "entity.h"
#include "sprite.h"
#include "tile.h"
#include "collision.h"
#include "renderer.h"
#include <stdio.h>

void handle_collisions(int id1, int id2) {
    printf("%d %d", id1, id2);
}

int main(void)
{
    int entity_count = 0;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RatGame");
    SetTargetFPS(60);

    RenderTexture2D renderTarget = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);

    int cols = GAME_WIDTH / TILE_SIZE;
    int rows = GAME_HEIGHT / TILE_SIZE;

    TileArray tile_arr = tile_array_create(rows * cols);
    tile_array_add(&tile_arr, SPRITE_WALL, GAME_WIDTH/2+1, (GAME_HEIGHT/2) + TILE_SIZE+2);

    CircleArray circle_arr = circ_array_create(16);
    RectangleArray rect_arr = rect_array_create(16);

    EntityArray entity_arr = entity_manager_create(16);

    Rectangle player_rect = {
        .x = GAME_WIDTH/2,
        .y = GAME_HEIGHT/2,
        .width = 10,
        .height = 10
    };

    RectWrapper player_collision = {
        .owner_id = entity_count,
        .rect = player_rect
    };

    Entity player = {
        .id = 1,
        .x = GAME_WIDTH / 2,
        .y = GAME_HEIGHT / 2,
        .size = 10,
        .speed = 2,
        .type = ENTITY_TYPE_PLAYER,
        .current_sprite_id = SPRITE_PLAYER_IDLE,
        .image_index = 0,
        .image_speed = 0.1f,
        .frame_counter = 0.0f,
        .collision = {
            .rect_collision = player_collision
        }
    };

    entity_manager_add(&entity_arr, player, &rect_arr, &circle_arr);

    while (!WindowShouldClose())
    {   
        entity_manager_update(&entity_arr, &tile_arr);
        check_collisions(&rect_arr, &circle_arr, handle_collisions);

        BeginDrawing();
        ClearBackground(BLACK);
        
        //BEGIN RENDERING
        begin_render(renderTarget);
        
        tile_array_draw(&tile_arr);
        entity_manager_draw(&entity_arr);

        //END RENDERING
        end_render(renderTarget);

        EndDrawing();
    }

    FREE_ARRAY(&rect_arr);
    FREE_ARRAY(&circle_arr);
    FREE_ARRAY(&tile_arr);
    FREE_ARRAY(&entity_arr);
    sprite_manager_unload_all();
    CloseWindow();
    return 0;
}