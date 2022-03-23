#ifndef __ABILITY_ITEMS_H__
#define __ABILITY_ITEMS_H__

#include "entity.h"
#include "gfc_vector.h"

/**
 * @brief creates an instance of a bug entity
 * @param position the location in the world to spawn the bug at
 * @returns NULL on error, or a pointer to the bug
 */
Entity* ability_item_new(Vector2D position,int item_id);

#endif
