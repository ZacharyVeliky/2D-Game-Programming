#include "simple_logger.h"
#include "time.h"
#include "energy_attack.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"
#include "collision.h"

Entity* energy_attack_new(start, owner_dir);


void energy_attack(Vector2D start, int owner_dir) {
    energy_attack_new(start, owner_dir);
}

void energy_think(Entity* self)
{
    Entity* ent;
    
    if (!self)return;

    //slog("about to colide");
    slog("i shot");
    ent = collision_test_get_ent(self);
    if (ent != NULL) {
        if (!ent->is_player) {
            if (ent->damage) {
                ent->damage(ent, 1);
                entity_free(self);
            }
        }
    }

    if (SDL_GetTicks() >= self->life + 999)
        entity_free(self);

    self->frame = (self->frame + 0.1);
    if (self->frame >= 4)self->frame = 0;
    if (self->direction == 90) {
        self->position.x -= 2;
        self->rotation.z = 180;
        self->draw_offset.x = -64;
    }
    else
        self->position.x += 2;
}

void energy_update(Entity* self) {
    if (!self)return;

    SDL_Rect rect2;
    rect2.x = self->position.x + 32;
    if (self->direction == 90)
        rect2.x = self->position.x - 64;
    rect2.y = self->position.y - 15;
    rect2.w = 32;
    rect2.h = 32;
    self->bounds = rect2;
}

Entity* energy_attack_new(Vector2D position, int dir)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/energy_attack.png", 32, 32, 4);
    ent->think = energy_think;
    ent->update = energy_update;
    ent->draw_offset.x = 0;
    ent->draw_offset.y = -32;
    ent->rotation.x = 16;
    ent->rotation.y = 16;
    ent->direction = dir;
    ent->draw_scale = vector2d(2, 2);
    vector2d_copy(ent->position, position);
    ent->life = SDL_GetTicks();
    return ent;
}

//eof