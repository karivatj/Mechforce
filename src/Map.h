#ifndef MAP_H
#define MAP_H

#define MAP_SIZE 65                     //Size of the map
#define SEED 5                          //Initial seed value of the corners of the map
#define POINT(x,y) (MAP_SIZE*(y)+(x))   //Macro for mimicing the behaviour of a 2-dimensional array in 1-dimensional array.

typedef struct{
    double height;
    int type;       /*Type eg. forest, mountain, water, plain. Maybe used when drawing the map in 2D*/
}MAP;

typedef struct {
    float x,y,z;
}Vertexarray;

MAP Map[MAP_SIZE * MAP_SIZE];
Vertexarray Map_VertexArray[MAP_SIZE * MAP_SIZE];

void MAP_SmoothTerrain(float k);    /*Functions which smoothes the terrain by removing sharp edges*/
void MAP_LoadMap(char keyword[]);   /*Loads a map into an array. used in 2D mode. Not sure if working*/
void MAP_Draw3DTerrain(void);       /*Draws the 3D terrain*/
void MAP_Draw2DTerrain(void);       /*Draws a 2D tilebased terrain*/
void MAP_GenerateMap(void);         /*Generates a 3D terrain*/

#endif /*MAP_H*/
