/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * EventHandler.h 1.00 by Kari Vatjus-Anttila
 *
 */

#ifndef SDL_EVENTS_H
#define SDL_EVENTS_H

#include <SDL/SDL.h>

SDL_Event event;                /*Event Structure*/

int  MF_Event_Handler(void);

void MF_MouseEventPress(SDL_EventType type, int button, int buttonstate,  int x, int y);
void MF_MouseEventRelease(SDL_EventType type, int button, int buttonstate,int x, int y);
void MF_MouseEventMotion(SDL_EventType type, int button, int buttonstate, int x, int y);

#endif /*SDL_EVENTS_H*/
