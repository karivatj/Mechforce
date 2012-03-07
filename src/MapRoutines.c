/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * MapRoutines.c 1.00 by Kari Vatjus-Anttila
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "Core/SDL_Engine.h"
#include "MapRoutines.h"

void Map_LoadMapFromFile(char keyword[])
{
    FILE *file;
    char string[128];
    char *token = NULL;
    int done = 0;
    int x = 0, y = 0;

    if((file = fopen ("../Data/maps.ini", "r")) == NULL)
    {
        fprintf(stderr,"ERROR*** Couldn't open Map file! (./Data/maps.ini)\n");
        SDL_Close(-1);
    }

    printf("Opening map datafile.\nStarting to read map data.\n");

    while(!feof(file))
    {
        fgets(string,64,file);

        if(strstr(string, keyword))
        {
            printf("Found %s section. Starting to read data.\n", keyword);
            while(!feof(file))
            {
                fgets(string,64,file);
                token = strtok(string," ");

                while(token != NULL)
                {
                    switch((*token))
                    {
                        case 'E':
                            done = 1;
                        break;

                        default:
                            Map[POINT(x,y)].type = atoi(token);
                            Map[POINT(x,y)].h = 0;
                            x++;
                        break;
                    }

                    if(done) break;

                    token = strtok (NULL, " ");
                }

                y++; x=0;

                if(done) break;
            }
        }
        if(done) break;
    }

    fclose (file);
}


void Map_Draw2DTerrain(void)
{
    int x,y;

    OrthogonalStart();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glDisable(GL_DEPTH_TEST);

   //SDL_DrawTile(1,10,500);

    //glTranslatef(0,0,-5);

    for(y = 0; y < MAP_SIZE; y++)
    {
        for(x = 0; x < MAP_SIZE; x++)
        {
            if(y % 2)
                SDL_DrawTile(Map[POINT(x,y)].type, 25 + (x * 32), 550 - (y * 16) - (y * 8));
            else
                SDL_DrawTile(Map[POINT(x,y)].type, 41 + (x * 32), 550 - (y * 16) - (y * 8));
        }
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    OrthogonalEnd();
}

void Map_Draw3DTerrain(void)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glEnableClientState(GL_VERTEX_ARRAY);   //We want a vertex array
    glEnableClientState(GL_COLOR_ARRAY);   //We want a vertex array
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &MAP_HD[0]);   //All v are grouped to three Floats, we start at the beginning of the array (offset=0) and want to use as VertexArray
    glColorPointer(3, GL_FLOAT, 0, &MAP_Colors[0]);
    glNormalPointer(GL_FLOAT, 0, &MAP_Normals[0]);

    glDrawArrays(GL_TRIANGLES, 0, numberoftriangles * 3); //We draw the first three vertices in the array as a triangle//////8////

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &MAP_Outlines[0]);  //Draw outlines

  //  glLineWidth(2.0);
    glColor3f(1,1,1);
    glDrawArrays(GL_LINES, 0, numberoftriangles * 2); //We draw the first three vertices in the array as a triangle
    glColor3f(1,1,1);
    //glLineWidth(1.0);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDisableClientState(GL_VERTEX_ARRAY);


    int i;

    for( i = 0; i < numberoftriangles * 3 / 4; i+=3)
    {
        Triangle t = {{{MAP_HD[i].x,MAP_HD[i].y,MAP_HD[i].z},{MAP_HD[i+1].x,MAP_HD[i+1].y,MAP_HD[i+1].z},{MAP_HD[i+2].x,MAP_HD[i+2].y,MAP_HD[i+2].z}}};
        Vector normal = {MAP_Normals[i].x, MAP_Normals[i].y, MAP_Normals[i].z};
        double x, y, z;

        x = (t.v[0].x + t.v[1].x + t.v[2].x) / 3;
        y = (t.v[0].y + t.v[1].y + t.v[2].y) / 3;
        z = (t.v[0].z + t.v[1].z + t.v[2].z) / 3;

        glLineWidth(2.0f);
        glColor3f(1,0,0);
        glBegin(GL_LINES);
        glVertex3f(x,y,z);
        glVertex3f(x + (normal.x*10),y+(normal.y*10),z+(normal.z*10));
        glEnd();
        glColor3f(0,0,0);
        glLineWidth(1.0);
    }

    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);

}

