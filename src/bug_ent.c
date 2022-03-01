#include "simple_logger.h"
#include "bug_ent.h"
#include "gfc_vector.h"

void bug_think(Entity *self)
{
    Vector2D direction;
    int mx,my;
    float angle;
    const Uint8 *keys;
    if (!self)return;
    self->frame = (self->frame + 0.1);
    if (self->frame >= 16)self->frame = 0;
    
    SDL_GetMouseState(&mx,&my);
    direction.x = mx - self->position.x;
    direction.y = my - self->position.y;
    angle = vector2d_angle(direction) - 90;
    self->rotation.z = angle;
    
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    
    if (keys[SDL_SCANCODE_W])
    {
        // move forward
        vector2d_set_magnitude(&direction,3);
        vector2d_copy(self->velocity,direction);
    }
    else
    {
        vector2d_scale(self->velocity,self->velocity,0.5);
        if (vector2d_magnitude(self->velocity)<0.05)
        {
            vector2d_clear(self->velocity);
        }
    }

}

void bug_update(Entity* self) {
    if (!self)return;
    entity_update(self);
}

Entity *bug_ent_new(Vector2D position)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/space_bug_top.png",128,128,16);
    ent->think = bug_think;
    ent->update = bug_update;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    //vector2d_set(ent->bounds, -32,32)
    vector2d_copy(ent->position,position);
    return ent;
}

//eof