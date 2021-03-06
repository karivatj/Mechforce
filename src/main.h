/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * main.h 1.00 by Kari Vatjus-Anttila
 *
 */
#pragma once

#ifndef MECHFORCE_H
#define MECHFORCE_H

#ifdef WIN32
#include <windows.h>
#endif

#include "FreeImage.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include "Core/SDL_Engine.h"

/*State enum specifies in what state the user is in a given time so we can act as expected*/
typedef enum {
    STATE_NULL,         /*Does nothing atm*/
    STATE_QUIT,         /*State when user presses exit or closes the program*/
    STATE_MAINMENU,     /*As the name says, this is the state when the user idles in the main menu*/
    STATE_GAME,         /*State when the user presses the New Game button*/
    STATE_OPTIONS,      /*State when the user presses the Options button*/
    STATE_RELOAD_CONFIG
} STATE;

extern STATE state;

#endif /* MECHFORCE_H */
