#include "simple_logger.h"
#include "time.h"
#include "ability_items.h"
#include "gfc_vector.h"
#include "../include/gf2d_draw.h"
#include "collision.h"

#include "stdio.h"

//SDL_Rect rect;
int dir;
SDL_Rect rect;

//Vector2D scale = { 2,2 };

void ability_think(Entity* self)
{
    //Vector2D direction;
    //int mx, my;

    if (!self)return;
    self->frame = (self->frame + 0.1);
    if (self->frame >= 0)self->frame = 0;
}

void ability_update(Entity* self) {
    if (!self)return;

    rect.x = self->position.x + 32;
    rect.y = self->position.y - 15;
    rect.w = 32;
    rect.h = 32;
    //Vector4D boxColor;
    //boxColor.x = 255;
    //boxColor.y = 255;
    //boxColor.z = 255;
    //boxColor.w = 255;
    //gf2d_draw_rect(rect2, boxColor);
    self->bounds = rect;
    if (self->is_collected) {
        entity_free(self);
    }
}

Entity* ability_item_new(Vector2D position, int item_id)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for more ents");
        return NULL;
    }switch (item_id)
    {
    case 1: ent->sprite = gf2d_sprite_load_all("images/Items/wand_of_energy.png", 1000, 1000, 4); 
            ent->draw_offset.x = 20;//-172
            ent->draw_offset.y = -20;//-167
            ent->draw_scale = vector2d(0.05, 0.05);
        break;
    case 2: ent->sprite = gf2d_sprite_load_all("images/Items/rocket_boots.png", 256, 256, 4); 
            ent->draw_offset.x = +25;//-172
            ent->draw_offset.y = -20;//-167
            ent->draw_scale = vector2d(0.2, 0.2);
            break;
    case 3: ent->sprite = gf2d_sprite_load_all("images/bomb.png", 32, 32, 4); 
            
        break;
    case 4: ent->sprite = gf2d_sprite_load_all("images/laser_gun.png", 32, 32, 4); break;
    case 5: ent->sprite = gf2d_sprite_load_all("images/laser_gun.png", 32, 32, 4); break;

    default:ent->sprite = gf2d_sprite_load_all("images/laser_gun.png", 32, 32, 4);
            ent->draw_offset.x = -20;//-172
            ent->draw_offset.y = -20;//-167    
        break;
    }
    ent->think = ability_think;
    ent->update = ability_update;
    ent->is_item = true;
    ent->item_id = item_id;
    //ent->bounds = rect;
    ent->direction = dir;
    ent->is_collected = false;
    //ent->draw_scale = scale;
    vector2d_copy(ent->position, position);
    return ent;
}

void collect_item() {

}
//eof