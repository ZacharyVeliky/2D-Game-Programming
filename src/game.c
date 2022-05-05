#include <SDL.h>

#include "../include/chipmunk/chipmunk.h"

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "SDL_ttf.h"

#include "entity.h"
#include "bug_ent.h"
#include "player_ent.h"
#include "collision_ent.h"
#include "../include/ability_items.h"

#include "tile_map.h"

int main(int argc, char * argv[])
{
    /*cpVect gravity = cpv(0, -100);
    cpSpace* space = cpSpaceNew();
    cpSpaceSetGravity(space, gravity);*/

    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;

    Bool isDrawingMenu = 0;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = { 255,100,255,200 };
    TileMap *tilemap;

    //args for map editor goes here

    Sprite* health_background;
    Sprite* health;
    Sprite* exp;
    Vector2D health_scale = { 0.2,0.1 };
    Vector2D current_health_scale = { 0.2,0.1 };
    Vector2D current_exp_scale = { 0.2,0.1 };
    Vector2D bg_scale = { 5, 5 };
    float currnet_player_health;
    float currnet_player_exp;

    Uint32 spawn_time = 0;
    
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
    tile_set_manager_init(64);
    entity_manager_init(512);

    TTF_Init();
    TTF_Font* Elfboy = TTF_OpenFont("Fonts/Elfboyclassic.ttf",32);

    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/background.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    health_background = gf2d_sprite_load_image("images/health_background.png");
    health = gf2d_sprite_load_image("images/health.png");
    exp = gf2d_sprite_load_image("images/experience.png");
    //bug_ent_new(vector2d(500,300));
    player_ent_new(vector2d(500,587));
    collision_ent_new(vector2d(700, 580));
    collision_ent_new(vector2d(200, 580));
    ability_item_new(vector2d(400, 580),1);
    ability_item_new(vector2d(50, 580),2);
    tilemap = tilemap_load("levels/test.json");

    SDL_Color clrFg = { 0,0,255,0 };  // Blue ("Fg" is foreground)
    SDL_Color clrBg = { 0,0,0,0 };  // Blue ("Fg" is foreground)
    SDL_Surface* text = TTF_RenderText_Blended(Elfboy, "Courier 12", clrFg);
    SDL_Rect rcDest = { 0,0,100,100 };
    SDL_BlitSurface(text, NULL, gf2d_graphics_screen_convert, &rcDest);
    

    /*main game loop*/
    while(!done)
    {
        currnet_player_health = player_health_math();
        current_health_scale.x = 0.2 * currnet_player_health;
        
        currnet_player_exp = player_exp_math();
        current_exp_scale.x = 0.2 * currnet_player_exp;
        if (SDL_GetTicks() >= spawn_time + 10000) {
            collision_ent_new(vector2d(800, 580));
            collision_ent_new(vector2d(900, 580));
            spawn_time = SDL_GetTicks();
        }


        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        if (!isDrawingMenu) {
            entity_manager_think_all();
            entity_manager_update_all();
        }

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw(
                sprite,
                vector2d(0,0),
                &bg_scale,
                NULL, NULL, NULL, NULL, NULL);
            // draw other game elements
            if (!isDrawingMenu) {
                tilemap_draw(tilemap);
                gf2d_sprite_draw(
                    health_background,
                    vector2d(10, 10),
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

                gf2d_sprite_draw(
                    health_background,
                    vector2d(10, 30),
                    &health_scale,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL);

                gf2d_sprite_draw(
                    exp,
                    vector2d(11.9, 31.9),
                    &current_exp_scale,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL);

                entity_manager_draw_all();
                //UI elements last
            }
            
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);    
            
            //(sText, gf2d_graphics_get_renderer, 0, 0);
            

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
       // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    SDL_FreeSurface(text);
    TTF_CloseFont(Elfboy);
    TTF_Quit();
    return 0;
}
/*eol@eof*/