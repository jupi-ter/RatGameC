#include <raylib.h>
#include "utils.h"
#include "entity.h"
#include "sprite.h"
#include "collision.h"
#include "renderer.h"
#include <stdio.h>
#include "timer.h"
#include "transform.h"
#include "renderable.h"
#include "game_generated.h"

void handle_collisions(int id1, int id2) {
    printf("%d %d\n", id1, id2);
}

int main(void)
{
    int SCALE_FACTOR = 4;
    //these takes their values from the loaded level, but for simplicity we remove that 
    int game_w = 128; 
    int game_h = 128;

    int screen_w = game_w * SCALE_FACTOR;
    int screen_h = game_h * SCALE_FACTOR;
    InitWindow(screen_w, screen_h, "RatGame");
    SetTargetFPS(60);
    
    RenderTexture2D render_target = LoadRenderTexture(game_w, game_h);
    
    // Create game state with entity registry and component arrays
    GameState game = {0};
    game.registry = entity_registry_create(16);
    game.transforms = transform_manager_create(16);
    game.renderables = renderable_manager_create(16);
    game.circles = circ_array_create(16);
    game.rectangles = rect_array_create(16);
    game.timers = timer_manager_create(16);
    
    // Example: Create a test entity (this would be done by codegen in real usage)
    // uint32_t test_entity = entity_create(&registry, &transforms, &renderables, &circles, &rectangles);
    // entity_set_collision(&registry, test_entity, COLLISION_RECT);
    // transforms.data[test_entity] = (transform_t){.x = 100, .y = 100, ...};
    // renderables.data[test_entity] = (Renderable){.current_sprite_id = SPRITE_PLAYER, ...};
    // rectangles.data[test_entity] = (RectWrapper){.owner_id = test_entity, .rect = {100, 100, 8, 8}};
    
    while (!WindowShouldClose())
    {
        // Update systems
        check_collisions(&game.registry, &game.rectangles, &game.circles, handle_collisions);
        timer_manager_update(&game.timers);
        renderable_manager_update(&game.renderables, &game.registry);
        
        BeginDrawing();
        ClearBackground(BLUE);
        
        // BEGIN RENDERING
        begin_render(render_target);
        
        renderable_manager_draw(&game.renderables, &game.transforms, &game.registry);
        
        // END RENDERING
        end_render(screen_w, screen_h, game_w, game_h, render_target);
        EndDrawing();
    }
    
    // Cleanup
    entity_registry_free(&game.registry);
    FREE_ARRAY(&game.renderables);
    FREE_ARRAY(&game.transforms);
    FREE_ARRAY(&game.rectangles);
    FREE_ARRAY(&game.circles);
    FREE_ARRAY(&game.timers);
    sprite_manager_unload_all();
    CloseWindow();
    return 0;
}
