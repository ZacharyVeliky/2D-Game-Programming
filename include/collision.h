#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "gfc_vector.h"
#include "entity.h"

//typedef struct COLLISION_OBJECT_S
//{
//	//Entity		ent;		/**<Entity that has collision Not required*/
//    SDL_Rect    bounds;     /**<More bounds*/
//}Collision_object;
//
//typedef struct {
//	float x, y;
//	float w, h;
//
//}Rect;
//
//typedef struct {
//	float x, y;
//	float r;
//
//}Circle;
/**
*@brief test if two rects are overlaping
*@param A test if two rects are overlaping
*/
//int collision_rect_test(SDL_Rect A, SDL_Rect B);
//int collision_rect_test_right(SDL_Rect A, SDL_Rect B);
//int collision_rect_test_left(SDL_Rect A, SDL_Rect B);
//int collision_circle_test(Circle A, Circle B);

//Bool collision_check_left(SDL_Rect player);
//Bool collision_check_right(SDL_Rect player);
Entity* collision_test_get_ent(Entity* player);
int collision_test_all_ents(Entity* player);

int collision_test_all_tiles_precise(Entity* player);
int collision_test_all_tiles(Entity* player);

int collision_test_all_precise(Entity* player_col);


#endif // !__COLLISION_H__

#pragma once
