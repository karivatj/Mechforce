#ifndef BUTTON_H
#define BUTTON_H

#define MAX_BUTTONS 15

#include "Mechforce.h"
#include "SDL/SDL.h"

/*Different kinds of button types*/
typedef struct {
    int type;
    int size;
    int group;
    int enabled;
    int mouseover;
    int x;
    int y;
    char caption[32];
    STATE tila;
    STATE kohdetila;
} BUTTON;    //Button datatype

BUTTON Buttons[MAX_BUTTONS];

int BTN_ReadButtonData(void);

BUTTON BTN_HandlePresses(BUTTON b, int button);
BUTTON BTN_HandleReleases(BUTTON b);
BUTTON BTN_HandleMouseOvers(BUTTON b, int button);
void BTN_HandleButtonStateChanges(SDL_EventType eventtype, int button, int buttonstate, int x, int y);

void BTN_DrawButtonScene(void);
void BTN_DrawButton(BUTTON b);

#endif /*BUTTON_H*/
