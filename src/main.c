#include <raylib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "utils.h"
#include "entity.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RatGame");
    SetTargetFPS(60);

    EntityArray entity_array = entity_manager_create(16);
    Entity player = {
        .id = 1,
        .x = SCREEN_WIDTH / 2,
        .y = SCREEN_HEIGHT / 2,
        .size = 10,
        .speed = 2,
        .type = ENTITY_TYPE_PLAYER,
        .current_sprite_id = SPRITE_PLAYER_IDLE,
        .image_index = 0,
        .image_speed = 0.1f,
        .frame_counter = 0.0f
    };

    entity_manager_add(&entity_array, player);

    while (!WindowShouldClose())
    {   
        entity_manager_update(&entity_array);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        entity_manager_draw(&entity_array);

        EndDrawing();
    }

    FREE_ARRAY(&entity_array);

    CloseWindow();
    return 0;
}