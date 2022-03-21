#include "simple_logger.h"
#include "collision.h"
#include "gfc_vector.h"

typedef struct
{
    Uint32 max_objects;            /**<how many entities exist*/
    SDL_Rect* col_list;           /**<a big ole list of colliders*/
}CollisionManager;

static CollisionManager collision_manager = { 0 };

int collision_rect_test(SDL_Rect A, SDL_Rect B) {
    if ((A.x + A.w >= B.x && A.x <= B.x + B.w) || (A.y + A.h >= B.y && A.y >= B.y + B.h))
        return 1;
    return 0;
}

int collision_rect_test_right(SDL_Rect A, SDL_Rect B) {
    if ((A.x + A.w >= B.x && A.x <= B.x) || (A.y + A.h >= B.y && A.y >= B.y))
        return 1;
    return 0;
}

int collision_rect_test_left(SDL_Rect A, SDL_Rect B) {
    if ((A.x >= B.x && A.x <= B.x + B.w) || (A.y >= B.y && A.y >= B.y + B.h))
        return 1;
    return 0;
}

void collision_manager_clear()
{
    int i;
    for (i = 0; i < collision_manager.max_objects; i++)
    {
        entity_free(&collision_manager.col_list[i]);
    }
}

void collision_manager_close()
{
    collision_manager_clear(); // clear all entities first
    if (collision_manager.col_list != NULL)
    {
        free(collision_manager.col_list);
    }
    slog("entity manager closed");
}

void collision_manager_init(Uint32 max_entities)
{
    if (max_entities == 0)
    {
        slog("cannot allocate memory for zero entities!");
        return;
    }
    if (collision_manager.col_list != NULL)
    {
        slog("entity manager already initialized");
        return;
    }
    collision_manager.max_objects = max_entities;
    collision_manager.col_list = gfc_allocate_array(sizeof(Entity), max_entities);
    atexit(collision_manager_close);
    slog("entity manager initialized");
}

Bool collision_check_left(SDL_Rect player)
{
    int i;
    for (i = 0; i < collision_manager.max_objects; i++)
    {
        collision_rect_test_left(player, collision_manager.col_list[i]);
    }
}

Bool collision_check_right(SDL_Rect player)
{
    int i;
    for (i = 0; i < collision_manager.max_objects; i++)
    {
        collision_rect_test_right(player, collision_manager.col_list[i]);
    }
}

//Entity* object_new()
//{
//    int i;
//    for (i = 0; i < collision_manager.max_objects; i++)
//    {
//        if (!collision_manager.col_list[i]._inuse)
//        {
//            //GOT ONE!
//            collision_manager.col_list[i]._inuse = 1;
//            collision_manager.col_list[i].draw_scale.x = 1;
//            collision_manager.col_list[i].draw_scale.y = 1;
//            return &collision_manager.col_list[i];
//        }
//    }
//    slog("out of entities");
//    return NULL;
//}

//int collision_circle_test(Circle A, Circle B) {
//    if (vector2d_magnitude_squared(
//        vector2d(A.x - B.x, A.y, B.y)) > vector2d((A.r, B.r) * (A.x, B.r)))return 1;
//    return 0;
//}