/*
 * Mechforce.h 1.01 by Kari Vatjus-Anttila
 *
 *
 * Currently implemented functions:
 *	- SDL_PlaySound(int id);
 *
 * Version history:
 * v1.0   - Initial prototype set.
 * v1.01  - Added SDL_PrintFont()
 * v1.02  - Added SDL_DrawScene()
 *          Added SDL_DrawGraphics();
 *          Added SDL_DrawText();
 * v1.03    Added SDL_StateMachine();
 */

#ifndef MECHFORCE_H
#define MECHFORCE_H

#include "SDL/SDL.h"

typedef enum {
    STATE_NULL,
    STATE_QUIT,
    STATE_MAINMENU,
    STATE_GAME,
    STATE_OPTIONS
} STATE;

extern STATE state;

STATE ResolveState(int id);
STATE ResolveButton(int id);

void MF_StateMachine(void);
void MF_MouseEventPress(SDL_EventType type, int button, int buttonstate,  int x, int y);
void MF_MouseEventRelease(SDL_EventType type, int button, int buttonstate,int x, int y);
void MF_MouseEventMotion(SDL_EventType type, int button, int buttonstate, int x, int y);
void MF_DrawHUD(void);
int  MF_Event_Handler(void);

#endif /* MECHFORCE_H */
