#include "simple_logger.h"
#include "player_ent.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"
#include "collision_ent.h"


float angle = -90;
float grav = -9.8;
SDL_Rect rect;

int player_health = 3;
int player_health_upgrade = 0;
int player_health_max;
int player_health_current = 3;

float player_health_math() {
    float ret = player_health_current / player_health_max;
    return ret;
}


void player_think(Entity* self)
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
    self->rotation.z = angle;

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    Entity* col = get_col_ent();

    if (collision_rect_test(self->bounds, col->bounds)) {
        slog("touch");
    }

    if (keys[SDL_SCANCODE_W] )
    {
        // jump
        //fix later
        //vector2d_set_magnitude(&self->position.y, 3);
        //vector2d_copy(self->velocity, self->position);
    }
    if (keys[SDL_SCANCODE_D] && !collision_rect_test_right(self->bounds, col->bounds))
    {
        // Right
        //fix later
        //vector2d_set_magnitude(&self->position.x, 3);
        //vector2d_copy(self->velocity, self->position);
        self->position.x += 1;
        angle = -90;
    }
    if (keys[SDL_SCANCODE_A] && !collision_rect_test_left(self->bounds, col->bounds))
    {
        // Left
        //fix later
        //vector2d_set_magnitude(&self->position.x, -3);
        //vector2d_copy(self->velocity, self->position);
        self->position.x -= 1;
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
    ent->sprite = gf2d_sprite_load_all("images/space_bug_top.png", 128, 128, 16);
    ent->think = player_think;
    ent->update = player_update;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounds = rect;
    vector2d_copy(ent->position, position);
    return ent;
}

//eof