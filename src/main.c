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
    * move entity extenders (player, enemy, etc) to separate, cleaner functions that don't take up main space.
    * move tile collisions to regular collisions (unsure)
*/

int main(void)
{
    LevelID level = LEVEL_ONE;
    LevelData level_data = tile_grid_load_level(level);
    
    if (!level_data.success) {
        printf("Something failed while trying to load level %d", level);
        return 1;
    }
    
    TileGrid tiles = level_data.tiles;

    int SCALE_FACTOR = 4;
    int game_w = level_data.width;
    int game_h = level_data.height;
    int screen_w = game_w * SCALE_FACTOR;
    int screen_h = game_h * SCALE_FACTOR;

    InitWindow(screen_w, screen_h, "RatGame");
    SetTargetFPS(60);
    
    RenderTexture2D render_target = LoadRenderTexture(game_w, game_h);
    
    // array declaration
    int entity_count = 0;
    CircleArray circles = circ_array_create(16);
    RectangleArray rectangles = rect_array_create(16);
    TransformArray transforms = transform_manager_create(16);
    PlayerArray players = player_array_create(4);
    EntityArray entities = entity_manager_create(16); // remove
    RenderableArray renderables = renderable_manager_create(16);

    // BEGIN CREATING PLAYER

    // INCREMENT ENTITY COUNT ON EVERY INSTANTIATION
    // THIS SHOULD BE A UNIQUE NUMBER
    //entity_count+=1; //commented for now because of segfault. to fix this we need add_at functions.

    int player_x = level_data.player_spawn_x;
    int player_y = level_data.player_spawn_y;

    Rectangle player_rect = {
        .x = (float)player_x,
        .y = (float)player_y,
        .width = 8,
        .height = 8
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
        .current_sprite_id = SPRITE_PLAYER,
        .image_index = 0,
        .image_speed = 0.1f,
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
        begin_render(render_target);
        
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
        end_render(screen_w, screen_h, game_w, game_h, render_target);

        EndDrawing();
    }

    FREE_ARRAY(&renderables);
    FREE_ARRAY(&transforms);
    FREE_ARRAY(&rectangles);
    FREE_ARRAY(&circles);
    tile_grid_free(&tiles);
    FREE_ARRAY(&entities);
    FREE_ARRAY(&players);
    sprite_manager_unload_all();
    CloseWindow();
    return 0;
}