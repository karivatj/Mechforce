#include "Mechforce.h"
#include "Button.h"
#include "SDL_Engine.h"

void MF_MouseEventPress(SDL_EventType type, int button, int buttonstate,  int x, int y)
{
    int cy = HEIGHT - y;
    switch(button)
    {
        case SDL_BUTTON_LEFT:
            BTN_HandleButtonStateChanges(type,button,buttonstate,x,cy);
        break;

        case SDL_BUTTON_RIGHT:
            lastx = x;
            lasty = y;
        break;

        default:
        break;
    }
}

void MF_MouseEventRelease(SDL_EventType type, int button, int buttonstate,  int x, int y)
{
    int cy = HEIGHT - y;
    lastx = x;
    lasty = y;
    BTN_HandleButtonStateChanges(type,button,buttonstate,x,cy);
}

void MF_MouseEventMotion(SDL_EventType type, int button, int buttonstate,  int x, int y)
{
    float diffx = x - lastx;
    float diffy = y - lasty;

    lastx = x;
    lasty = y;
/*
    if(button == 1 && state == STATE_GAME)
    {
        rotx += (float) 0.5f * diffy;
        roty -= (float) 0.5f * diffx;

        camerax -= (float) 0.05f * diffx;
        cameray += (float) 0.05f * diffy;
    }
*/
    int cy = HEIGHT - y;    //K‰‰nnet‰‰n y akseli ymp‰ri ku SDL:ss‰ se o erip‰in mit‰ OpenGL:s‰
    BTN_HandleButtonStateChanges(type,button,buttonstate,x,cy);
}
