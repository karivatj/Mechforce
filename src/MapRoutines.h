/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * MapRoutines.h 1.00 by Kari Vatjus-Anttila
 *
 */

#ifndef MAP_H
#define MAP_H

#define SEED 0                          //Initial seed value of the corners of the map
#define MAP_SIZE 129                       //Size of the map
#define POINT(x,y) (MAP_SIZE*(x)+(y))   //Macro for mimicing the behaviour of a 2-dimensional array in 1-dimensional array.

typedef struct{
    double h;  /*Height of the point*/
    int type;       /*Type eg. forest, mountain, water, plain. Maybe used when drawing the map in 2D*/
}MAP;

typedef struct {
    float x;
    float y;
    float z;
} Vector;

typedef struct {
    Vector v[3];
} Triangle;

typedef struct {
    Triangle t[6];
} Hexagon;

typedef struct {
    float x;
    float y;
    float z;
} Vertexarray;

typedef struct {
    float r;
    float g;
    float b;
} Colorarray;

int numberoftriangles;

MAP Map[MAP_SIZE * MAP_SIZE];

Vertexarray *MAP_HD;        //Vertexarray which holds the hexagon data
Vertexarray *MAP_Outlines;  //Array which holds the outline data for the hexagons
Colorarray  *MAP_Colors;    //Array which holds the color data for the hexagons
Vertexarray *MAP_Normals;   //Array which holds the normal data for the hexagons. Used with lighting

void Map_LoadMapFromFile(char keyword[]);       /*Loads a map into an array. used in 2D mode. Not sure if working*/
void Map_Draw3DTerrain(void);                   /*Draws the 3D terrain*/
void Map_Draw2DTerrain(void);                   /*Draws a 2D tilebased terrain*/
void Map_GenerateMap(void);                     /*Generates a 3D terrain*/
void Map_SmoothTerrain(float k, int passes);    /*Functions which smoothes the terrain by removing sharp edges*/
void Map_SetTiles(void);
void Map_NormalizeValues(void);

void Map_SetColor(Colorarray *c, int height, int index);
void Map_CreateOutline(Vertexarray *v, int index, float x, float y, float height);
void Map_CreateTriangle(Vertexarray *v, int index, float x1, float y1, float h1, float x2, float y2, float h2, float x3, float y3, float h3);
void Map_CreateNormal(Vertexarray *v, int index, float x1, float y1, float h1, float x2, float y2, float h2, float x3, float y3, float h3);
void Map_GenerateMapData(Vertexarray *v, Vertexarray *n, Vertexarray *o, Colorarray *c, MAP *h, int MapSize);
void Map_AllocateMemoryForMapData(void);

int Map_CalculateElementCountFromArray(int MapSize);
int Map_Cleanup();


#endif /*MAP_H*/
