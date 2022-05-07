#include "simple_logger.h"
#include "time.h"
#include "energy_attack.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"
#include "collision.h"

#include "stdio.h"

//SDL_Rect rect;
int dir;

Vector2D scale = { 2,2 };

Entity* parent;

Uint32 life = 0;


void energy_attack(Vector2D start, int owner_dir, Entity* owner) {
    dir = owner_dir;
    parent = owner;
    energy_attack_new(start);
    
}

void energy_think(Entity* self)
{
    //Vector2D direction;
    //int mx, my;
    Entity* ent;
    //slog("about to colide");
    ent = collision_test_get_ent(self);
    if (ent) {
        if (!ent->is_player) {
            if (ent->damage) {
                ent->damage(ent, 1);
                //slog("i did the damage");
                entity_free(self);
            }
        }
    }
    if (SDL_GetTicks() >= life + 999)
        entity_free(self);

    if (!self)return;
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
    //Vector2D direction;
    //direction.x = 0 - self->position.x;
    //direction.y = 0 - self->position.y;

    SDL_Rect rect2;
    rect2.x = self->position.x + 32;
    if (dir == 90)
        rect2.x = self->position.x - 64;
    rect2.y = self->position.y - 15;
    rect2.w = 32;
    rect2.h = 32;
    //Vector4D boxColor;
    //boxColor.x = 255;
    //boxColor.y = 255;
    //boxColor.z = 255;
    //boxColor.w = 255;
    //gf2d_draw_rect(rect2, boxColor);
    self->bounds = rect2;
}

Entity* energy_attack_new(Vector2D position)
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
    //ent->bounds = rect;
    ent->direction = dir;
    ent->draw_scale = scale;
    vector2d_copy(ent->position, position);
    life = SDL_GetTicks();
    return ent;
}

//eof