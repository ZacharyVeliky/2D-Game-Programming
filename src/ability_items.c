#include "simple_logger.h"
#include "time.h"
#include "ability_items.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"
#include "collision_ent.h"

#include "stdio.h"

//SDL_Rect rect;
int dir;

Vector2D scale = { 2,2 };

void energy_attack(Vector2D start, int player_dir) {
    dir = player_dir;
    energy_attack_new(start);
}

void energy_think(Entity* self)
{
    //Vector2D direction;
    //int mx, my;

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

Entity* ability_item_new(Vector2D position, int item_id)
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
    return ent;
}

//eof