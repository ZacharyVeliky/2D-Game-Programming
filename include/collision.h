#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "gfc_vector.h"
#include "entity.h"
#include "tile_set.h"

typedef struct {
	float x, y;
	float w, h;

}Rect;

typedef struct {
	float x, y;
	float r;

}Circle;
/**
*@brief test if two rects are overlaping
*@param A test if two rects are overlaping
*/

int collision_test_all();
//int collision_circle_test(Circle A, Circle B);


#endif // !__COLLISION_H__

#pragma once
