#ifndef __ENERGY_ATTACK_H__
#define __ENERGY_ATTACK_H__

#include "entity.h"
#include "gfc_vector.h"

/**
 * @brief creates an instance of a bug entity
 * @param position the location in the world to spawn the bug at
 * @returns NULL on error, or a pointer to the bug
 */
Entity* energy_attack_new(Vector2D position);
void energy_attack(Vector2D start, int dir, Entity* owner);
#endif
