#ifndef SDL_TEXTURES_H
#define SDL_TEXTURES_H

#include "FreeImage.h"

#define TILEMAP_WIDTH   4
#define TILEMAP_HEIGHT  4
#define TILE_WIDTH      32
#define TILE_HEIGHT     32
#define MAX_TILES       16

GLuint tiletexture[MAX_TILES];  //Texture array for tile textures
GLuint backgrounds[3];
GLuint buttontextures[14];

GLuint background;      //Displaylist variable for background and HUD
GLuint tile;            //Displaylist variable for states

GLuint Texture_GenerateGLTexture(BYTE *bits, int type, GLuint width, GLuint height);
GLuint Texture_LoadImage(char *filename, int type, int flag);
void Texture_GenerateTilemap(void);
void Texture_LoadTextures(void);
int Texture_Cleanup();

#endif /* SDL_TEXTURES_H */
