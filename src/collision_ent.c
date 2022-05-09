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

    rect.x = self->position.x - (float)20;
    rect.y = self->position.y - (float)20;
    rect.w = 40;
    rect.h = 40;
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
    //ent->sprite = gf2d_sprite_load_all("images/space_bug_top.png", 128, 128, 16);
    ent->update = collision_update;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounds = bound;
    ent->health = 1;
    ent->touch_player = true;
    vector2d_copy(ent->position, position);
    col_ent = ent;
    return ent;
}

//eof