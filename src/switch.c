#include "simple_logger.h"
#include "collision_ent.h"
#include "gfc_vector.h"

#include "collision.h"
#include "../include/gf2d_draw.h"
#include "../include/switch.h"
#include "../include/door.h"


SDL_Rect rect;

Bool is_activated = false;
Uint32 last_activate = 0;

void activate(Entity * self) {
    //slog("almost there");
    if (SDL_GetTicks() >= last_activate) {
        slog("activate");
        if (is_activated) {
            self->is_flipped = false;
            open();
        }
        else {
            self->is_flipped = true;
        }
        last_activate = SDL_GetTicks() + 150;
    }
}

void switch_update(Entity* self) {
    if (!self)return;

    if (!self->is_flipped) {
        self->frame = 1;
    }
    else
        self->frame = 2;


    Entity* ent;

    
    ent = collision_test_get_ent(self);
    if (ent != NULL) {
        if (ent->is_player) {
            if (ent->is_interact) {
                //slog("flip");
                activate(self);
            }
        }
    }

    rect.x = self->position.x - 16;
    rect.y = self->position.y - 6;
    rect.w = 60;
    rect.h = 40;
    self->bounds = rect;

}

Entity* switch_ent_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/Objects/lever.png", 16, 16, 3);
    ent->update = switch_update;
    ent->draw_offset.x = 0;
    ent->draw_offset.y = 0;
    ent->rotation.x = 0;
    ent->rotation.y = 0;
    ent->health = 1;
    ent->draw_scale = vector2d(2, 2);
    ent->switch_id = 1;
    ent->is_flipped = 0;
    vector2d_copy(ent->position, position);
    return ent;
}

//eof