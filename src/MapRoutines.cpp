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
#include "MapRoutines.h"

MAP Map[MAP_SIZE * MAP_SIZE];
Vertexarray *MAP_HD;        //Vertexarray which holds the hexagon data
Vertexarray *MAP_Outlines;  //Array which holds the outline data for the hexagons
Vertexarray *MAP_Normals;   //Array which holds the normal data for the hexagons. Used with lighting
Colorarray  *MAP_Colors;    //Array which holds the color data for the hexagons
int numberoftriangles = 0;

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

  //glLineWidth(2.0);
    glColor3f(1,1,1);
    glDrawArrays(GL_LINES, 0, numberoftriangles * 2); //We draw the first three vertices in the array as a triangle
    glColor3f(1,1,1);
    //glLineWidth(1.0);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDisableClientState(GL_VERTEX_ARRAY);

#if 0
    int i;

    //printf("Triangles in Scene: %d\n", numberoftriangles);
    for( i = 0; i < numberoftriangles * 3; i+=3)
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
        glColor3f(1,1,1);
        glLineWidth(1.0);

    }
#endif

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
    printf("Generating Map\n");
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
    Map_GenerateMapData(MAP_HD, MAP_Normals, MAP_Outlines, MAP_Colors, Map, MAP_SIZE);
    printf("Map Generation Process Done\n");
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
            if(Map[POINT(x,y)].h < 0)
                Map[POINT(x,y)].h = 0;
        }
    }
}

void Map_GenerateMapData(Vertexarray *v, Vertexarray *n, Vertexarray *o, Colorarray *c, MAP *h, int MapSize)
{
    int x, y, i;
    int triangleindex = 0;
    int colorindex = 0;
    int outlineindex = 0;

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

                /*Create the triangles to form the hexagon*/
                Map_CreateTriangle(v,n, triangleindex,   Vector(x,     y,   h[POINT(x,y)].h),
                                                         Vector(x-0.5, y+1, h[POINT(x,y+1)].h),
                                                         Vector(x+0.5, y+1, h[POINT(x,y+1)].h));
                Map_CreateTriangle(v,n, triangleindex+3, Vector(x-0.5, y+1, h[POINT(x,y+1)].h),
                                                         Vector(x,     y+2, h[POINT(x,y+2)].h),
                                                         Vector(x+0.5, y+1, h[POINT(x,y+1)].h));
                Map_CreateTriangle(v,n, triangleindex+6, Vector(x,     y+2, h[POINT(x,y+2)].h),
                                                         Vector(x+1  , y+2, h[POINT(x+1,y+2)].h),
                                                         Vector(x+0.5, y+1, h[POINT(x,y+1)].h));
                Map_CreateTriangle(v,n, triangleindex+9, Vector(x+1,   y+2, h[POINT(x+1,y+2)].h),
                                                         Vector(x+1.5, y+1, h[POINT(x+2,y+1)].h),
                                                         Vector(x+0.5, y+1, h[POINT(x,y+1)].h));
                Map_CreateTriangle(v,n, triangleindex+12,Vector(x+1.5, y+1, h[POINT(x+2,y+1)].h),
                                                         Vector(x+1,   y  , h[POINT(x+1,y)].h),
                                                         Vector(x+0.5, y+1, h[POINT(x,y+1)].h));
                Map_CreateTriangle(v,n, triangleindex+15,Vector(x+1,   y,   h[POINT(x+1,y)].h),
                                                         Vector(x,     y,   h[POINT(x,y)].h),
                                                         Vector(x+0.5, y+1, h[POINT(x,y+1)].h));

                /*Create outlines for the hexagon*/
                Map_CreateOutline(o, outlineindex,    Vector(x,     y,   h[POINT(x, y)].h));
                Map_CreateOutline(o, outlineindex+1,  Vector(x-0.5, y+1, h[POINT(x, y+1)].h));
                Map_CreateOutline(o, outlineindex+2,  Vector(x-0.5, y+1, h[POINT(x, y+1)].h));
                Map_CreateOutline(o, outlineindex+3,  Vector(x,     y+2, h[POINT(x, y+2)].h));
                Map_CreateOutline(o, outlineindex+4,  Vector(x,     y+2, h[POINT(x, y+2)].h));
                Map_CreateOutline(o, outlineindex+5,  Vector(x+1,   y+2, h[POINT(x+1, y+2)].h));
                Map_CreateOutline(o, outlineindex+6,  Vector(x+1,   y+2, h[POINT(x+1, y+2)].h));
                Map_CreateOutline(o, outlineindex+7,  Vector(x+1.5, y+1, h[POINT(x+2, y+1)].h));
                Map_CreateOutline(o, outlineindex+8,  Vector(x+1.5, y+1, h[POINT(x+2, y+1)].h));
                Map_CreateOutline(o, outlineindex+9,  Vector(x+1,   y,   h[POINT(x+1, y)].h));
                Map_CreateOutline(o, outlineindex+10, Vector(x+1,   y,   h[POINT(x+1, y)].h));
                Map_CreateOutline(o, outlineindex+11, Vector(x,     y,   h[POINT(x, y)].h));

                outlineindex += 12;
                triangleindex += 18;
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

                Map_CreateTriangle(v, n, triangleindex,   Vector(x-1.5, y+1, h[POINT(x-1,y+1)].h),
                                                          Vector(x-2,   y+2, h[POINT(x-2,y+2)].h),
                                                          Vector(x-1,   y+2, h[POINT(x-1,y+2)].h));
                Map_CreateTriangle(v, n, triangleindex+3, Vector(x-2,   y+2, h[POINT(x-2,y+2)].h),
                                                          Vector(x-1.5, y+3, h[POINT(x-1,y+3)].h),
                                                          Vector(x-1,   y+2, h[POINT(x-1,y+2)].h));
                Map_CreateTriangle(v, n, triangleindex+6, Vector(x-1.5, y+3, h[POINT(x-1,y+3)].h),
                                                          Vector(x-0.5, y+3, h[POINT(x,y+3)].h),
                                                          Vector(x-1,   y+2, h[POINT(x-1,y+2)].h));
                Map_CreateTriangle(v, n, triangleindex+9, Vector(x-0.5, y+3, h[POINT(x,y+3)].h),
                                                          Vector(x,     y+2, h[POINT(x,y+2)].h),
                                                          Vector(x-1,   y+2, h[POINT(x-1,y+2)].h));
                Map_CreateTriangle(v, n, triangleindex+12,Vector(x,     y+2, h[POINT(x,y+2)].h),
                                                          Vector(x-0.5, y+1, h[POINT(x,y+1)].h),
                                                          Vector(x-1,   y+2, h[POINT(x-1,y+2)].h));
                Map_CreateTriangle(v, n, triangleindex+15,Vector(x-0.5, y+1, h[POINT(x,y+1)].h),
                                                          Vector(x-1.5, y+1, h[POINT(x-1,y+1)].h),
                                                          Vector(x-1,   y+2, h[POINT(x-1,y+2)].h));

                Map_CreateOutline(o, outlineindex,    Vector(x-0.5, y+1, h[POINT(x, y+1)].h));
                Map_CreateOutline(o, outlineindex+1,  Vector(x-1.5, y+1, h[POINT(x-1 , y+1)].h));
                Map_CreateOutline(o, outlineindex+2,  Vector(x-1.5, y+1, h[POINT(x-1, y+1)].h));
                Map_CreateOutline(o, outlineindex+3,  Vector(x-2,   y+2, h[POINT(x-2, y+2)].h));
                Map_CreateOutline(o, outlineindex+4,  Vector(x-2,   y+2, h[POINT(x-2, y+2)].h));
                Map_CreateOutline(o, outlineindex+5,  Vector(x-1.5, y+3, h[POINT(x-1, y+3)].h));
                Map_CreateOutline(o, outlineindex+6,  Vector( x-1.5, y+3, h[POINT(x-1, y+3)].h));
                Map_CreateOutline(o, outlineindex+7,  Vector(x-0.5, y+3, h[POINT(x, y+3)].h));
                Map_CreateOutline(o, outlineindex+8,  Vector(x-0.5, y+3, h[POINT(x, y+3)].h));
                Map_CreateOutline(o, outlineindex+9,  Vector(x,     y+2, h[POINT(x, y+2)].h));
                Map_CreateOutline(o, outlineindex+10, Vector(x,     y+2, h[POINT(x, y+2)].h));
                Map_CreateOutline(o, outlineindex+11, Vector(x-0.5, y+1, h[POINT(x, y+1)].h));

                outlineindex += 12;
                triangleindex += 18;
                numberoftriangles += 6;
            }
        }
    }
}

