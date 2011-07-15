#include "Mechforce.h"
#include "SDL_Engine.h"

void SDL_LoadSounds(void)
{
    printf("Loading soundfiles.\n");
    sounds[0] = Mix_LoadWAV("../Sound/MouseClick.wav");
    sounds[1] = Mix_LoadWAV("../Sound/MouseOver.wav");
    sounds[2] = Mix_LoadWAV("../Sound/MouseRelease.wav");

    if(!sounds[0] || !sounds[1] || !sounds[2])
    {
        fprintf(stderr, "ERROR*** SDL_Mixer Error: %s\n", Mix_GetError());
    }
    printf("Loaded following soundfiles succesfully:\n\t../Sound/MouseClick.wav OK\n\t../Sound/MouseOver.wav OK\n\t../Sound/MouseRelease.wav OK\n\n");
}

void SDL_PlaySound(int id)
{
    int channel = 0;

    switch(id)
    {
        case 1:
            channel = Mix_PlayChannel(-1, sounds[0], 0);
        break;

        case 2:
            channel = Mix_PlayChannel(-1, sounds[1], 0);
        break;

        case 3:
            channel = Mix_PlayChannel(-1, sounds[2], 0);
        break;

        default:
        break;
    }

    if (channel == -1)
    fprintf(stderr, "SDL_Mixer Error : %s ", Mix_GetError());
}
