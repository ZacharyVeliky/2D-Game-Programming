#include "simple_logger.h"
//#include "time.h"
#include "player_ent.h"
#include "gfc_vector.h"
#include "gfc_audio.h"
#include "../include/gf2d_draw.h"
#include "collision.h"
#include "../include/energy_attack.h"
#include "simple_json.h"

#include "stdio.h"

SJson* json;

//double grav = -9.8;
SDL_Rect rect;


int player_health;
int player_health_current = 3;
int angle = 0;
int level = 1;
int req_exp = 3;
int current_exp = 0;

Bool has_energy_attack = 0;
Bool has_rocket_boots = 0;
Bool has_dash = 0;

Uint32 last_time;
Uint32 last_attack_time;
Uint32 last_jump_time;
Uint32 last_dash_time;

Sound* fireball;


typedef enum {
    IDLE,
    WALKING,
    JUMPING,
    FALLING,
    ATTACKING
} MovementState;

MovementState playerMovement = IDLE;

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

void player_damage(Entity* self, int damage) {
    if (self->current_health >= 1) {
        self->current_health -= damage;
        if (self->current_health <= 0)
            slog("You Died");
    }
    else
        slog("You Died");
}

void smack(Vector2D start, int owner_dir) {
    NULL;
}

void player_attack(int atkNum, Entity* player) {
    switch (atkNum)
    {

    case 1: //slog("Fire Ball");
        energy_attack(player->position, angle);
        break;

    case 2: slog("Smack");
        smack(player->position, player->is_mirror);
        break;

    case 3: slog("Dash");
        energy_attack(player->position, angle);
        break;
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
    
    if (playerMovement != ATTACKING) {
        switch (playerMovement)
        {
        case IDLE:
            self->frame = (self->frame + 0.1);
            if (self->frame >= 4)self->frame = 0;
            break;

        case WALKING:
            if (self->frame < 5 || self->frame >10)
                self->frame = 5;
            self->frame = (self->frame + 0.1);
            if (self->frame >= 10)self->frame = 5;
            break;

        case JUMPING:
            self->frame = (self->frame + 0.1);
            if (self->frame >= 4)self->frame = 0;
            break;

        case FALLING:
            self->frame = (self->frame + 0.1);
            if (self->frame >= 4)self->frame = 0;
            break;

        default:
            self->frame = (self->frame + 0.1);
            if (self->frame >= 4)self->frame = 0;
            break;
        }
    }
    else
    {
        self->frame = (self->frame + 0.1);
        if (self->frame >= 4) { 
            self->frame = 0;
            playerMovement = IDLE;
        };
    }
    

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    Vector2D direction = { self->position.y, self->position.y };
    Vector2D mag_position = { self->position.x, self->position.y };


    if (keys[SDL_SCANCODE_W] && self->position.y > 586 && has_rocket_boots)
    {
        // jump
        //fix later
        playerMovement = JUMPING;

        if (SDL_GetTicks() > last_jump_time + 100) {
            vector2d_set_magnitude(&self->position, self->position.y-1);
            vector2d_copy(self->velocity, direction);
        }

        last_jump_time = SDL_GetTicks();
    }
    if (keys[SDL_SCANCODE_D] && (collision_test_all_precise(self) != 3))
    {
        playerMovement = WALKING;
        self->position.x += 1;
        self->is_mirror = 0;
        angle = 0;
    }
    if (keys[SDL_SCANCODE_A] && (collision_test_all_precise(self) != 1))
    {
        playerMovement = WALKING;
        self->position.x -= 1;
        self->is_mirror = 1;
        angle = 90;
    }
    else
    {
        playerMovement = IDLE;
    }
    if (keys[SDL_SCANCODE_K]) {
        if (SDL_GetTicks() >= last_time + 1000) {
            //slog("time: %zu", last_time);
            player_damage(self, 1);
            last_time = SDL_GetTicks();
        }
    }
    if (keys[SDL_SCANCODE_SPACE] && (SDL_GetTicks() >= last_attack_time + 1000) && has_energy_attack) {
        gfc_sound_play(fireball, 0, 0.25, 2, 2);
        player_attack(1, self);
        last_attack_time = SDL_GetTicks();
    }
    if (keys[SDL_SCANCODE_F] && (SDL_GetTicks() >= last_attack_time + 1000) && has_energy_attack) {
        playerMovement = ATTACKING;
        player_attack(2, self);
        last_attack_time = SDL_GetTicks();
    }
    if (keys[SDL_SCANCODE_LSHIFT] && (SDL_GetTicks() >= last_dash_time + 500) && has_dash) {
        player_attack(3, self);
        last_attack_time = SDL_GetTicks();
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
    if (!collision_test_all_tiles(self) && self->position.y < 592 && !collision_test_all_ents(self) ) { //
        self->position.y++;
    }
}

void player_update(Entity* self) {
    if (!self)return;
    //Vector2D direction;
    //direction.x = 0 - self->position.x;
    //direction.y = 0 - self->position.y;

    rect.x = self->position.x - 16;
    rect.y = self->position.y - 16;
    rect.w = 32;
    rect.h = 32;
    Vector4D boxColor;
    boxColor.x = 255;
    boxColor.y = 255;
    boxColor.z = 255;
    boxColor.w = 255;
    gf2d_draw_rect(rect, boxColor);
    self->bounds = rect;
    //slog("A.x %i", self->bounds.x);
    //slog("A.y %i", self->bounds.y);
    //vector2d_set_magnitude(&self->velocity, grav);
    //vector2d_copy(self->velocity, self->position);
    //vector2d_copy(player_position, self->position);
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
    ent->sprite = gf2d_sprite_load_all("images/Player/player.png", 16, 16, 10);
    ent->think = player_think;
    ent->update = player_update;
    ent->damage = player_damage;
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
    sj_free(json);
    fireball = gfc_sound_load("Sound/fireball.wav", 0.5, 1);
    return ent;
}

//eof