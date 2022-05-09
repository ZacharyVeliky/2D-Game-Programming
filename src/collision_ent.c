#include "simple_logger.h"
#include "collision_ent.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"
#include "../include/player_ent.h"


Entity* col_ent;
SDL_Rect rect;

void collision_update(Entity* self) {

    Vector2D direction;
    direction.x = 0 - self->position.x;
    direction.y = 0 - self->position.y;
    rect.x = 930;
    rect.y = 545;
    rect.w = 300;
    rect.h = 10;
    self->bounds = rect;
}

Entity* collision_ent_new(Vector2D position, SDL_Rect bound)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/Items/wand_of_energy.png", 500, 500, 4);
    ent->update = collision_update;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounds = bound;
    rect = bound;
    ent->health = 1;
    ent->touch_player = true;
    vector2d_copy(ent->position, position);
    col_ent = ent;
    return ent;
}

//eof