void Map_SmoothTerrain(float k, int passes)
{
    int i, x,y;

    for(i = 0; i < passes; i++)
    {
        /* Rows, left to right */
        for(x = 1;x < MAP_SIZE; x++)
            for (y = 0;y < MAP_SIZE; y++)
                Map[POINT(x,y)].h = Map[POINT(x-1,y)].h * (1-k) + Map[POINT(x,y)].h * k;

        /* Rows, right to left*/
        for(x = MAP_SIZE-2;x < -1; x--)
            for (y = 0;y < MAP_SIZE; y++)
                Map[POINT(x,y)].h = Map[POINT(x+1,y)].h * (1-k) + Map[POINT(x,y)].h * k;

        /* Columns, bottom to top */
        for(x = 0;x < MAP_SIZE; x++)
            for (y = 1;y < MAP_SIZE; y++)
               Map[POINT(x,y)].h = Map[POINT(x,y-1)].h * (1-k) + Map[POINT(x,y)].h * k;

        /* Columns, top to bottom */
        for(x = 0;x < MAP_SIZE; x++)
            for (y = MAP_SIZE; y < -1; y--)
                Map[POINT(x,y)].h = Map[POINT(x,y+1)].h * (1-k) + Map[POINT(x,y)].h * k;
    }
}

//Generate the map using Diamond-Square Algorithm
void Map_GenerateMap()
{
    int sideLength;

    int x;
    int y;
    double h = 100;

    numberoftriangles = 0;

    //Seed the data
    Map[POINT(0,0)].h = Map[POINT(0,MAP_SIZE-1)].h = Map[POINT(MAP_SIZE-1,0)].h = Map[POINT(MAP_SIZE-1,MAP_SIZE-1)].h = SEED;

    for(sideLength = MAP_SIZE-1; sideLength >= 2; sideLength /=2, h/= 2.0)
    {
        int halfSide = sideLength/2;

        for(x=0;x<MAP_SIZE-1;x+=sideLength)
        {
            for(y=0;y<MAP_SIZE-1;y+=sideLength)
            {
                double avg =    Map[POINT(x,y)].h +                       //top left
                                Map[POINT(x+sideLength,y)].h +            //top right
                                Map[POINT(x,y+sideLength)].h +            //lower left
                                Map[POINT(x+sideLength,y+sideLength)].h;  //lower right
                avg /= 4.0;

                //center is average plus random offset
                Map[POINT(x+halfSide,y+halfSide)].h =

                avg + ((double)rand()/(double)RAND_MAX * 2 * h) - h;
            }
        }

        for(x=0;x<MAP_SIZE-1;x+=halfSide)
        {
            for(y=(x+halfSide)%sideLength;y<MAP_SIZE-1;y+=sideLength)
            {
                double  avg =   Map[POINT((x - halfSide  + MAP_SIZE) % (MAP_SIZE), y)].h +         //left
                                Map[POINT((x + halfSide) % MAP_SIZE - 1, y)].h +                   //right
                                Map[POINT(x, (y + halfSide) % MAP_SIZE - 1)].h +                   //below
                                Map[POINT(x, (y - halfSide  + MAP_SIZE - 1) % (MAP_SIZE - 1))].h;  //above center

                avg /= 4.0;

                avg += ((double)rand() / (double)RAND_MAX * 2 * h) - h;

                Map[POINT(x,y)].h = avg;

                if(x == 0)  Map[POINT(MAP_SIZE-1,y)].h = avg;
                if(y == 0)  Map[POINT(x,MAP_SIZE-1)].h = avg;
            }
        }
    }

    Map_SmoothTerrain(0.75, 3);
    Map_NormalizeValues();
    Map_SetTiles();
    Map_GenerateMapData();
}

