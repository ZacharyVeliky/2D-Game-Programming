#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SDL.h>
#include "gf2d_sprite.h"
#include "chipmunk/chipmunk.h"

typedef struct ENTITY_S
{
    Uint8       _inuse;         /**<this flag keeps track if this entity is active or free to reassign*/
    Uint8       solid;          /**<If true entity clips the world*/
    Sprite     *sprite;         /**<sprite used to draw the sprite*/
    float       frame;          /**<current frame to draw*/
    Vector2D    draw_offset;    /**<draw position relative to the entity position*/
    Vector2D    position;       /**<where our entity lives*/
    Vector2D    velocity;       /**<how our entity moves*/
    Vector3D    rotation;       /**<how to rotate the sprite*/
    Vector2D    draw_scale;     /**<the scale factor for drawing the sprite*/ 
    Vector2D    mins,maxs;      /**<describe the bounding box around this entity*/
    SDL_Rect    bounds;         /**<More bounds*/
    int         direction;      /**<More bounds*/
    int         health;         //maybe the entity has health
    int         current_health; //maybe the entity has health
    Bool        is_item;
    Bool        is_collected;
    int         item_id;
    Bool        can_be_damaged;
    Bool        is_player;
    Bool        is_mirror;
    Bool        is_invincible;
    Uint32      iframes;
    Uint32      life;
    void (*think)(struct ENTITY_S *self);   /**<a pointer to a think function for this entity*/
    void (*update)(struct ENTITY_S *self);   /**<a pointer to an update function for this entity*/
    void (*damage)(struct ENTITY_S *self, int dam);   /**<a pointer to a damage function for this entity*/
}Entity;

typedef struct
{
    Uint32      max_entities;            /**<how many entities exist*/
    Entity*     entity_list;           /**<a big ole list of entities*/
}EntityManager;

static EntityManager entity_manager = { 0 };

Bool set_draw;

/**
 * @brief initialize the internal entity entity_manager_init
 * @note must be called before other entity functions
 * @param max_entities how many concurrent entities will be supported
 */
void entity_manager_init(Uint32 max_entities);

/**
 * @brief draws all active entities to the screen
 */
void entity_manager_draw_all();

/**
 * @brief runs any think function for all active entities
 */
void entity_manager_think_all();


/**
 * @brief runs any think function for all active entities
 */
void entity_update(Entity* ent);


/**
 * @brief runs any think function for all active entities
 */
void entity_manager_update_all();


/**
 * @brief free all active entities
 * @note for use in level transitions.
 */
void entity_manager_clear();

/**
 * @brief get a new empty entity
 * @returns NULL on error, or a pointer to a blank entity
 */
Entity *entity_new();

/**
 * @brief get a new empty entity
 * @returns NULL on error, or a pointer to a blank entity
 */
void entity_update(Entity* ent);

/**
 * @brief draws the given entity
 * @param entity the entity to draw
 */
void entity_draw(Entity *entity);

/**
 * @brief free the memory of an entity
 * @param entity the entity to free
 */
void entity_free(Entity *entity);

EntityManager *get_entity_manager_list();
#endif
