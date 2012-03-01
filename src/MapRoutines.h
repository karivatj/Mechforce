/*
 * Mechforce.h 1.00 by Kari Vatjus-Anttila
 *
 */

#ifndef MAP_H
#define MAP_H

#define SEED 0                          //Initial seed value of the corners of the map
#define MAP_SIZE 65                       //Size of the map
#define POINT(x,y) (MAP_SIZE*(x)+(y))   //Macro for mimicing the behaviour of a 2-dimensional array in 1-dimensional array.

typedef struct{
    double h;  /*Height of the point*/
    int type;       /*Type eg. forest, mountain, water, plain. Maybe used when drawing the map in 2D*/
}MAP;

typedef struct {
    float x,y,z;
}Vertexarray;

int numberoftriangles;

MAP Map[MAP_SIZE * MAP_SIZE];

Vertexarray *MAP_HD;        //Vertexarray which holds the hexagon data
Vertexarray *MAP_Outlines;  //Array which holds the outline data for the hexagons
Vertexarray *MAP_Colors;         //Array which holds the color data for the hexagons

void Map_SmoothTerrain(float k, int passes);            /*Functions which smoothes the terrain by removing sharp edges*/
void Map_LoadMapFromFile(char keyword[]);   /*Loads a map into an array. used in 2D mode. Not sure if working*/
void Map_Draw3DTerrain(void);               /*Draws the 3D terrain*/
void Map_Draw2DTerrain(void);               /*Draws a 2D tilebased terrain*/
void Map_GenerateMap(void);                 /*Generates a 3D terrain*/
void Map_SetTiles(void);
void Map_NormalizeValues(void);
void Map_CreateTrianglesFromMapData(Vertexarray *values, MAP *hdata, int MapSize);
void Map_CreateOutlinesFromMapData(Vertexarray *values, MAP *odata, int MapSize);
void Map_CreateColorData(Vertexarray *values, int MapSize);
int Map_CalculateElementCountFromArray(int MapSize);
int Map_Cleanup();
#endif /*MAP_H*/
