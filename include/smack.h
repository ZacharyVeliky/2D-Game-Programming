#ifndef __SMACK_H__
#define __SMACK_H__

#include "entity.h"
#include "gfc_vector.h"

/**
 * @brief creates an instance of a bug entity
 * @param position the location in the world to spawn the bug at
 * @returns NULL on error, or a pointer to the bug
 */
Entity* samck_new(Vector2D position, int dir);
void smack_attack(Vector2D start, int dir);
#endif
