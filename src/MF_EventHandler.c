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
#include "SDL_Utils.h"
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
                    case SDLK_F12:
                        if(Utils_ScreenShot() == -1)
                            SDL_Close(-1);
                        break;
                    case SDLK_7:
                        MAP_GenerateMap();
                        break;
                    case SDLK_LEFT:
                        camerax += 5;
                        printf("camera x = %f camera y = %f camera z = %f\n", camerax, cameray, cameraz);
                        break;
                    case SDLK_RIGHT:
                        camerax -= 5;
                        break;
                    case SDLK_DOWN:
                        cameray += 5;
                        break;
                    case SDLK_UP:
                        cameray -= 5;
                        break;
                    default:
                    break;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button)
                {
                    case SDL_BUTTON_WHEELUP:
                    if((cameraz += 10) > -180)
                        cameraz = -180;
                    break;

                    case SDL_BUTTON_WHEELDOWN:
                    if((cameraz -= 10) < -500)
                        cameraz = -500;
                    break;
                    default:
                        MF_MouseEventPress(event.type, event.button.button, event.button.state, event.button.x, event.button.y);
                    break;
                }
            break;

            case SDL_MOUSEBUTTONUP:
                MF_MouseEventRelease(event.type, event.button.button, event.button.state, event.button.x, event.button.y);
            break;

            case SDL_MOUSEMOTION:
                MF_MouseEventMotion(event.type, event.button.button, event.motion.state, event.motion.x, event.motion.y);
            break;

            case SDL_VIDEORESIZE:
                printf("Window Resize Event\n");
#if 0
                screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 16, flags);

                SCREEN_WIDTH = event.resize.w;
                SCREEN_HEIGHT = event.resize.h;

                if(orthogonalEnabled)
                    OrthogonalStart();
                else
                    glEnable3D();
#endif
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
    float temp;

    lastx = x;
    lasty = y;

    if(button == SDL_BUTTON_LEFT && state == STATE_GAME)
    {
        temp = rotx;

        if((rotx += (float) 0.1f * diffy) < 0)
            rotx = 0;
        else if((rotx += (float) 0.1f * diffy) > 90)
            rotx = 90;

        roty -= (float) 0.1f * diffx;
    }

    int cy = SCREEN_HEIGHT - y;
    BTN_HandleButtonStateChanges(type, button, buttonstate, x, cy);
}
