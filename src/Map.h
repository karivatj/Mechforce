#ifndef MAP_H
#define MAP_H

#define MAP_SIZE 65
#define SEED 5

#define POINT(x,y) (MAP_SIZE*(y)+(x)) //Macro for mimicing the behaviour of a 2-dimensional array in 1-dimensional array.

#include "SDL_Engine.h"

typedef struct {
        float height;
	int type;
}MAP;

typedef struct {
    float x,y,z;
}Vertexarray;

MAP Map[MAP_SIZE * MAP_SIZE];
Vertexarray Map_VertexArray[MAP_SIZE * MAP_SIZE];

void MAP_SmoothTerrain(float k);
void MAP_LoadMap(char keyword[]);
void MAP_Draw3DTerrain(void);
void MAP_Draw2DTerrain(void);
void MAP_GenerateMap(void);


#endif /*MAP_H*/
