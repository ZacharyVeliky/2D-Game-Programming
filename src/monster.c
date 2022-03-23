#include "simple_logger.h"
#include "time.h"
#include "../include/monster.h"
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

void monster_damage(int damage) {
    if (health_current >= 1)
        health_current -= damage;
    else
        slog("You Died");
}

void attack() {
    return;
}

void monster_attack() {
    if (SDL_GetTicks() >= last_attack_time)

        attack();
}

void monster_think(Entity* self)
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

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    Entity* col = get_col_ent();

    if (collision_rect_test(self->bounds, col->bounds)) {
        slog("touch");
    }
}

void monster_update(Entity* self) {
    if (!self)return;
    if (health_current <= 0)
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

Entity* monster_ent_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/space_bug_top.png", 128, 128, 16);
    //ent->sprite = gf2d_sprite_load_all("images/player.png", 120, 115, 11);
    ent->think = monster_think;
    ent->update = monster_update;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounds = rect;
    vector2d_copy(ent->position, position);
    return ent;
}

//eof