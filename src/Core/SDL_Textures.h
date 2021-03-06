/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * SDL_Textures.h 1.00 by Kari Vatjus-Anttila
 *
 */
#pragma once

#ifndef SDL_TEXTURES_H
#define SDL_TEXTURES_H

#include "../main.h"
#include <string>
#include <GL/gl.h>

#define TILEMAP_WIDTH   4
#define TILEMAP_HEIGHT  4
#define TILE_WIDTH      32
#define TILE_HEIGHT     32
#define MAX_TILES       16

extern GLuint tiletexture[MAX_TILES];  //Texture array for tile textures
extern GLuint backgrounds[3];
extern GLuint buttontextures[14];

extern GLuint background;      //Displaylist variable for background and HUD
extern GLuint tile;            //Displaylist variable for states

GLuint Texture_GenerateGLTexture(void *bits, int type, GLuint width, GLuint height);
GLuint Texture_LoadImage(std::string, int type, int flag);
SDL_Surface* Texture_getSDLSurfaceFromImage(const char *file, int flag);
void Texture_GenerateTilemap(void);
void Texture_LoadTextures(void);
void Texture_HandleFreeImageError(FREE_IMAGE_FORMAT fif, const char *message);
int Texture_Cleanup();

#endif /* SDL_TEXTURES_H */
