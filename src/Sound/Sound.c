#include "Sound.h"
#include "../main.h"
#include "../Core/SDL_Engine.h"
#include "../Prefs/Preferences.h"

void Sound_LoadSounds(void)
{
    printf("Loading soundfiles.\n");
    sounds[0] = Mix_LoadWAV("../Sound/MouseClick.wav");
    sounds[1] = Mix_LoadWAV("../Sound/MouseOver.wav");
    sounds[2] = Mix_LoadWAV("../Sound/MouseRelease.wav");

    if(!sounds[0] || !sounds[1] || !sounds[2])
    {
        fprintf(stderr, "ERROR*** SDL_Mixer Error: %s\n", Mix_GetError());
        SDL_Close(-1);
    }

    printf("Loaded soundfiles succesfully.\n\n");
}

void Sound_PlaySound(int id)
{
    int channel = 0;

    if(pref_soundsoff != 1)
    {
        switch(id)
        {
            case 1: channel = Mix_PlayChannel(-1, sounds[0], 0); break;
            case 2: channel = Mix_PlayChannel(-1, sounds[1], 0); break;
            case 3: channel = Mix_PlayChannel(-1, sounds[2], 0); break;
            default: break;
        }

        if (channel == -1)
            fprintf(stderr, "SDL_Mixer Error : %s ", Mix_GetError());
    }
}

int Sound_Cleanup()
{
    int i;
    printf("Freeing Soundfiles");
    for(i = 0;i < MAX_SOUNDS; i++)
        Mix_FreeChunk(sounds[i]);

    Mix_CloseAudio();
    return 0;
}
