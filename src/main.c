#include <raylib.h>
#include "utils.h"
#include "entity.h"
#include "sprite.h"
#include "tile.h"
#include "collision.h"
#include "renderer.h"
#include <stdio.h>
#include "player.h"

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

    TileArray tiles = tile_array_create(rows * cols);
    tile_array_add(&tiles, SPRITE_WALL, GAME_WIDTH/2+1, (GAME_HEIGHT/2) + TILE_SIZE+2);

    CircleArray circles = circ_array_create(16);
    RectangleArray rectangles = rect_array_create(16);
    PlayerArray players = player_array_create(4);
    EntityArray entities = entity_manager_create(16);

    // BEGIN CREATING PLAYER

    // INCREMENT ENTITY COUNT ON EVERY INSTANTIATION
    // THIS SHOULD BE A UNIQUE NUMBER
    //entity_count+=1; //commented for now because of segfault. to fix this we need add_at functions.

    int player_x = GAME_WIDTH / 2;
    int player_y = GAME_HEIGHT / 2;

    Rectangle player_rect = {
        .x = (float)player_x,
        .y = (float)player_y,
        .width = 10,
        .height = 10
    };

    RectWrapper player_collision = {
        .owner_id = entity_count, //USE ENTITY COUNT ON EVERY CONNECTED COMPONENT
        .rect = player_rect
    };

    Entity player_entity = {
        .id = entity_count,
        .x = (float)player_x,
        .y = (float)player_y,
        .size = 10,
        .type = ENTITY_TYPE_PLAYER,
        .current_sprite_id = SPRITE_PLAYER_IDLE,
        .image_index = 0,
        .image_speed = 0.3f,
        .frame_counter = 0.0f,
        .collision_shape = COLLISION_RECT
    };
    
    Player player = {
        .entity_id = entity_count,
        .buffer_counter = 0,
        .coyote_counter = 0,
        .hsp = 0,
        .vsp = 0,
        .move_speed = 0
    };

    rect_array_add(&rectangles, player_collision);
    entity_manager_add(&entities, player_entity);
    player_array_add(&players, player);

    // END CREATING PLAYER

    while (!WindowShouldClose())
    {
        update_players(&players, &entities, &tiles, &rectangles);
        check_collisions(&rectangles, &circles, handle_collisions);

        BeginDrawing();
        ClearBackground(BLACK);
        
        //BEGIN RENDERING
        begin_render(renderTarget);
        
        tile_array_draw(&tiles);
        // will be replaced by renderables
        entity_manager_draw(&entities);
        //draw_collisions(&rectangles, &circles);

        char buffer[16];
        sprintf(buffer,"%d", GetFPS());
        DrawText(buffer, 8, 8, 8, BLACK);

        //END RENDERING
        end_render(renderTarget);

        EndDrawing();
    }

    FREE_ARRAY(&rectangles);
    FREE_ARRAY(&circles);
    FREE_ARRAY(&tiles);
    FREE_ARRAY(&entities);
    FREE_ARRAY(&players);
    sprite_manager_unload_all();
    CloseWindow();
    return 0;
}