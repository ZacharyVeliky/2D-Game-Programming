#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "entity.h"

/**
 * @brief creates an instance of a bug entity
 * @param position the location in the world to spawn the bug at
 * @returns NULL on error, or a pointer to the bug
 */
Entity* switch_ent_new(Vector2D position);
void activate(Entity* ent);
#endif

