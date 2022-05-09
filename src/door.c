#include "simple_logger.h"
#include "collision_ent.h"
#include "gfc_vector.h"

#include "collision.h"
#include "../include/gf2d_draw.h"


SDL_Rect rect;

Uint32 last_enter = 0;

Bool is_open = false;

void open(){
    if (is_open)
        is_open = false;
    else
        is_open = true;
}

void enter() {
    if (SDL_GetTicks() >= last_enter) {
        if (is_open) {
            is_open = false;
        }
        else {
            is_open = true;
        }
        last_enter = SDL_GetTicks() + 150;
    }
}

void door_update(Entity* self) {
    if (!self)return;

    if (!is_open) {
        self->frame = 1;
    }
    else
        self->frame = 2;

    Entity* ent;

    ent = collision_test_get_ent(self);
    if (ent != NULL) {
        if (ent->is_player) {
            if (ent->is_interact) {
                enter();
            }
        }
    }

    rect.x = self->position.x - 7;
    rect.y = self->position.y - 6;
    rect.w = 60;
    rect.h = 60;
    self->bounds = rect;
}

Entity* door_ent_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/Objects/door.png", 24, 24, 3);
    ent->update = door_update;
    ent->draw_offset.x = 0;
    ent->draw_offset.y = 0;
    ent->rotation.x = 0;
    ent->rotation.y = 0;
    ent->health = 1;
    ent->draw_scale = vector2d(2, 2);
    vector2d_copy(ent->position, position);
    return ent;
}

//eof