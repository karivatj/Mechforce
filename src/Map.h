#ifndef MAP_H
#define MAP_H

#define MAP_SIZE 9
#define SEED 1000

#define POINT(x,y) (MAP_SIZE*(y)+(x))

#include "SDL_Engine.h"

typedef struct {
        float height;
	int type;
}MAP;

typedef struct {
    float x,y,z;
}Vertexarray;

double Map[MAP_SIZE * MAP_SIZE];
Vertexarray Map_VertexArray[MAP_SIZE * MAP_SIZE];

void MAP_DrawFractalMap(void);
void MAP_DrawTileMap();
void MAP_GenerateMap();
void MAP_LoadMap(char keyword[]);
void MAP_SetTile();

#endif /*MAP_H*/
