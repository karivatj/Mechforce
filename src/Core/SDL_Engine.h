#ifndef SDL_ENGINE_H
#define SDL_ENGINE_H

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

/*Few Defines*/
#define BPP             16
#define FPS             60

#define TRUE 1
#define FALSE 0

SDL_Surface *screen;            /*The surface where the graphics will be drawn*/

float camerax, cameray, cameraz; /*Variables controlling the camera*/
float rotx, roty, lastx, lasty;

int flags;  //Variable that contains info about our videoflags.
int orthogonalEnabled;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

void Init_SDL(void);
void Init_GL(void);
void OrthogonalStart(void);
void OrthogonalEnd(void);
void glEnable3D(void);

void SDL_BuildDisplayLists(void);
void SDL_Close(int code);

void SDL_DrawScene(void);
void SDL_DrawTile(int index, int x, int y);
void SDL_DrawHUD(void);

#endif  //SDL_ENGINE_H
