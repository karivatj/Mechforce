/*
 * Sound.h 1.00 by Kari Vatjus-Anttila
 *
 */
 
#ifndef SDL_SOUND_H
#define SDL_SOUND_H

#include "SDL/SDL_mixer.h"

#define MAX_SOUNDS      3

Mix_Chunk *sounds[MAX_SOUNDS];  /*Sound Array*/

void SDL_LoadSounds(void);
void SDL_PlaySound(int id);

#endif /*SDL_SOUND_H*/
