/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * Sound.h 1.00 by Kari Vatjus-Anttila
 *
 */
 
#ifndef SDL_SOUND_H
#define SDL_SOUND_H

#include "SDL/SDL_mixer.h"

#define MAX_SOUNDS      3

Mix_Chunk *sounds[MAX_SOUNDS];  /*Sound Array*/

void Sound_LoadSounds(void);
void Sound_PlaySound(int id);
int Sound_Cleanup();

#endif /*SDL_SOUND_H*/
