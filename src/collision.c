#include "collision.h"
#include "gfc_vector.h"
//#include "../include/gf2d_draw.h"

int collision_rect_test(SDL_Rect A, SDL_Rect B) {
    if ((A.x > B.x + B.w) || (A.x + A.w < B.x) || (A.y > B.y + B.h) || (A.y + A.h < B.y)) return 0;
    return 1;
}

//int collision_circle_test(Circle A, Circle B) {
//    if (vector2d_magnitude_squared(
//        vector2d(A.x - B.x, A.y, B.y)) > vector2d((A.r, B.r) * (A.x, B.r)))return 0;
//    return 1;
//}