                                                            #include "game_generated.h"

uint32_t player_create(GameState* game, float x, float y) {
    uint32_t entity_id = entity_create(&game->registry, &game->transforms,
    &game->renderables, &game->circles, &game->rectangles);


    game->entity_types[entity_id] = ENTITY_TYPE_PLAYER;

    game->transforms.data[entity_id] = (transform_t){
        .x = x, .y = y,
        .image_xscale = 1.0f, .image_yscale = 1.0f,
        .up = 1, .right = 1, .rotation_rad = 0.0f
    };

    game->renderables.data[entity_id] = (Renderable){
        .current_sprite_id = SPRITE_NONE,
        .image_index = 0,
        .frame_counter = 0.0f,
        .image_speed = 0.0f
    };

    if (game->players.count >= game->players.capacity) {
        game->players.capacity = game->players.capacity == 0 ? 8 : game->players.capacity * 2;
        game->players.data = realloc(game->players.data, sizeof(Player) * game->players.capacity);
    }

    game->players.data[game->players.count++] = (Player){
        .entity_id = entity_id,
        .hsp = 0,
        .vsp = 0
    };

    // on_create
    Player* entity = &game->players.data[game->players.count - 1];
    uint32_t eid = entity->entity_id;  // For component access
    entity->hsp = 0;
    entity->vsp = 0;
    (&game->renderables.data[eid])->current_sprite_id = SPRITE_YELLOW;
    (&game->renderables.data[eid])->image_speed = 0.1;
    return entity_id;
}

void player_update(GameState* game, uint32_t entity_id) {
    Player* entity = NULL;
    for (int i = 0; i < game->players.count; i++) {
        if (game->players.data[i].entity_id == entity_id) {
            entity = &game->players.data[i];
            break;
        }
    }
    if (!entity) return;

    uint32_t eid = entity_id;

    // on_update
    entity->hsp = 0;
    entity->vsp = 0;
    if (keyboard_check(KEY_RIGHT)) {
        entity->hsp = 2;
    }
    if (keyboard_check(KEY_LEFT)) {
        entity->hsp = -2;
    }
    if (keyboard_check(KEY_DOWN)) {
        entity->vsp = 2;
    }
    if (keyboard_check(KEY_UP)) {
        entity->vsp = -2;
    }
    (&game->transforms.data[eid])->x = (&game->transforms.data[eid])->x + entity->hsp;
    (&game->transforms.data[eid])->y = (&game->transforms.data[eid])->y + entity->vsp;
}

void player_destroy(GameState* game, uint32_t entity_id) {
    int moved_id = entity_destroy(&game->registry, entity_id,
        &game->transforms, &game->renderables,
        &game->circles, &game->rectangles);

    for (int i = 0; i < game->players.count; i++) {
        if (game->players.data[i].entity_id == entity_id) {
            game->players.data[i] = game->players.data[game->players.count - 1];
            game->players.count--;
            break;
        }
    }

    // Fix moved entity references (swap-and-pop)
    if (moved_id != -1) {
        for (int i = 0; i < game->players.count; i++) {
            if (game->players.data[i].entity_id == (uint32_t)moved_id) {
                game->players.data[i].entity_id = entity_id;
                break;
            }
        }
    }
}

void game_init(GameState* game) {
    game->entity_types = malloc(sizeof(EntityType) * 128);

    game->players.data = malloc(sizeof(Player) * 8);
    game->players.capacity = 8;
    game->players.count = 0;

    player_create(game, 64, 64);
}

void game_update(GameState* game) {
    for (int i = 0; i < game->players.count; i++) {
        player_update(game, game->players.data[i].entity_id);
    }
}

void game_cleanup(GameState* game) {
    free(game->players.data);
}

void dispatch_collision(GameState* game, uint32_t id1, uint32_t id2) {
    switch (game->entity_types[id1]) {
    default:
        break;
    }
}

void instance_destroy(GameState* game, uint32_t entity_id) {
    switch (game->entity_types[entity_id]) {
    case ENTITY_TYPE_PLAYER:
        player_destroy(game, entity_id);
        break;
    default:
        break;
    }
}

