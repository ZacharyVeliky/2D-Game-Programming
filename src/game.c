#include <SDL.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "entity.h"
#include "bug_ent.h"
#include "player_ent.h"
#include "collision_ent.h"

#include "tile_map.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = { 255,100,255,200 };
    TileMap *tilemap;

    //main collision areas
    SDL_Rect col_1 = { 0,0,32,32 };

    Sprite* health_background;
    Sprite* health;
    Vector2D health_scale = { 0.2,0.2 };
    Vector2D current_health_scale = { 0.2,0.2 };
    float currnet_player_health;
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    tile_set_manager_init(16);
    entity_manager_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    health_background = gf2d_sprite_load_image("images/health_background.png");
    health = gf2d_sprite_load_image("images/health.png");
    //bug_ent_new(vector2d(500,300));
    player_ent_new(vector2d(500,600));
    //,col_1 
    collision_ent_new(vector2d(700, 600));
    tilemap = tilemap_load("levels/testlevel.json");

    /*main game loop*/
    while(!done)
    {
        currnet_player_health = player_health_math();
        //slog("current hp %i", currnet_player_health);

        current_health_scale.x = currnet_player_health *.2;
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        entity_manager_think_all();
        entity_manager_update_all();
        //gf2d_sprite_draw_image(health_background, vector2d(0, 0));
        //gf2d_sprite_draw_image(health, vector2d(9, 9));
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            
            // draw other game elements
            tilemap_draw(tilemap);
            gf2d_sprite_draw(
                health_background,
                vector2d(10,10),
                &health_scale,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL);
            gf2d_sprite_draw(
                health,
                vector2d(11.9, 11.9),
                &current_health_scale,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL);
            entity_manager_draw_all();
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
       // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/