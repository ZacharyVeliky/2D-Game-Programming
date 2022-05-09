#include "simple_logger.h"
//#include "time.h"
#include "../include/enemy.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"
#include "collision_ent.h"
#include "../include/energy_attack.h"
#include "simple_json.h"

#include "stdio.h"

#include "simple_logger.h"
#include "collision_ent.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"
#include "../include/player_ent.h"


SDL_Rect rect;

Uint32 last_attack_time;

int current_animation = 1;

void attack() {

    return;
}

void enemy_attack() {
    if (SDL_GetTicks() >= last_attack_time) {
        attack();
    }
}

void enemy_take_damage(Entity* self, int dam) {
    if (!self->is_invincible) {
        self->current_health -= dam;
        self->is_invincible = true;
        self->iframes = SDL_GetTicks() + 333;
        current_animation = 2;
    }
}

void enemy_think(Entity* self)
{
    //Vector2D direction;
    //int mx, my;

    if (!self)return;

    if (self->current_health <= 0) {
        player_get_exp(1);
        self->is_invincible = false;
        entity_free(self);
    }

    switch (current_animation)
    {
    case 1:
        self->frame = (self->frame + 0.05);
        if (self->frame >= 3)self->frame = 0;
        break;

    case 2:
        if (self->frame < 4 || self->frame >8)
            self->frame = 4;
        self->frame = (self->frame + 0.05);
        if (self->frame >= 8) {
            self->frame = 0;
            current_animation = 1;
        }
        break;

    default:
        self->frame = (self->frame + 0.05);
        if (self->frame >= 3)self->frame = 0;
        break;
    }

    if (SDL_GetTicks() >= self->iframes) {
        self->is_invincible = false;
        current_animation = 1;
    }
    
    Entity* ent;

    ent = collision_test_get_ent(self);
    if (ent != NULL) {
        if (ent->is_player) {
            if (ent->damage) {
                ent->damage(ent, 1);
            }
        }
    }
}

void enemy_update(Entity* self) {
    if (!self)return;

    rect.x = self->position.x - 10;
    rect.y = self->position.y - 10;
    rect.w = 20;
    rect.h = 20;
    self->bounds = rect;
}

Entity* enemy_ent_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }

    ent->sprite = gf2d_sprite_load_all("images/Enemies/fly/blue_fly.png", 8, 8, 8);
    ent->draw_offset.x = -8;//-172
    ent->draw_offset.y = -8;//-167
    ent->draw_scale = vector2d(2, 2);
    ent->think = enemy_think;
    ent->update = enemy_update;
    ent->damage = enemy_take_damage;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounds = rect;
    ent->current_health = 1;
    ent->is_invincible = false;
    ent->touch_player = true;
    vector2d_copy(ent->position, position);
    return ent;
}

//eof