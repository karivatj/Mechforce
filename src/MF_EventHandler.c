/* MF_EventHandler.c
 *
 * Mechforce Event Handler where all of the keyboard and
 * mouse events are handled.
 *
 * @author Kari Vatjus-Anttila <kari.vatjus-anttila@cie.fi>
 *
 */
#include "Mechforce.h"
#include "Map.h"
#include "Button.h"
#include "SDL_Engine.h"

/**
 * MF_Event_Handler is a function which
 * handles the events the user has made
 *
 * @return int returns in special cases an integer
 *
 */
int MF_Event_Handler(void)
{
     while(SDL_PollEvent(&event))
     {
        switch(event.type)
        {
            case SDL_QUIT:      //X was pressed
                    return (1);
            break;

            case SDL_KEYDOWN:	//If a button was pressed
                switch(event.key.keysym.sym)	//What was the button pressed?
                {
                    case SDLK_F1:
                        return (4);
                        break;
                    case SDLK_7:
                        MAP_GenerateMap();
                        break;
                    default:
                    break;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
                  MF_MouseEventPress(event.type, event.button.button, event.button.state, event.button.x, event.button.y);
            break;

            case SDL_MOUSEBUTTONUP:
                MF_MouseEventRelease(event.type, event.button.button, event.button.state, event.button.x, event.button.y);
            break;

            case SDL_MOUSEMOTION:
                MF_MouseEventMotion(event.type, event.button.button, event.button.state, event.motion.x, event.motion.y);
            break;

            default:
            break;
        }
    }

    return(0);
}

/**
 * MF_MouseEventPress is a function which
 * handles the mouse press events.
 *
 */
void MF_MouseEventPress(SDL_EventType type, int button, int buttonstate,  int x, int y)
{
    int cy = SCREEN_HEIGHT - y; /*Flip the coordinates.*/

    switch(button)
    {
        case SDL_BUTTON_LEFT:
            BTN_HandleButtonStateChanges(type, button, buttonstate, x, cy);
        break;

        case SDL_BUTTON_RIGHT:
            lastx = x;
            lasty = y;
        break;

        default:
        break;
    }
}

/**
 * MF_MouseEventRelease is a function which
 * handles the mouse release events.
 *
 */
void MF_MouseEventRelease(SDL_EventType type, int button, int buttonstate,  int x, int y)
{
    int cy = SCREEN_HEIGHT - y;

    lastx = x;
    lasty = y;

    if(button == SDL_BUTTON_LEFT)
    BTN_HandleButtonStateChanges(type,button,buttonstate,x,cy);
}

/**
 * MF_MouseEventMotion is a function which
 * handles the mouse motion events.
 *
 */
void MF_MouseEventMotion(SDL_EventType type, int button, int buttonstate,  int x, int y)
{
    float diffx = x - lastx;
    float diffy = y - lasty;

    lastx = x;
    lasty = y;

    if(button == SDL_BUTTON_LEFT && state == STATE_GAME)
    {/*
        rotx += (float) 0.5f * diffy;
        roty -= (float) 0.5f * diffx;
*/
        camerax -= (float) 0.50f * diffx;
        cameray += (float) 0.50f * diffy;

        printf("Camera X: %f Camera Y: %f\n",camerax, cameray);
    }

    int cy = SCREEN_HEIGHT - y;
    BTN_HandleButtonStateChanges(type,button,buttonstate,x,cy);
}
