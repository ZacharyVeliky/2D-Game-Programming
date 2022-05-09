#ifndef __DOOR_H__
#define __DOOR_H__

#include "entity.h"

/**
 * @brief creates an instance of a bug entity
 * @param position the location in the world to spawn the bug at
 * @returns NULL on error, or a pointer to the bug
 */
Entity* door_ent_new(Vector2D position);
void open();
#endif

