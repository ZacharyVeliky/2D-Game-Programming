#include "simple_logger.h"
#include "time.h"
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


Entity* col_ent;
SDL_Rect rect;

Uint32 last_attack_time;

typedef enum {
    IDLE,
    WALKING,
    JUMPING,
    FALLING,
};

void attack() {

    return;
}

void enemy_attack() {
    if (SDL_GetTicks() >= last_attack_time) {
        attack();
    }
}

void enemy_take_damage(Entity* self, int dam) {
    //if (!self->is_invincible) {
        self->current_health -= dam;

    //}
}

void enemy_think(Entity* self)
{
    //Vector2D direction;
    //int mx, my;

    const Uint8* keys;
    if (!self)return;
    self->frame = (self->frame + 0.05);
    if (self->frame >= 3)self->frame = 0;

    //SDL_GetMouseState(&mx, &my);
    //direction.x = mx - self->position.x;
    //direction.y = my - self->position.y;
    //angle = vector2d_angle(direction) - 90;
    //self->rotation.z = angle2;
}

void enemy_update(Entity* self) {
    if (!self)return;
    if (self->health <= 0) {
        player_get_exp(1);
        entity_free(self);
    }

    //if ()

    rect.x = self->position.x - 10;
    rect.y = self->position.y - 10;
    rect.w = 20;
    rect.h = 20;
    self->bounds = rect;
    //slog("A.x %i", self->bounds.x);
    //slog("A.y %i", self->bounds.y);
    //vector2d_set_magnitude(&self->velocity, grav);
    //vector2d_copy(self->velocity, self->position);
    //vector2d_copy(player_position, self->position);
}

Entity* enemy_ent_new(Vector2D position, int enemyID)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }

    ent->sprite = gf2d_sprite_load_all("images/Enemies/fly/blue_fly_idle.png", 8, 8, 3);
    ent->draw_offset.x = -8;//-172
    ent->draw_offset.y = -8;//-167
    ent->draw_scale = vector2d(2, 2);
    ent->think = enemy_think;
    ent->update = enemy_update;
    ent->damage = enemy_take_damage;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounds = rect;
    ent->health = 2;
    ent->is_invincible = false;
    vector2d_copy(ent->position, position);
    return ent;
}

//eof