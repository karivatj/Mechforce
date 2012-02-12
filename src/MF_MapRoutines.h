/*
 * Mechforce.h 1.00 by Kari Vatjus-Anttila
 *
 */

#ifndef MAP_H
#define MAP_H

#define SEED 0                          //Initial seed value of the corners of the map
#define MAP_SIZE 33                       //Size of the map
#define POINT(x,y) (MAP_SIZE*(x)+(y))   //Macro for mimicing the behaviour of a 2-dimensional array in 1-dimensional array.

typedef struct{
    double height;  /*Height of the point*/
    int type;       /*Type eg. forest, mountain, water, plain. Maybe used when drawing the map in 2D*/
}MAP;

typedef struct {
    float x,y,z;
}Vertexarray;

int numberoftriangles;

MAP Map[MAP_SIZE * MAP_SIZE];
Vertexarray *MAP_HD;

void MAP_SmoothTerrain(float k, int passes);            /*Functions which smoothes the terrain by removing sharp edges*/
void MAP_LoadMapFromFile(char keyword[]);   /*Loads a map into an array. used in 2D mode. Not sure if working*/
void MAP_Draw3DTerrain(void);               /*Draws the 3D terrain*/
void MAP_Draw2DTerrain(void);               /*Draws a 2D tilebased terrain*/
void MAP_GenerateMap(void);                 /*Generates a 3D terrain*/
void MAP_SetTiles(void);
void MAP_NormalizeValues(void);
void MAP_CreateTrianglesFromMapData(Vertexarray *values);
int MAP_CalculateElementCountFromArray(int MapSize);
#endif /*MAP_H*/
