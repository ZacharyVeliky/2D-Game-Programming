#include "simple_logger.h"
#include "collision_ent.h"
#include "gfc_vector.h"

#include "collision.h"
#include "../include/gf2d_draw.h"
#include "../include/player_ent.h"


SDL_Rect rect;

int mx, my;

void start_update(Entity* self) {
    if (!self)return;

    SDL_GetMouseState(&mx, &my);
    


    rect.x = self->position.x - 0;
    rect.y = self->position.y - 0;
    rect.w = 288;
    rect.h = 96;
    self->bounds = rect;
}

Entity* start_ui_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/UI/start.png", 144, 48, 3);
    ent->update = start_update;
    ent->draw_offset.x = 0;
    ent->draw_offset.y = 0;
    ent->draw_scale = vector2d(2, 2);
    ent->touch_player = false;
    ent->is_ui = true;
    vector2d_copy(ent->position, position);
    return ent;
}

//eof