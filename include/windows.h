#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#include <SDL.h>
#include "gf2d_sprite.h"

typedef struct window_S
{
    Uint8       _inuse;     /**<this flag keeps track if this window is active or free to reassign*/
    float       frame;      /**<current frame to draw*/
    Vector2D    draw_offset;/**<draw position relative to the window position*/
    Vector2D    position;   /**<where our window lives*/
    Vector2D    velocity;   /**<how our window moves*/
    Vector3D    rotation;   /**<how to rotate the sprite*/
    Vector2D    draw_scale; /**<the scale factor for drawing the sprite*/
    Vector2D    mins, maxs;  /**<describe the bounding box around this window*/
    SDL_Rect    bounds;     /**<More bounds*/
    //void (*think)(struct window_S* self);   /**<a pointer to a think function for this window*/
    //void (*update)(struct window_S* self);   /**<a pointer to a think function for this window*/
}window;

typedef struct
{
    Uint32      max_windows;            /**<how many entities exist*/
    window* window_list;           /**<a big ole list of entities*/
}windowManager;

static windowManager window_manager = { 0 };

/**
 * @brief initialize the internal window window_manager_init
 * @note must be called before other window functions
 * @param max_entities how many concurrent entities will be supported
 */
void window_manager_init(Uint32 max_entities);

/**
 * @brief draws all active entities to the screen
 */
void window_manager_draw_all();

/**
 * @brief runs any think function for all active entities
 */
void window_manager_think_all();


/**
 * @brief runs any think function for all active entities
 */
void window_update(window* ent);


/**
 * @brief runs any think function for all active entities
 */
void window_manager_update_all();


/**
 * @brief free all active entities
 * @note for use in level transitions.
 */
void window_manager_clear();

/**
 * @brief get a new empty window
 * @returns NULL on error, or a pointer to a blank window
 */
window* window_new();

/**
 * @brief get a new empty window
 * @returns NULL on error, or a pointer to a blank window
 */
void window_update(window* ent);

/**
 * @brief draws the given window
 * @param window the window to draw
 */
void window_draw(window* window);

/**
 * @brief free the memory of an window
 * @param window the window to free
 */
void window_free(window* window);

windowManager* get_window_manager_list();
#endif
