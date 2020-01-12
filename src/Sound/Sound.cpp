/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * Sound.c 1.00 by Kari Vatjus-Anttila
 *
 */

#include "../main.h"
#include "../Core/SDL_Engine.h"
#include "../Prefs/Preferences.h"
#include "Sound.h"

Mix_Chunk *sounds[MAX_SOUNDS];  /*Sound Array*/

void Sound_LoadSounds(void)
{
    #if 1
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
    #endif
}

void Sound_PlaySound(int id)
{
    #if 1
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
    #endif
}

int Sound_Cleanup()
{
    #if 1
    int i;
    printf("Freeing Soundfiles...\n");
    for(i = 0;i < MAX_SOUNDS; i++)
        Mix_FreeChunk(sounds[i]);

    Mix_CloseAudio();
    return 0;
    #endif
}
