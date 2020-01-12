/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * SDL_Engine.h 1.00 by Kari Vatjus-Anttila
 *
 */
#pragma once

#ifndef SDL_ENGINE_H
#define SDL_ENGINE_H

#include "SDL/SDL.h"

/*Few Defines*/
#define BPP             16
#define FPS             60

#define TRUE 1
#define FALSE 0

extern SDL_Surface *screen;            /*The surface where the graphics will be drawn*/

extern float camerax, cameray, cameraz; /*Variables controlling the camera*/
extern float rotx, roty, lastx, lasty;

extern int flags;  //Variable that contains info about our videoflags.
extern int orthogonalEnabled;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

void Init_SDL(int width, int height);
void Init_GL(void);
void OrthogonalStart(void);
void OrthogonalEnd(void);
void glEnable3D(int width, int height);

void SDL_BuildDisplayLists(void);
void SDL_Close(int code);

void SDL_DrawScene(int width, int height);
void SDL_DrawTile(int index, int x, int y);
void SDL_DrawHUD(void);

#endif  //SDL_ENGINE_H
