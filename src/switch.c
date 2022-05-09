#include "simple_logger.h"
#include "collision_ent.h"
#include "gfc_vector.h"

#include "collision.h"
#include "../include/gf2d_draw.h"
#include "../include/player_ent.h"


Entity* col_ent;
SDL_Rect rect;

Bool is_activated = false;

void activate(Entity * ent) {
    
}

void switch_think(Entity* self)
{
    Vector2D direction;
    int mx, my;

    const Uint8* keys;
    if (!self)return;

}

void switch_update(Entity* self) {
    if (!self)return;

    Entity* ent;

    ent = collision_test_get_ent(self);
    if (ent != NULL) {
        if (!ent->is_player) {
            if (ent->damage) {
                ent->damage(ent, 1);
                entity_free(self);
            }
        }
    }

    rect.x = self->position.x - 20;
    rect.y = self->position.y - 20;
    rect.w = 40;
    rect.h = 40;
    self->bounds = rect;

}

Entity* switch_ent_new(Vector2D position, SDL_Rect bound)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/Objects/lever.png", 28, 16, 16);
    ent->update = switch_update;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounds = bound;
    ent->health = 1;
    vector2d_copy(ent->position, position);
    col_ent = ent;
    return ent;
}

//eof