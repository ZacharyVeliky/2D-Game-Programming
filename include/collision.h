#ifndef __COLLISION_H__
#define __COLLISION_H__

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
int collision_rect_test(Rect A, Rect B);
int collision_circle_test(Circle A, Circle B);

#endif // !__COLLISION_H__

#pragma once
