
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "gfc_vector.h"
#include "entity.h"
#include "SDL_rect.h"
#include "../include/collision.h"
#include "../include/tile_map.h"

typedef struct
{
    Uint32 max_entities;            /**<how many entities exist*/
    Entity  *entity_list;           /**<a big ole list of entities*/
}EntityManager;

static EntityManager entity_manager = {0};


void entity_manager_close()
{
    entity_manager_clear(); // clear all entities first
    if(entity_manager.entity_list != NULL)
    {
        free(entity_manager.entity_list);
    }
    slog("entity manager closed");    
}

void entity_manager_init(Uint32 max_entities)
{
    if (max_entities == 0)
    {
        slog("cannot allocate memory for zero entities!");
        return;
    }
    if (entity_manager.entity_list != NULL)
    {
        slog("entity manager already initialized");
        return;
    }
    entity_manager.max_entities = max_entities;
    entity_manager.entity_list = gfc_allocate_array(sizeof(Entity),max_entities);
    atexit(entity_manager_close);
    slog("entity manager initialized");
}

void entity_manager_clear()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_free(&entity_manager.entity_list[i]);
    }
}

Entity *entity_new()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)
        {
            //GOT ONE!
            entity_manager.entity_list[i]._inuse = 1;
            entity_manager.entity_list[i].draw_scale.x = 1;
            entity_manager.entity_list[i].draw_scale.y = 1;
            return &entity_manager.entity_list[i];
        }
    }
    slog("out of entities");
    return NULL;
}

void entity_think(Entity *ent)
{
    if (!ent)return;
    //generic upkeep
    
    if (ent->think)
    {
        ent->think(ent);
    }
    
    //vector2d_add(ent->position,ent->position,ent->velocity);
}

void entity_manager_think_all()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_think(&entity_manager.entity_list[i]);
    }
}

void entity_update(Entity* ent)
{
    if (!ent)return;
    //generic upkeep

    if (ent->update)
    {
        ent->update(ent);
    }
}


void entity_manager_update_all()
{
    int i;
    for (i = 0; i < entity_manager.max_entities; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_update(&entity_manager.entity_list[i]);
    }
}

void entity_manager_draw_all()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_draw(&entity_manager.entity_list[i]);
    }
}

void entity_draw(Entity *entity)
{
    Vector2D drawPosition;
    if (entity == NULL)
    {
        slog("null pointer provided, nothing to do!");
        return;
    }
    if (entity->sprite == NULL)return;// nothing to draw
    vector2d_add(drawPosition,entity->position,entity->draw_offset);
    gf2d_sprite_draw(
        entity->sprite,        
        drawPosition,
        &entity->draw_scale,
        NULL,
        &entity->rotation,
        NULL,
        NULL,
        (Uint32)entity->frame);

    //SDL_Rect rect;
    //rect.x = entity->position.x-20;
    //rect.y = entity->position.y-20;
    //rect.w = 40;
    //rect.h = 40;
    Vector4D rectBoxColor;
    rectBoxColor.x = 255;
    rectBoxColor.y = 255;
    rectBoxColor.z = 255;
    rectBoxColor.w = 255;
    gf2d_draw_rect(entity->bounds, rectBoxColor);
}

void entity_free(Entity *entity)
{
    if (entity == NULL)
    {
        slog("null pointer provided, nothing to do!");
        return;
    }
    if (entity->sprite != NULL)
    {
        gf2d_sprite_free(entity->sprite);
    }
    memset(entity,0,sizeof(Entity));
}

// eof