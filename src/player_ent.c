#include "simple_logger.h"
#include "player_ent.h"
#include "gfc_vector.h"
#include "gfc_audio.h"
#include "../include/gf2d_draw.h"
#include "collision.h"

#include "../include/energy_attack.h"
#include "../include/smack.h"

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
Bool has_smack = 0;

Bool level_flag = false;

Bool can_cahnge_anim = true;

Uint32 last_time;
Uint32 last_attack_time;
Uint32 last_jump_time;
Uint32 last_dash_time;
Uint32 last_interact_time = 0;

Sound* fireball;

typedef enum {
    IDLE,
    WALKING,
    JUMPING,
    FALLING,
    ATTACKING,
    PAIN
} MovementState;

MovementState playerMovement = IDLE;

float player_health_math() {
    float ret = (float)player_health_current / (float)player_health;
    return ret;
}

float player_exp_math() {
    float ret = (float)current_exp / (float)req_exp;
    if (ret == 1) {
        level_flag = true;
        current_exp = 0;
        level++;
        req_exp++;
    }
    return ret;
}

void player_get_exp(int exp) {
    current_exp += exp;
}

void player_damage(Entity* self, int damage) {
    if (self->current_health >= 1) {
        if (!self->is_invincible) {
            self->current_health -= damage;
            self->is_invincible = true;
            self->iframes = SDL_GetTicks() + 333;
            if (self->current_health <= 0)
                slog("You Died");
        }
    }
    else
        slog("You Died");
}

void player_attack(int atkNum, Entity* player) {
    switch (atkNum)
    {

    case 1: //slog("Fire Ball");
        energy_attack(player->position, angle);
        break;

    case 2: //slog("Smack");
        smack_attack(player->position, angle);
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
    
    if (playerMovement != ATTACKING && playerMovement != PAIN) {
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
    else if (playerMovement == ATTACKING)
    {
        self->frame = (self->frame + 0.1);
        if (self->frame >= 14) { 
            self->frame = 0;
            can_cahnge_anim = true;
            playerMovement = IDLE;
        };
    }
    else if (playerMovement == PAIN) 
    {

    }
    
    if (SDL_GetTicks() >= self->iframes) {
        self->is_invincible = false;
    }

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    Vector2D direction = { 0, self->position.y};
    Vector2D mag_position = { self->position.x, self->position.y };



    if (keys[SDL_SCANCODE_W] && has_rocket_boots) //&& self->position.y > 586 
    {
        // jump
        //fix later
        playerMovement = JUMPING;

        if (SDL_GetTicks() > last_jump_time + 200) {
            self->position.y -= 50;
            //slog("%f", self->position.y);
            //vector2d_set_magnitude(&direction, self->position.y -3 );
            //slog("%f", direction.y);
            //vector2d_copy(self->velocity, direction);
            //
            //self->velocity.y -=1 ;
        }

        last_jump_time = SDL_GetTicks();
    }
    if (keys[SDL_SCANCODE_D] && (collision_test_all_precise(self) != 3))
    {
        if (can_cahnge_anim)
            playerMovement = WALKING;
        self->position.x += 1;
        self->is_mirror = 0;
        angle = 0;
    }
    if (keys[SDL_SCANCODE_A] && (collision_test_all_precise(self) != 1))
    {
        if (can_cahnge_anim)
            playerMovement = WALKING;
        self->position.x -= 1;
        self->is_mirror = 1;
        angle = 90;
    }
    else
    {
        if (can_cahnge_anim)
            playerMovement = IDLE;
    }
    if (keys[SDL_SCANCODE_K]) {
        if (SDL_GetTicks() >= last_time + 1000) {
            //slog("time: %zu", last_time);
            player_damage(self, 1);
            last_time = SDL_GetTicks();
        }
    }

    if (keys[SDL_SCANCODE_E] && SDL_GetTicks() >= last_interact_time) {
        self->is_interact = true;
        last_interact_time = SDL_GetTicks() + 200;
    }

    if (keys[SDL_SCANCODE_SPACE] && (SDL_GetTicks() >= last_attack_time + 1000) && has_energy_attack) {
        gfc_sound_play(fireball, 0, 0.25, 2, 2);
        player_attack(1, self);
        last_attack_time = SDL_GetTicks();
    }
    if (keys[SDL_SCANCODE_F] && (SDL_GetTicks() >= last_attack_time + 250) && has_smack) {
        playerMovement = ATTACKING;
        self->frame = 11;
        can_cahnge_anim = false;
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
            case 3:
                ent->is_collected = true;
                has_smack = true;
                break;
            default:
                break;
            }
        }
    }
    if (!collision_test_all_tiles(self) && self->position.y < 592 && !collision_test_all_ents(self) ) { // 592
        self->position.y += 1;
    }
    else
    {
        self->velocity.y++;
    }
}

void player_update(Entity* self) {
    if (!self)return;

    if (SDL_GetTicks() >= last_interact_time - 100)
        self->is_interact = false;

    if (level_flag) {
        self->health++;
        self->current_health = self->health;
        level_flag = false;
    }

    player_health_current = self->current_health;
    player_health = self->health;

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
    ent->sprite = gf2d_sprite_load_all("images/Player/player.png", 16, 16, 14);
    ent->think = player_think;
    ent->update = player_update;
    ent->damage = player_damage;
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->draw_scale = vector2d(2, 2);
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->velocity.x = 0;
    ent->velocity.y = 0;
    ent->bounds = rect;
    ent->is_player = true;
    ent->iframes = SDL_GetTicks()+333;
    vector2d_copy(ent->position, position);

    json = sj_load("entity/player.json");
    sj_get_integer_value(sj_object_get_value(json, "base_health"), &ent->health);
    ent->current_health = ent->health;
    sj_free(json);
    fireball = gfc_sound_load("Sound/fireball.wav", 0.5, 1);
    return ent;
}

//eof