void Map_SetTiles(void)
{
    int x, y;

    for(x = 0; x < MAP_SIZE; x++)
    {
        for(y = 0; y < MAP_SIZE; y++)
        {
           if(Map[POINT(x,y)].h > 90) Map[POINT(x,y)].type = 15;
           else if(Map[POINT(x,y)].h > 70) Map[POINT(x,y)].type = 14;
           else if(Map[POINT(x,y)].h > 40) Map[POINT(x,y)].type = 13;
           else if(Map[POINT(x,y)].h > 10) Map[POINT(x,y)].type = 12;
           else if(Map[POINT(x,y)].h >  0) Map[POINT(x,y)].type = 0;
           else if(Map[POINT(x,y)].h > -10) Map[POINT(x,y)].type = 8;
           else if(Map[POINT(x,y)].h > -30) Map[POINT(x,y)].type = 9;
           else if(Map[POINT(x,y)].h > -70) Map[POINT(x,y)].type = 10;
           else if(Map[POINT(x,y)].h < -70) Map[POINT(x,y)].type = 11;
        }
    }
}

void Map_NormalizeValues(void)
{
    int x, y;
    for(x = 0; x < MAP_SIZE; x++)
    {
        for(y = 0; y < MAP_SIZE; y++)
        {
            //if(Map[POINT(x,y)].h < 0)
                Map[POINT(x,y)].h = 0;
        }
    }
}