void Map_AllocateMemoryForMapData(void)
{
    int numofelem =  Map_CalculateElementCountFromArray(MAP_SIZE);
    int size = numofelem * 6 * 3;

    if((MAP_HD = static_cast<Vertexarray*>(calloc(size, sizeof(Vertexarray)))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %u bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    if((MAP_Colors = static_cast<Colorarray*>(calloc(size*2, sizeof(Colorarray)))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %u bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    if((MAP_Normals = static_cast<Vertexarray*>(calloc(size, sizeof(Vertexarray)))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %u bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    size = numofelem * 6 * 2;

    if((MAP_Outlines = static_cast<Vertexarray*>(calloc(size, sizeof(Vertexarray)))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %u bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }
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

void Map_SetColor(Colorarray *c, int height, int index)
{
    Vector color;

    if(height == 0) //Color for water
    {
        color.x = 0;
        color.y = 0;
        color.z = 1;
    }
    else if(height < 3)
    {
        color.x = 1;
        color.y = 0.81;
        color.z = 0.48;
    }
    else    //Color for grass
    {
        color.x = 0;
        color.y = 1;
        color.z = 0;
    }

    c[index].c = color;
}

void Map_CreateOutline(Vertexarray *outlines, int index, Vector vector)
{
    vector.x *= 5;
    vector.y *= 5;

    outlines[index].v = vector;

}

void Map_CreateTriangle(Vertexarray *vertices, Vertexarray *normals, int index, Vector a, Vector b, Vector c)
{
    a = a * 5;
    b = b * 5;
    c = c * 5;

    Map_CreateNormal(normals, index/3, a, b, c);

    vertices[index].v   = a;
    vertices[index+1].v = b;
    vertices[index+2].v = c;
}

void Map_CreateNormal(Vertexarray *normals, int index, Vector a, Vector b, Vector c)
{
    Vector normal;
    Vector P;
    Vector Q;
    float magnitude;

    P.x = b.x - a.x;
    P.y = b.y - a.y;
    P.z = b.z - a.z;

    Q.x = c.x - a.x;
    Q.y = c.y - a.y;
    Q.z = c.z - a.z;

    normal.x = (P.y * Q.z) - (P.z * Q.y);
    normal.y = (P.z * Q.x) - (P.x * Q.z);
    normal.z = (P.x * Q.y) - (P.y * Q.x);

    magnitude = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

    normal = normal / magnitude;

    normals[index].v = normal;
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
