#include <SDL.h>
#include <SDL_mixer.h>
#include "simple_logger.h"

#include "sound.h"

void sound_init(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8  enableMP3, Uint8  enableOgg){
    int flags = 0;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        slog("Failed to open audio: %s\n", SDL_GetError());
        return;
    }

    if (enableMP3)
    {
        flags |= MIX_INIT_MP3;
    }
    if (enableOgg)
    {
        flags |= MIX_INIT_OGG;
    }
    if (!(Mix_Init(flags) & flags))
    {
        slog("failed to initialize some audio support: %s", SDL_GetError());
    }
    atexit(Mix_Quit);
    atexit(gf2d_audio_close);
    gf2d_sound_init(maxSounds);
    slog("audio system initialized");
}