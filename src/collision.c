#include "simple_logger.h"
#include "collision.h"
#include "gfc_vector.h"

int collision_rect_test(SDL_Rect A, SDL_Rect B) {
    if ((A.x + A.w >= B.x && A.x <= B.x + B.w) || (A.y + A.h >= B.y && A.y >= B.y + B.h))
        return 1;
    return 0;
}

int collision_rect_test_right(SDL_Rect A, SDL_Rect B) {
    if ((A.x + A.w >= B.x && A.x <= B.x) || (A.y + A.h >= B.y && A.y >= B.y))
        return 3;
    return 0;
}

int collision_rect_test_left(SDL_Rect A, SDL_Rect B) {
    if ((A.x >= B.x && A.x <= B.x + B.w) || (A.y >= B.y && A.y >= B.y + B.h))
        return 1;
    return 0;
}

int collision_test_all_ents(Entity* player) {
    Entity* other;
    int i;
    EntityManager* e_man = get_entity_manager_list();
    for (int i = 0; i < e_man->max_entities; i++) {
        other = &e_man->entity_list[i];
        if (player == other)
            continue;
        if (collision_rect_test_right(player->bounds, other->bounds))
            return  3;
        if (collision_rect_test_left(player->bounds, other->bounds))
            return 1;
        else
            return 0;
    }
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

int collision_test_all_tiles(Entity* player) {
    TileSet* other;
    int i;
    TileSetManager* t_man = get_tile_manager_list();
    for (int i = 0; i < t_man->tileset_count; i++) {
        other = &t_man->tile_set_list[i];
        if (player == other)
            continue;
        if (collision_rect_test_right(player->bounds, other->bounds))
            return  3;
        if (collision_rect_test_left(player->bounds, other->bounds))
            return 1;
        else
            return 0;
    }
}

int collision_test_all(Entity* player_col) {
    int t,e;
    t = collision_test_all_tiles(player_col);
    e = collision_test_all_ents(player_col);
    if (t != 0)
        return t;
    else if (e != 0)
        return e;
    else
        return 0;
}

//int collision_circle_test(Circle A, Circle B) {
//    if (vector2d_magnitude_squared(
//        vector2d(A.x - B.x, A.y, B.y)) > vector2d((A.r, B.r) * (A.x, B.r)))return 1;
//    return 0;
//}