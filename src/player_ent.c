#include "simple_logger.h"
#include "time.h"
#include "player_ent.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"
#include "collision.h"
#include "../include/energy_attack.h"
#include "simple_json.h"

#include "stdio.h"

SJson* json;

float angle = 0;
float grav = -9.8;
SDL_Rect rect;

Vector2D player_position;

int player_health;
int player_health_current = 3;

int level = 1;
int req_exp = 3;
int current_exp = 0;

Bool has_energy_attack = 0;
Bool has_rocket_boots = 0;

Uint32 last_time;
Uint32 attack_last_time;
Uint32 last_jump_time;

float player_health_math() {
    float ret = (float)player_health_current / (float)player_health;
    return ret;
}

float player_exp_math() {
    float ret = (float)current_exp / (float)req_exp;
    if (ret == 1) {
        current_exp = 0;
        level++;
        player_health++;
        req_exp++;
        player_health_current = player_health;
    }
    return ret;
}

void player_get_exp(int exp) {
    current_exp += exp;
}

void player_damage(int damage) {
    if (player_health_current >= 1) {
        player_health_current -= damage;
        if (player_health_current <= 0)
            slog("You Died");
    }
    else
        slog("You Died");
}

void player_attack(int atkNum, Entity* player) {
    switch (atkNum)
    {
    case 1: energy_attack(player_position, angle, player);
    default:
        break;
    }
}

void player_think(Entity* self)
{
    if (!self)return;

    //Vector2D direction;
    //int mx, my;
    Entity* ent;
    const Uint8* keys;
    
    self->frame = (self->frame + 0.1);
    if (self->frame >= 4)self->frame = 0;

    //self->rotation.z = angle;

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    //Entity* col = get_col_ent();
    Vector2D direction = { self->position.x, self->position.y };
    Vector2D mag_position = { self->position.x, self->position.y };
    //if (collision_test_all(self)) {
    //    slog("touch");
    //}

    if (keys[SDL_SCANCODE_W] && self->position.y > 586 && has_rocket_boots)
    {
        // jump
        //fix later

        if (SDL_GetTicks() > last_jump_time + 100) {
            vector2d_set_magnitude(&self->position, self->position.y-1);
            vector2d_copy(self->velocity, direction);
        }

        last_jump_time = SDL_GetTicks();
    }
    if (keys[SDL_SCANCODE_D] && (collision_test_all_precise(self) != 3))
    {
        // Right
        //fix later
        //vector2d_set_magnitude(&self->position.x, 3);
        //vector2d_copy(self->velocity, self->position);
        self->position.x += 1;
        self->is_mirror = 0;
        angle = 0;
    }
    if (keys[SDL_SCANCODE_A] && (collision_test_all_precise(self) != 1))
    {
        // Left
        //fix later
        //vector2d_set_magnitude(&self->position.x, -3);
        //vector2d_copy(self->velocity, self->position);
        self->position.x -= 1;
        self->is_mirror = 1;
        angle = 90;
    }
    else
    {
        vector2d_scale(self->velocity, self->velocity, 0.5);
        if (vector2d_magnitude(self->velocity) < 0.05)
        {
            vector2d_clear(self->velocity);
        }
    }
    if (keys[SDL_SCANCODE_K]) {
        if (SDL_GetTicks() >= last_time + 1000) {
            //slog("time: %zu", last_time);
            player_damage(1);
            last_time = SDL_GetTicks();
        }
    }
    if (keys[SDL_SCANCODE_SPACE] && (SDL_GetTicks() >= attack_last_time + 1000) && has_energy_attack) {
        player_attack(1, self);
        attack_last_time = SDL_GetTicks();
    }
    //collect items
    ent = collision_test_get_ent(self);
    if (ent) {
        if (ent->is_item) {
            switch (ent->item_id)
            {
            case 1:
                ent->is_collected = true;
                has_energy_attack = true;
                break;
            case 2:
                ent->is_collected = true;
                has_rocket_boots = true;
                break;
            default:
                break;
            }
        }
    }
    if (!collision_test_all_tiles(self) && self->position.y < 586 && !collision_test_all_ents(self) ) { //
        self->position.y++;
    }
}

void player_update(Entity* self) {
    if (!self)return;
    //Vector2D direction;
    //direction.x = 0 - self->position.x;
    //direction.y = 0 - self->position.y;

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
    //slog("A.x %i", self->bounds.x);
    //slog("A.y %i", self->bounds.y);
    vector2d_set_magnitude(&self->velocity, grav);
    vector2d_copy(self->velocity, self->position);
    vector2d_copy(player_position, self->position);
}



Entity* player_ent_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/Player/player_idle.png", 16, 16, 4);
    ent->think = player_think;
    ent->update = player_update;
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->draw_scale = vector2d(2, 2);
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounds = rect;
    ent->is_player = true;
    vector2d_copy(ent->position, position);

    json = sj_load("entity/player.json");
    sj_get_integer_value(sj_object_get_value(json, "base_health"), &player_health);
    slog("base: %d", player_health);   
    slog("current: %d", player_health_current);
    sj_free(json);
    return ent;
}

//eof