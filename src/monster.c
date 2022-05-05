#include "simple_logger.h"
#include "time.h"
#include "../include/enemy.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"
#include "collision_ent.h"
#include "../include/energy_attack.h"
#include "simple_json.h"

#include "stdio.h"

float angle2 = -90;
SDL_Rect rect;

Vector2D player_position;

int health;
int health_current;

Uint32 last_attack_time;

void enemy_damage(int damage) {
    if (health_current >= 1)
        health_current -= damage;
    else
        slog("You Died");
}

void attack() {
    return;
}

void enemy_attack() {
    if (SDL_GetTicks() >= last_attack_time)
        attack();
}

void enemy_think(Entity* self)
{
    //Vector2D direction;
    //int mx, my;

    const Uint8* keys;
    if (!self)return;
    self->frame = (self->frame + 0.1);
    if (self->frame >= 16)self->frame = 0;

    //SDL_GetMouseState(&mx, &my);
    //direction.x = mx - self->position.x;
    //direction.y = my - self->position.y;
    //angle = vector2d_angle(direction) - 90;
    self->rotation.z = angle2;
}

void enemy_update(Entity* self) {
    if (!self)return;
    if (self->health <= 0)
        entity_free(self);

    rect.x = self->position.x - 20;
    rect.y = self->position.y - 20;
    rect.w = 40;
    rect.h = 40;
    Vector4D boxColor;
    boxColor.x = 255;
    boxColor.y = 255;
    boxColor.z = 255;
    boxColor.w = 255;
    gf2d_draw_rect(rect, boxColor);
    self->bounds = rect;
    health_current = self->current_health;
    //slog("A.x %i", self->bounds.x);
    //slog("A.y %i", self->bounds.y);
    //vector2d_set_magnitude(&self->velocity, grav);
    //vector2d_copy(self->velocity, self->position);
    //vector2d_copy(player_position, self->position);
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
    ent->sprite = gf2d_sprite_load_all("images/Enemies/fly/blue_fly_idle_.png", 128, 128, 16);
    ent->think = enemy_think;
    ent->update = enemy_update;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounds = rect;
    ent->health = 2;
    vector2d_copy(ent->position, position);
    return ent;
}

//eof