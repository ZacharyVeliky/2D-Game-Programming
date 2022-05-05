#include "simple_logger.h"
#include "collision.h"
#include "tile_set.h"

typedef struct
{
    Uint32 max_objects;            /**<how many entities exist*/
    SDL_Rect* col_list;           /**<a big ole list of colliders*/
}CollisionManager;

static CollisionManager collision_manager = { 0 };

int collision_rect_test(SDL_Rect A, SDL_Rect B) {
    if (A.x <= B.x + B.w && A.x + A.w >= B.x && A.y <= B.y + B.h && A.y + A.h >= B.y)
        return 1;
    return 0;
}

int collision_rect_test_right(SDL_Rect A, SDL_Rect B) {//|| (A.y + A.h >= B.y && A.y >= B.y) &&(A.y <= B.y + B.h - 1 && A.y + A.h + 1 >= B.y) 
    if ((A.x + A.w >= B.x && A.x <= B.x))//
        return 1;
    return 0;
}

int collision_rect_test_left(SDL_Rect A, SDL_Rect B) {//|| (A.y >= B.y && A.y >= B.y + B.h)
    if ((B.x + B.w >= A.x && B.x <= A.x) )// 
        return 1;
    return 0;
}

Entity* collision_test_get_ent(Entity* player) {
    Entity* other;
    int i;
    EntityManager* e_man = get_entity_manager_list();
    for (int i = 0; i < e_man->max_entities; i++) {
        other = &e_man->entity_list[i];
        if (player == other)
            continue;
        if (collision_rect_test(player->bounds, other->bounds))
            return other;
    }
}

int collision_test_all_ents(Entity* player) {
    Entity* other;
    EntityManager* e_man = get_entity_manager_list();
    for (int i = 0; i < e_man->max_entities; i++) {
        
        other = &e_man->entity_list[i];
        if (player == other)
            continue;

        if (collision_rect_test(player->bounds, other->bounds))
            return 1;
    }
    return 0;
}

int collision_test_all_ents_precise(Entity* player) {
    Entity* other;
    EntityManager* e_man = get_entity_manager_list();
    for (int i = 0; i < e_man->max_entities; i++) {
        
        other = &e_man->entity_list[i];
        if (player == other)
            continue;
        if (collision_rect_test(player->bounds, other->bounds)) {
            if (collision_rect_test_right(player->bounds, other->bounds))
                return  3;
            if (collision_rect_test_left(player->bounds, other->bounds))
                return 1;
        }
    }
    return 0;
}

int collision_test_all_tiles(Entity* player) {
    TileSet* other;
    TileSetManager* t_man = get_tile_set_manager();
    for (int i = 0; i < t_man->tileset_count; i++) {

        other = &t_man->tile_set_list[i];

        if (collision_rect_test(player->bounds, other->bounds)) {
            slog("touch");
            return 1;
        }
    }
    return 0;
}

int collision_test_all_tiles_precise(Entity* player) {
    TileSet* other;
    TileSetManager* t_man = get_tile_set_manager();
    for (int i = 0; i < t_man->tileset_count; i++) {
        other = &t_man->tile_set_list[i];
        if (player == other)
            continue;
        
        if (collision_rect_test_right(player->bounds, other->bounds)) {
            //slog("3");
            return  3;
        }
        if (collision_rect_test_left(player->bounds, other->bounds)) {
            //slog("1");
            return 1;
        }
    }
    return 0;
}

int collision_test_all(Entity* player_col) {
    int t, e;
    t = collision_test_all(player_col);
    e = collision_test_all(player_col);
    if (t != 0)
        return t;
    if (e != 0)
        return e;
    else
        return 0;
}
int collision_test_all_precise(Entity* player_col) {
    int t, e;
    t = collision_test_all_tiles_precise(player_col);
    e = collision_test_all_ents_precise(player_col);
    if (t != 0)
        return t;
    if (e != 0)
        return e;
    else
        return 0;
}