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

/*State enum specifies in what state the user is in a given time so we can act as expected*/
typedef enum {
    STATE_NULL,     /*Does nothing atm*/
    STATE_QUIT,     /*State when user presses exit or closes the program*/
    STATE_MAINMENU, /*As the name says, this is the state when the user idles in the main menu*/
    STATE_GAME,     /*State when the user presses the New Game button*/
    STATE_OPTIONS   /*State when the user presses the Options button*/
} STATE;

extern STATE state;



void MF_StateMachine(void); /*State machine where we check what state the user is in and act accordingly*/
void MF_MouseEventPress(SDL_EventType type, int button, int buttonstate,  int x, int y);
void MF_MouseEventRelease(SDL_EventType type, int button, int buttonstate,int x, int y);
void MF_MouseEventMotion(SDL_EventType type, int button, int buttonstate, int x, int y);
int  MF_Event_Handler(void);

#endif /* MECHFORCE_H */
