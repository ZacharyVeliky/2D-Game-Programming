#include "simple_logger.h"
#include "collision_ent.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"


Entity* col_ent;
SDL_Rect rect;

void collision_think(Entity* self)
{
    Vector2D direction;
    int mx, my;

    const Uint8* keys;
    if (!self)return;

}

void collision_update(Entity* self) {
    if (!self)return;
    Vector2D direction;
    direction.x = 0 - self->position.x;
    direction.y = 0 - self->position.y;

    rect.x = self->position.x - 20;
    rect.y = self->position.y - 20;
    rect.w = 40;
    rect.h = 40;
    Vector4D rectBoxColor;
    rectBoxColor.x = 255;
    rectBoxColor.y = 255;
    rectBoxColor.z = 255;
    rectBoxColor.w = 255;
    gf2d_draw_rect(rect, rectBoxColor);
    self->bounds = rect;
    //slog("B.x %i", self->bounds.x);
    //slog("B.y %i", self->bounds.y);
    //vector2d_set_magnitude(&self->velocity, grav);
    //vector2d_copy(self->velocity, self->position);
}

Entity* collision_ent_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/space_bug_top.png", 128, 128, 16);
    ent->think = collision_think;
    ent->update = collision_update;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounds = rect;
    vector2d_copy(ent->position, position);
    col_ent = ent;
    return ent;
}

//eof