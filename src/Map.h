#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define PAIKKA(mx1,my1) (MAP_WIDTH*(my1)+(mx1))

#include "SDL_Engine.h"

typedef struct {
	float korkeus;
	int type;
}MAP;

typedef struct {
    float x,y,z;
}Vertexarray;

MAP Map[MAP_WIDTH * MAP_HEIGHT]; //20x15 map
Vertexarray Map_VertexArray[MAP_WIDTH * MAP_HEIGHT];

void MAP_DrawFractalMap(void);
void MAP_DrawTileMap();                                                             //Piirret‰‰n muistissa oleva kartta
void MAP_GenerateMap();                                                             //Generoidaan kartta
void MAP_LoadMap(char keyword[]);                                                   //Ladataan kartta karttatiedostosta
void MAP_SetTile();

float randnum (float min, float max);                                               //Arvo numero parametreina annetulta v‰lilt‰
float avgDiamondVals (int i, int j, int stride, int size, int subSize, float *fa);  //Lasketaan keskiarvoja
float avgSquareVals (int i, int j, int stride, int size, float *fa);                //Lasketaan keskiarvoja
float* alloc2DFractArray (int size);                                                //Luodaan taulukko annetun koon mukaan
void dump2DFractArray (float *fa, int size);                                        //Tulostetaan taulukko
int FindMinMax(float *fa, int size);
void CopyArray(float *fa);


#endif /*MAP_H*/
