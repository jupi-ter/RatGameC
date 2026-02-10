#include <raylib.h>
#include "utils.h"
#include "entity.h"
#include "sprite.h"
#include "tile.h"
#include "collision.h"
#include "renderer.h"
#include <stdio.h>
#include "player.h"
#include "timer.h"
#include "transform.h"
#include "renderable.h"

void handle_collisions(int id1, int id2) {
    printf("%d %d", id1, id2);
}

/*
    TODO:
    * create the renderables struct and refactor.
    * analyze structure, if favorable: create the transform struct and refactor.
    * move entity extenders (player, enemy, etc) to separate, cleaner functions that don't take up main space.
    * move tiles to a grid with a 2d boolean array and make their sprite drawing even simpler
    * move tile collisions to regular collisions (unsure)
    * create the level loader. if feeling spicy, create a gui level editor.
*/

int main(void)
{
    int entity_count = 0;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RatGame");
    SetTargetFPS(60);

    RenderTexture2D renderTarget = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);

    int cols = GAME_WIDTH / TILE_SIZE;
    int rows = GAME_HEIGHT / TILE_SIZE;

    TileGrid tiles = tile_grid_create(cols, rows);
    tile_grid_load_level(&tiles, LEVEL_TWO);
    
    CircleArray circles = circ_array_create(16);
    RectangleArray rectangles = rect_array_create(16);
    TransformArray transforms = transform_manager_create(16);
    PlayerArray players = player_array_create(4);
    EntityArray entities = entity_manager_create(16);
    RenderableArray renderables = renderable_manager_create(16);
    // BEGIN CREATING PLAYER

    // INCREMENT ENTITY COUNT ON EVERY INSTANTIATION
    // THIS SHOULD BE A UNIQUE NUMBER
    //entity_count+=1; //commented for now because of segfault. to fix this we need add_at functions.

    Vector2 player_position = level_get_player_spawnpoint(LEVEL_ONE);
    int player_x = player_position.x;
    int player_y = player_position.y;

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

    transform_t transform = {
        .x = (float)player_x,
        .y = (float)player_y,
        .up = 1,
        .right = 1,
        .image_xscale = 1.0f,
        .image_yscale = 1.0f,
        .rotation_rad = 0.0f
    };

    Entity player_entity = {
        .id = entity_count,
        .collision_shape = COLLISION_RECT
    };
    
    Renderable player_render = {
        .current_sprite_id = SPRITE_PLAYER_IDLE,
        .image_index = 0,
        .image_speed = 0.3f,
        .frame_counter = 0.0f,
    };

    Player player = {
        .entity_id = entity_count,
        .buffer_counter = 0,
        .coyote_counter = 0,
        .hsp = 0,
        .vsp = 0,
        .move_speed = 0,
        .target_xscale = 1.0f,
        .target_yscale = 1.0f,
    };

    renderable_manager_add(&renderables, player_render);
    transform_manager_add(&transforms, transform);
    rect_array_add(&rectangles, player_collision);
    entity_manager_add(&entities, player_entity);
    player_array_add(&players, player);

    TimerArray timers = timer_manager_create(16);

    // END CREATING PLAYER

    while (!WindowShouldClose())
    {
        update_players(&players, &entities, &transforms, &renderables, &tiles, &rectangles, &timers);
        check_collisions(&rectangles, &circles, handle_collisions);
        timer_manager_update(&timers);
        renderable_manager_update(&renderables);

        BeginDrawing();
        ClearBackground(BLUE);
        
        // BEGIN RENDERING
        begin_render(renderTarget);
        
        tile_grid_draw(&tiles);
        renderable_manager_draw(&renderables, &transforms);
        // tile_grid_draw_debug(&tiles);
        // will be replaced by renderables
        // entity_manager_draw(&entities);
        // draw_collisions(&rectangles, &circles);
 
        // char buffer[16];
        // sprintf(buffer,"%d", GetFPS());
        // DrawText(buffer, 8, 8, 8, BLACK);
 
        // END RENDERING
        end_render(renderTarget);

        EndDrawing();
    }

    FREE_ARRAY(&rectangles);
    FREE_ARRAY(&circles);
    tile_grid_free(&tiles);
    FREE_ARRAY(&entities);
    FREE_ARRAY(&players);
    sprite_manager_unload_all();
    CloseWindow();
    return 0;
}