#ifndef SDL_ENGINE_H
#define SDL_ENGINE_H

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_opengl.h"
#include "FTGL/ftgl.h"

/*Few Defines*/
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600
#define BPP             16
#define FPS             60  //FPS Limit
#define MAX_SOUNDS      3
#define MAX_TILES       16
#define MAX_TXT_EVENTS  1024
#define MAX_FONTS       2
#define TILEMAP_WIDTH   4
#define TILEMAP_HEIGHT  4
#define TILE_WIDTH      32
#define TILE_HEIGHT     32

#define TRUE 1
#define FALSE 0

/*SDL Stuff*/
SDL_Surface *screen;            /*The surface where the graphics will be drawn*/
SDL_Event event;                /*Event structure*/
Mix_Chunk *sounds[MAX_SOUNDS];
FTGLfont *font[MAX_FONTS];

typedef struct {
    int reserved;
    int x;
    int y;
    int r;
    int g;
    int b;
    int size;
    int font;
    char text[128];
} TXT_EVENTS;

TXT_EVENTS text_events[MAX_TXT_EVENTS];

float camerax, cameray, cameraz;
float rotx, roty, lastx, lasty;

/*FPS Stuff*/
GLfloat fps;
GLfloat framestart;
/***********/

GLuint background;      //Displaylist variable for background and HUD
GLuint tile;            //Displaylist variable for states

GLuint tiletexture[MAX_TILES];  //Texture array for tile textures
GLuint backgrounds[3];
GLuint buttontextures[14];

void Init_SDL(void);
void Init_GL(void);
void OrthogonalStart(void);
void OrthogonalEnd(void);
void glEnable3D(void);
void Print(int size, double x, double y, double r, double g, double b, int fontindex, const char *msg);
GLuint glGenerateTexture(SDL_Surface *surface, int type);

void SDL_BuildDisplayLists(void);
void SDL_Close(int code);


void SDL_DrawScene(void);
void SDL_DrawText(int size, int x, int y, double r, double g, double b, int font, char *msg, ...);
void SDL_DrawTile(int index, int x, int y);
void SDL_DrawHUD(void);

void SDL_GenerateTilemap(void);
void SDL_LoadTextures(void);
void SDL_LoadSounds(void);
void SDL_PlaySound(int id);


#endif  //SDL_ENGINE_H
