/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * EventHandler.c by Kari Vatjus-Anttila
 *
 * Mechforce Event Handler where all of the keyboard and
 * mouse events are handled.
 *
 */

#include "main.h"
#include "Utils/Utilities.h"
#include "MapRoutines.h"
#include "Widget.h"
#include "EventHandler.h"

/**
 * MF_Event_Handler is a function which
 * handles the events the user has made
 *
 * @return int returns in special cases an integer
 *
 */
int MF_Event_Handler(void)
{
    SDL_Event event;
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
                    case SDLK_F12:
                        if(Utils_ScreenShot() == -1)
                            SDL_Close(-1);
                        break;
                    case SDLK_7:
                        Map_GenerateMap();
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
                        //cameraz -= 5;
                        break;
                    case SDLK_UP:
                        cameray -= 5;
                       // cameraz += 5;
                        break;
                    default:
                    break;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button)
                {
                    case SDL_BUTTON_WHEELUP:
                    if((cameraz += 10) > 200)
                        cameraz = -200;
                    break;

                    case SDL_BUTTON_WHEELDOWN:
                    if((cameraz -= 10) < -500)
                        cameraz = -500;
                    break;
                    default:
                        MF_MouseEventPress((SDL_EventType)event.type, event.button.button, event.button.state, event.button.x, event.button.y);
                    break;
                }
            break;

            case SDL_MOUSEBUTTONUP:
                MF_MouseEventRelease((SDL_EventType)event.type, event.button.button, event.button.state, event.button.x, event.button.y);
            break;

            case SDL_MOUSEMOTION:
                MF_MouseEventMotion((SDL_EventType)event.type, event.button.button, event.motion.state, event.motion.x, event.motion.y);
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
            Widget_HandleButtonStateChanges(type, button, buttonstate, x, cy);
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

    if(button == SDL_BUTTON_LEFT)
    Widget_HandleButtonStateChanges(type,button,buttonstate,x,cy);
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

    /*EXPERIMENTAL*/

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;

    glReadPixels(x, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    printf("X: %d Y: %d Z: %d\n", (int)posX, (int)posY, (int)posZ);

    /*EXPERIMENTAL END*/

    if(button == SDL_BUTTON_LEFT && state == STATE_GAME)
    {
        if((rotx += (float) 0.1f * diffy) < 0)
            rotx = 0;
        else if((rotx += (float) 0.1f * diffy) > 90)
            rotx = 90;

        roty -= (float) 0.1f * diffx;
    }

    int cy = SCREEN_HEIGHT - y;

    Widget_HandleButtonStateChanges(type, button, buttonstate, x, cy);
}
