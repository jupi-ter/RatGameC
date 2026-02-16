#include <raylib.h>
#include "utils.h"
#include "entity.h"
#include "sprite.h"
#include "tile.h"
#include "collision.h"
#include "renderer.h"
#include <stdio.h>
#include "timer.h"
#include "transform.h"
#include "renderable.h"

void handle_collisions(int id1, int id2) {
    printf("%d %d\n", id1, id2);
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
    
    // Create entity registry and component arrays
    EntityRegistry registry = entity_registry_create(16);
    TransformArray transforms = transform_manager_create(16);
    RenderableArray renderables = renderable_manager_create(16);
    CircleArray circles = circ_array_create(16);
    RectangleArray rectangles = rect_array_create(16);
    TimerArray timers = timer_manager_create(16);
    
    // Example: Create a test entity (this would be done by codegen in real usage)
    // uint32_t test_entity = entity_create(&registry, &transforms, &renderables, &circles, &rectangles);
    // entity_set_collision(&registry, test_entity, COLLISION_RECT);
    // transforms.data[test_entity] = (transform_t){.x = 100, .y = 100, ...};
    // renderables.data[test_entity] = (Renderable){.current_sprite_id = SPRITE_PLAYER, ...};
    // rectangles.data[test_entity] = (RectWrapper){.owner_id = test_entity, .rect = {100, 100, 8, 8}};
    
    while (!WindowShouldClose())
    {
        // Update systems
        check_collisions(&registry, &rectangles, &circles, handle_collisions);
        timer_manager_update(&timers);
        renderable_manager_update(&renderables, &registry);
        
        BeginDrawing();
        ClearBackground(BLUE);
        
        // BEGIN RENDERING
        begin_render(render_target);
        
        tile_grid_draw(&tiles);
        renderable_manager_draw(&renderables, &transforms, &registry);
        // tile_grid_draw_debug(&tiles);
        // draw_collisions(&rectangles, &circles);
 
        // char buffer[16];
        // sprintf(buffer,"%d", GetFPS());
        // DrawText(buffer, 8, 8, 8, BLACK);
 
        // END RENDERING
        end_render(screen_w, screen_h, game_w, game_h, render_target);
        EndDrawing();
    }
    
    // Cleanup
    entity_registry_free(&registry);
    FREE_ARRAY(&renderables);
    FREE_ARRAY(&transforms);
    FREE_ARRAY(&rectangles);
    FREE_ARRAY(&circles);
    FREE_ARRAY(&timers);
    tile_grid_free(&tiles);
    sprite_manager_unload_all();
    CloseWindow();
    return 0;
}
