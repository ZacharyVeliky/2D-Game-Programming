#ifndef __COLLISION_ENT_H
#define __COLLISION_ENT_H

#include "entity.h"

/**
 * @brief creates an instance of a collider
 * @param position the location in the world to spawn a collider
 * @returns NULL on error, or a pointer to the bug
 */
Entity* collision_ent_new(Vector2D position);
Entity* get_col_ent();

#endif