void Map_GenerateMapData(void)
{
    int numofelem =  Map_CalculateElementCountFromArray(MAP_SIZE);
    int size = numofelem * 6 * 3;

    if((MAP_HD = calloc(size, sizeof(Vertexarray))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %u bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    if((MAP_Colors = calloc(size*2, sizeof(Colorarray))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %u bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    if((MAP_Normals = calloc(size, sizeof(Vertexarray))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %u bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    size = numofelem * 6 * 2;

    if((MAP_Outlines = calloc(size, sizeof(Vertexarray))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %u bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    size = Map_CreateData(MAP_HD, MAP_Normals, MAP_Outlines, MAP_Colors, Map, MAP_SIZE);
}

int Map_CalculateElementCountFromArray(int MapSize)
{
    int numberofelements = 0;
    int x, y;

    for (x = 0; x < MapSize; x+=3)
    {
        for (y = 0; y < MapSize; y+=2)
        {
            if(x <= MapSize - 3)
            {
                if(y == MapSize -1) break;

                numberofelements += 1;
            }
            if(x != 0)
            {
                if(y == MapSize - 3) break; /*If we are at the second last cell of the row, then stop*/

                numberofelements += 1;
            }
        }
    }

    return numberofelements;
}

void Map_CreateOutline(Vertexarray *v, int index, float x, float y, float height)
{
    x = x * 5;
    y = y * 5;

    v[index].x = x;
    v[index].y = height;
    v[index].z = y;
}

void Map_CreateTriangle(Vertexarray *v, int index, float x1, float y1, float h1, float x2, float y2, float h2, float x3, float y3, float h3)
{
    x1 *= 5;    y1 *= 5;
    x2 *= 5;    y2 *= 5;
    x3 *= 5;    y3 *= 5;

    v[index].x = x1; v[index+1].x = x2; v[index+2].x = x3;
    v[index].y = h1; v[index+1].y = h2; v[index+2].y = h3;
    v[index].z = y1; v[index+1].z = y2; v[index+2].z = y3;
}

void Map_SetColor(Colorarray *c, int height, int index)
{
    float r,g,b;

    r = g = b = 0;

    if(height == 0)
    {
        r = 0; g = 0; b = 1;
    }
    else if(height < 3)
    {
        r = 1; g = 0.81; b = 0.48;
    }
    else
    {
        r = 0; g = 1; b = 0;
    }

    c[index].r = r;
    c[index].g = g;
    c[index].b = b;
}

void Map_CreateNormal(Vertexarray *v, int index, float x1, float y1, float h1, float x2, float y2, float h2, float x3, float y3, float h3)
{
    Vector normal;
    Vector U;
    Vector V;
    double magnitude;

    x1 *= 5;    y1 *= 5;
    x2 *= 5;    y2 *= 5;
    x3 *= 5;    y3 *= 5;

    U.x = x2 - x1;
    U.y = h2 - h1;
    U.z = y2 - y1;

    V.x = x3 - x1;
    V.y = h3 - h1;
    V.z = y3 - y1;

    normal.x = (U.y * V.z) - (U.z * V.y);
    normal.y = (U.z * V.x) - (U.x * V.z);
    normal.z = (U.x * V.y) - (U.y * V.x);

    magnitude = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

    normal.x = normal.x/magnitude;
    normal.y = normal.y/magnitude;
    normal.z = normal.z/magnitude;

    v[index].x = normal.x;
    v[index].y = normal.y;
    v[index].z = normal.z;
}

int Map_CreateData(Vertexarray *v, Vertexarray *n, Vertexarray *o, Colorarray *c, MAP *h, int MapSize)
{
    int x, y, i;
    int triangleindex = 0;
    int colorindex = 0;
    int outlineindex = 0;
    int normalindex = 0;

    for (x = 0; x < MapSize; x+=3)
    {
        for (y = 0; y < MapSize; y+=2)
        {
            if(x <= MapSize - 3)
            {
                if(y == MapSize -1) break;

                /*Fill up the Color data*/
                for(i = 0; i < 18; i++)
                {
                    Map_SetColor(c, h[POINT(x,y)].h, colorindex);
                    colorindex += 1;
                }

                /*Create the triangles fro the hexagon*/
                Map_CreateTriangle(v, triangleindex,    x,     y,   h[POINT(x,y)].h,     x-0.5, y+1, h[POINT(x,y+1)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateTriangle(v, triangleindex+3,  x,     y,   h[POINT(x,y)].h,     x+1,   y,   h[POINT(x+1,y)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateTriangle(v, triangleindex+6,  x-0.5, y+1, h[POINT(x,y+1)].h,   x,     y+2, h[POINT(x,y+2)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateTriangle(v, triangleindex+9,  x,     y+2, h[POINT(x,y+2)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1,   y+2, h[POINT(x+1,y+2)].h);
                Map_CreateTriangle(v, triangleindex+12, x+1,   y,   h[POINT(x+1,y)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1.5, y+1, h[POINT(x+2,y+1)].h);
                Map_CreateTriangle(v, triangleindex+15, x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1,   y+2, h[POINT(x+1,y+2)].h, x+1.5, y+1, h[POINT(x+2,y+1)].h);

                Map_CreateNormal(n, normalindex,    x,     y,   h[POINT(x,y)].h,     x-0.5, y+1, h[POINT(x,y+1)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateNormal(n, normalindex+1,  x,     y,   h[POINT(x,y)].h,     x+1,   y,   h[POINT(x+1,y)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateNormal(n, normalindex+2,  x-0.5, y+1, h[POINT(x,y+1)].h,   x,     y+2, h[POINT(x,y+2)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateNormal(n, normalindex+3,  x,     y+2, h[POINT(x,y+2)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1,   y+2, h[POINT(x+1,y+2)].h);
                Map_CreateNormal(n, normalindex+4, x+1,   y,   h[POINT(x+1,y)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1.5, y+1, h[POINT(x+2,y+1)].h);
                Map_CreateNormal(n, normalindex+5, x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1,   y+2, h[POINT(x+1,y+2)].h, x+1.5, y+1, h[POINT(x+2,y+1)].h);

#if 0
                /*Create normals*/
                Map_CreateNormal(n, normalindex,   x,     y,   h[POINT(x,y)].h,     x-0.5, y+1, h[POINT(x,y+1)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateNormal(n, normalindex+1, x,     y,   h[POINT(x,y)].h,     x+1,   y,   h[POINT(x+1,y)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateNormal(n, normalindex+2, x-0.5, y+1, h[POINT(x,y+1)].h,   x,     y+2, h[POINT(x,y+2)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateNormal(n, normalindex+3, x,     y+2, h[POINT(x,y+2)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1,   y+2, h[POINT(x+1,y+2)].h);
                Map_CreateNormal(n, normalindex+4, x+1,   y,   h[POINT(x+1,y)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1.5, y+1, h[POINT(x+2,y+1)].h);
                Map_CreateNormal(n, normalindex+5, x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1,   y+2, h[POINT(x+1,y+2)].h, x+1.5, y+1, h[POINT(x+2,y+1)].h);
#endif
                /*Create outlines for the hexagon*/
                Map_CreateOutline(o, outlineindex,    x,     y,   h[POINT(x, y)].h);
                Map_CreateOutline(o, outlineindex+1,  x-0.5, y+1, h[POINT(x, y+1)].h);
                Map_CreateOutline(o, outlineindex+2,  x-0.5, y+1, h[POINT(x, y+1)].h);
                Map_CreateOutline(o, outlineindex+3,  x,     y+2, h[POINT(x, y+2)].h);
                Map_CreateOutline(o, outlineindex+4,  x,     y+2, h[POINT(x, y+2)].h);
                Map_CreateOutline(o, outlineindex+5,  x+1,   y+2, h[POINT(x+1, y+2)].h);
                Map_CreateOutline(o, outlineindex+6,  x+1,   y+2, h[POINT(x+1, y+2)].h);
                Map_CreateOutline(o, outlineindex+7,  x+1.5, y+1, h[POINT(x+2, y+1)].h);
                Map_CreateOutline(o, outlineindex+8,  x+1.5, y+1, h[POINT(x+2, y+1)].h);
                Map_CreateOutline(o, outlineindex+9,  x+1,   y,   h[POINT(x+1, y)].h);
                Map_CreateOutline(o, outlineindex+10, x+1,   y,   h[POINT(x+1, y)].h);
                Map_CreateOutline(o, outlineindex+11, x,     y,   h[POINT(x, y)].h);

                outlineindex += 12;
                triangleindex += 18;
                normalindex += 6;
                numberoftriangles += 6;
            }
            if(x != 0)
            {
                if(y == MapSize - 3) break; /*If we are at the second last cell of the row, then stop*/

                for(i = 0; i < 18; i++)
                {
                    Map_SetColor(c, h[POINT(x,y)].h, colorindex);
                    colorindex += 1;
                }

                Map_CreateTriangle(v, triangleindex,    x-1.5, y+1, h[POINT(x-1,y+1)].h, x-2,   y+2, h[POINT(x-2,y+2)].h, x-1,   y+2, h[POINT(x-1,y+2)].h);
                Map_CreateTriangle(v, triangleindex+3,  x-2,   y+2, h[POINT(x-2,y+2)].h, x-1.5, y+3, h[POINT(x-1,y+3)].h, x-1,   y+2, h[POINT(x-1,y+2)].h);
                Map_CreateTriangle(v, triangleindex+6,  x-1.5, y+1, h[POINT(x-1,y+1)].h, x-1,   y+2, h[POINT(x-1,y+2)].h, x-0.5, y+1, h[POINT(x,y+1)].h);
                Map_CreateTriangle(v, triangleindex+9,  x-1,   y+2, h[POINT(x-1,y+2)].h, x-0.5, y+1, h[POINT(x,y+1)].h,   x,     y+2, h[POINT(x,y+2)].h);
                Map_CreateTriangle(v, triangleindex+12, x-1,   y+2, h[POINT(x-1,y+2)].h, x-1.5, y+3, h[POINT(x-1,y+3)].h, x-0.5, y+3, h[POINT(x,y+3)].h);
                Map_CreateTriangle(v, triangleindex+15, x-1,   y+2, h[POINT(x-1,y+2)].h, x,     y+2, h[POINT(x,y+2)].h,   x-0.5, y+3, h[POINT(x,y+3)].h);

                Map_CreateNormal(n, normalindex,    x-1.5, y+1, h[POINT(x-1,y+1)].h, x-2,   y+2, h[POINT(x-2,y+2)].h, x-1,   y+2, h[POINT(x-1,y+2)].h);
                Map_CreateNormal(n, normalindex+1,  x-2,   y+2, h[POINT(x-2,y+2)].h, x-1.5, y+3, h[POINT(x-1,y+3)].h, x-1,   y+2, h[POINT(x-1,y+2)].h);
                Map_CreateNormal(n, normalindex+2,  x-1.5, y+1, h[POINT(x-1,y+1)].h, x-1,   y+2, h[POINT(x-1,y+2)].h, x-0.5, y+1, h[POINT(x,y+1)].h);
                Map_CreateNormal(n, normalindex+3,  x-1,   y+2, h[POINT(x-1,y+2)].h, x-0.5, y+1, h[POINT(x,y+1)].h,   x,     y+2, h[POINT(x,y+2)].h);
                Map_CreateNormal(n, normalindex+4, x-1,   y+2, h[POINT(x-1,y+2)].h, x-1.5, y+3, h[POINT(x-1,y+3)].h, x-0.5, y+3, h[POINT(x,y+3)].h);
                Map_CreateNormal(n, normalindex+5, x-1,   y+2, h[POINT(x-1,y+2)].h, x,     y+2, h[POINT(x,y+2)].h,   x-0.5, y+3, h[POINT(x,y+3)].h);
             /*
                Map_CreateNormal(n, normalindex,   x,     y,   h[POINT(x,y)].h,     x-0.5, y+1, h[POINT(x,y+1)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateNormal(n, normalindex+1, x,     y,   h[POINT(x,y)].h,     x+1,   y,   h[POINT(x+1,y)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateNormal(n, normalindex+2, x-0.5, y+1, h[POINT(x,y+1)].h,   x,     y+2, h[POINT(x,y+2)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h);
                Map_CreateNormal(n, normalindex+3, x,     y+2, h[POINT(x,y+2)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1,   y+2, h[POINT(x+1,y+2)].h);
                Map_CreateNormal(n, normalindex+4, x+1,   y,   h[POINT(x+1,y)].h,   x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1.5, y+1, h[POINT(x+2,y+1)].h);
                Map_CreateNormal(n, normalindex+5, x+0.5, y+1, h[POINT(x+1,y+1)].h, x+1,   y+2, h[POINT(x+1,y+2)].h, x+1.5, y+1, h[POINT(x+2,y+1)].h);
               */
                Map_CreateOutline(o, outlineindex,    x-0.5, y+1, h[POINT(x, y+1)].h);
                Map_CreateOutline(o, outlineindex+1,  x-1.5, y+1, h[POINT(x-1 , y+1)].h);
                Map_CreateOutline(o, outlineindex+2,  x-1.5, y+1, h[POINT(x-1, y+1)].h);
                Map_CreateOutline(o, outlineindex+3,  x-2,   y+2, h[POINT(x-2, y+2)].h);
                Map_CreateOutline(o, outlineindex+4,  x-2,   y+2, h[POINT(x-2, y+2)].h);
                Map_CreateOutline(o, outlineindex+5,  x-1.5, y+3, h[POINT(x-1, y+3)].h);
                Map_CreateOutline(o, outlineindex+6,  x-1.5, y+3, h[POINT(x-1, y+3)].h);
                Map_CreateOutline(o, outlineindex+7,  x-0.5, y+3, h[POINT(x, y+3)].h);
                Map_CreateOutline(o, outlineindex+8,  x-0.5, y+3, h[POINT(x, y+3)].h);
                Map_CreateOutline(o, outlineindex+9,  x,     y+2, h[POINT(x, y+2)].h);
                Map_CreateOutline(o, outlineindex+10, x,     y+2, h[POINT(x, y+2)].h);
                Map_CreateOutline(o, outlineindex+11, x-0.5, y+1, h[POINT(x, y+1)].h);

                outlineindex += 12;
                triangleindex += 18;
                normalindex += 6;
                numberoftriangles += 6;
            }
        }
    }
    return triangleindex;
}

int Map_Cleanup()
{
    printf("Destroying Map Data...\n");

    free(MAP_HD);
    free(MAP_Outlines);
    free(MAP_Colors);
    free(MAP_Normals);

    return 0;
}
