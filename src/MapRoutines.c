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

    glLineWidth(2.0);
    glColor3f(1,1,1);
    glDrawArrays(GL_LINES, 0, numberoftriangles * 2); //We draw the first three vertices in the array as a triangle
    glColor3f(1,1,1);
    glLineWidth(1.0);

#if 0
    for(i = 0; i < MAP_SIZE; i++)
    {
        for(j = 0; j < MAP_SIZE; j++)
        {
            temp[POINT(i,j)].x = MAP_HD[POINT(j,i)].x;
            temp[POINT(i,j)].y = MAP_HD[POINT(j,i)].y;
            temp[POINT(i,j)].z = MAP_HD[POINT(j,i)].z;
/*
            temp[i * MAP_SIZE + j].x = MAP_HD[j * MAP_SIZE + i].x;
            temp[i * MAP_SIZE + j].y = MAP_HD[j * MAP_SIZE + i].y;
            temp[i * MAP_SIZE + j].z = MAP_HD[j * MAP_SIZE + i].z;*/
        }
    }

    glVertexPointer(3, GL_FLOAT, 0, &temp[0]);   //All v are grouped to three Floats, we start at the beginning of the array (offset=0) and want to use as VertexArray

    for(i = 0; i<MAP_SIZE; i++)
    {
        glDrawArrays(GL_LINE_STRIP, MAP_SIZE * i, MAP_SIZE);
    }
#endif

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDisableClientState(GL_VERTEX_ARRAY);
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

    //Generate3DMapData();
/*****************************************************/
/* Tästä alas päin omaan moduuliin, not related code */
/*****************************************************/

    int numofelem =  Map_CalculateElementCountFromArray(MAP_SIZE);

    int size = numofelem * 6 * 3;

    if((MAP_HD = calloc(size, sizeof(Vertexarray))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %ud bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    if((MAP_Colors = calloc(size*2, sizeof(Vertexarray))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %ud bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    if((MAP_Normals = calloc(size, sizeof(Vertexarray))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %ud bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    size = numofelem * 6 * 2;

    if((MAP_Outlines = calloc(size, sizeof(Vertexarray))) == NULL) //Allocate memory for the map data
    {
        printf("Not enough memory for map data! Tried to allocate %ud bytes but failed!\n", size * sizeof(Vertexarray));
        SDL_Close(-1);
    }

    size = Map_CreateTrianglesFromMapData(MAP_HD, Map, MAP_SIZE);
    size = Map_CreateColorData(MAP_Colors, Map, MAP_SIZE);
    size = Map_CreateOutlinesFromMapData(MAP_Outlines, Map, MAP_SIZE);
    size = Map_CreateNormalData(MAP_Normals, Map, MAP_SIZE);

#if 1
    static double min = 100000, max = -1;

    for(x = 0; x < MAP_SIZE; x++)
    {
        for(y = 0; y < MAP_SIZE; y++)
        {
           if(Map[POINT(x,y)].h < min) min = Map[POINT(x,y)].h;
           if(Map[POINT(x,y)].h > max) max = Map[POINT(x,y)].h;
        }
    }

    printf("Min %f Max %f\n", min, max);

    //Print out the Map
    for(x = 0; x < MAP_SIZE; x++)
    {
        for(y = 0; y < MAP_SIZE; y++)
        {
            printf("[%.3f] ",Map[POINT(x,y)].h);
        }
        printf("\n");
    }
    puts("");
#endif

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
            //Map[POINT(x,y)].h = round(Map[POINT(x,y)].h);

            //Map[POINT(x,y)].h = ((int)(Map[POINT(x,y)].h / 3)) * 3; // Round up to nearest 3

            if(Map[POINT(x,y)].h < 0)
                Map[POINT(x,y)].h = 0;
        }
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

int Map_CreateOutlinesFromMapData(Vertexarray *v, MAP *odata, int MapSize)
{
    int numofvert = 0;
    int x, y;

    for (x = 0; x < MapSize; x+=3)
    {
        for (y = 0; y < MapSize; y+=2)
        {
            if(x <= MapSize - 3)
            {
                if(y == MapSize -1) break;

                v[numofvert].x = x * 5;                       v[numofvert +1].x = (x-0.5) * 5;
                v[numofvert].y = odata[POINT(x, y)].h;        v[numofvert +1].y = odata[POINT(x, y+1)].h;
                v[numofvert].z = y * 5;                       v[numofvert +1].z = (y+1) * 5;

                v[numofvert +2].x = (x-0.5) * 5;              v[numofvert +3].x = x * 5;
                v[numofvert +2].y = odata[POINT(x, y+1)].h;   v[numofvert +3].y = odata[POINT(x, y+2)].h;
                v[numofvert +2].z = (y+1) * 5;                v[numofvert +3].z = (y+2) * 5;

                v[numofvert +4].x = x * 5;                    v[numofvert +5].x = (x+1) * 5;
                v[numofvert +4].y = odata[POINT(x, y+2)].h;   v[numofvert +5].y = odata[POINT(x+1, y+2)].h;
                v[numofvert +4].z = (y+2) * 5;                v[numofvert +5].z = (y+2) * 5;

                v[numofvert +6].x = (x+1) * 5;                v[numofvert +7].x = (x+1.5) * 5;
                v[numofvert +6].y = odata[POINT(x+1, y+2)].h; v[numofvert +7].y = odata[POINT(x+2, y+1)].h;
                v[numofvert +6].z = (y+2) * 5;                v[numofvert +7].z = (y+1) * 5;

                v[numofvert +8].x = (x+1.5) * 5;              v[numofvert +9].x = (x+1) * 5;
                v[numofvert +8].y = odata[POINT(x+2, y+1)].h; v[numofvert +9].y = odata[POINT(x+1, y)].h;
                v[numofvert +8].z = (y+1) * 5;                v[numofvert +9].z = y * 5;

                v[numofvert +10].x = (x+1) * 5;               v[numofvert +11].x = x * 5;
                v[numofvert +10].y = odata[POINT(x+1, y)].h;  v[numofvert +11].y = odata[POINT(x, y)].h;
                v[numofvert +10].z = y * 5;                   v[numofvert +11].z = y * 5;

                numofvert += 12;
            }
            if(x != 0)
            {
                if(y == MapSize - 3) break; /*If we are at the second last cell of the row, then stop*/

                v[numofvert].x = (x-0.5) * 5;                 v[numofvert +1].x = (x-1.5) * 5;
                v[numofvert].y = odata[POINT(x, y+1)].h;      v[numofvert +1].y = odata[POINT(x-1, y+1)].h;
                v[numofvert].z = (y+1) * 5;                   v[numofvert +1].z = (y+1) * 5;

                v[numofvert +2].x = (x-1.5) * 5;              v[numofvert +3].x = (x-2) * 5;
                v[numofvert +2].y = odata[POINT(x-1, y+1)].h; v[numofvert +3].y = odata[POINT(x-2, y+2)].h;
                v[numofvert +2].z = (y+1) * 5;                v[numofvert +3].z = (y+2) * 5;

                v[numofvert +4].x = (x-2) * 5;                v[numofvert +5].x = (x-1.5) * 5;
                v[numofvert +4].y = odata[POINT(x-2, y+2)].h; v[numofvert +5].y = odata[POINT(x-1, y+3)].h;
                v[numofvert +4].z = (y+2) * 5;                v[numofvert +5].z = (y+3) * 5;

                v[numofvert +6].x = (x-1.5) * 5;              v[numofvert +7].x = (x-0.5) * 5;
                v[numofvert +6].y = odata[POINT(x-1, y+3)].h; v[numofvert +7].y = odata[POINT(x, y+3)].h;
                v[numofvert +6].z = (y+3) * 5;                v[numofvert +7].z = (y+3) * 5;

                v[numofvert +8].x = (x-0.5) * 5;              v[numofvert +9].x = (x) * 5;
                v[numofvert +8].y = odata[POINT(x, y+3)].h;   v[numofvert +9].y = odata[POINT(x, y+2)].h;
                v[numofvert +8].z = (y+3) * 5;                v[numofvert +9].z = (y+2) * 5;

                v[numofvert +10].x = (x) * 5;             v[numofvert +11].x = (x-0.5) * 5;
                v[numofvert +10].y = odata[POINT(x, y+2)].h;  v[numofvert +11].y = odata[POINT(x, y+1)].h;
                v[numofvert +10].z = (y+2) * 5;               v[numofvert +11].z = (y+1) * 5;

                numofvert += 12;
            }
        }
    }
    return numofvert;
}

int Map_CreateTrianglesFromMapData(Vertexarray *v, MAP *h, int MapSize)
{
    int x, y;
    int numofvert = 0;

    for (x = 0; x < MapSize; x+=3)
    {
        for (y = 0; y < MapSize; y+=2)
        {
            numofvert = numberoftriangles * 3;

            if(x <= MapSize - 3)
            {
                if(y == MapSize -1) break;
                /*Triangle type A*/

                /*Triangle #1 */
                v[numofvert].x = x * 5;                v[numofvert +1].x = (x-0.5) * 5;            v[numofvert +2].x = (x+0.5) * 5;
                v[numofvert].y = h[POINT(x, y)].h;      v[numofvert +1].y = h[POINT(x, y+1)].h;     v[numofvert +2].y = h[POINT(x+1, y+1)].h;
                v[numofvert].z = y * 5;                v[numofvert +1].z = (y+1) * 5;              v[numofvert +2].z = (y+1) * 5;

                /*Triangle #2*/
                v[numofvert +3].x = x * 5;                v[numofvert +4].x = (x+1) * 5;              v[numofvert +5].x = (x+0.5) * 5;
                v[numofvert +3].y = h[POINT(x, y)].h;     v[numofvert +4].y = h[POINT(x+1, y)].h;     v[numofvert +5].y = h[POINT(x+1, y+1)].h;
                v[numofvert +3].z = y * 5;                v[numofvert +4].z = y * 5;                  v[numofvert +5].z = (y+1) * 5;

                /*Triangle #3*/
                v[numofvert +6].x = (x-0.5) * 5;            v[numofvert +7].x = x * 5;                  v[numofvert +8].x = (x+0.5) * 5;
                v[numofvert +6].y = h[POINT(x, y+1)].h;     v[numofvert +7].y = h[POINT(x, y+2)].h;     v[numofvert +8].y = h[POINT(x+1, y+1)].h;
                v[numofvert +6].z = (y+1) * 5;              v[numofvert +7].z = (y+2) * 5;              v[numofvert +8].z = (y+1) * 5;

                /*Triangle #4*/
                v[numofvert+9].x = x * 5;                  v[numofvert +10].x = (x+0.5) * 5;              v[numofvert +11].x = (x+1) * 5;
                v[numofvert+9].y = h[POINT(x, y+2)].h;     v[numofvert +10].y = h[POINT(x+1, y+1)].h;     v[numofvert +11].y = h[POINT(x+1, y+2)].h;
                v[numofvert+9].z = (y+2) * 5;              v[numofvert +10].z = (y+1) * 5;                v[numofvert +11].z = (y+2) * 5;

                /*Triangle #5*/
                v[numofvert +12].x = (x+1) * 5;              v[numofvert +13].x = (x+0.5) * 5;              v[numofvert +14].x = (x+1.5) * 5;
                v[numofvert +12].y = h[POINT(x+1, y)].h;     v[numofvert +13].y = h[POINT(x+1, y+1)].h;     v[numofvert +14].y = h[POINT(x+2, y+1)].h;
                v[numofvert +12].z = y * 5;                  v[numofvert +13].z = (y+1) * 5;                v[numofvert +14].z = (y+1) * 5;

                /*Triangle #6*/
                v[numofvert +15].x = (x+0.5) * 5;              v[numofvert +16].x = (x+1) * 5;                v[numofvert +17].x = (x+1.5) * 5;
                v[numofvert +15].y = h[POINT(x+1, y+1)].h;     v[numofvert +16].y = h[POINT(x+1, y+2)].h;     v[numofvert +17].y = h[POINT(x+2, y+1)].h;
                v[numofvert +15].z = (y+1) * 5;                v[numofvert +16].z = (y+2) * 5;                v[numofvert +17].z = (y+1) * 5;

                numberoftriangles += 6;
                numofvert = numberoftriangles * 3;
            }
            if(x != 0)
            {
                if(y == MapSize - 3) break; /*If we are at the second last cell of the row, then stop*/

                /*Triangle #1 */
                v[numofvert].x = (x-1.5) * 5;              v[numofvert +1].x = (x-2) * 5;                v[numofvert +2].x = (x-1) * 5;
                v[numofvert].y = h[POINT(x-1, y+1)].h;     v[numofvert +1].y = h[POINT(x-2, y+2)].h;     v[numofvert +2].y = h[POINT(x-1, y+2)].h;
                v[numofvert].z = (y+1) * 5;                v[numofvert +1].z = (y+2) * 5;                v[numofvert +2].z = (y+2) * 5;

                /*Triangle #2*/
                v[numofvert +3].x = (x-2) * 5;                v[numofvert +4].x = (x-1.5) * 5;              v[numofvert +5].x = (x-1) * 5;
                v[numofvert +3].y = h[POINT(x-2, y+2)].h;     v[numofvert +4].y = h[POINT(x-1, y+3)].h;     v[numofvert +5].y = h[POINT(x-1, y+2)].h;
                v[numofvert +3].z = (y+2) * 5;                v[numofvert +4].z = (y+3) * 5;                v[numofvert +5].z = (y+2) * 5;

                /*Triangle #3*/
                v[numofvert +6].x = (x-1.5) * 5;              v[numofvert +7].x = (x-1) * 5;                v[numofvert +8].x = (x-0.5) * 5;
                v[numofvert +6].y = h[POINT(x-1, y+1)].h;     v[numofvert +7].y = h[POINT(x-1, y+2)].h;     v[numofvert +8].y = h[POINT(x, y+1)].h;
                v[numofvert +6].z = (y+1) * 5;                v[numofvert +7].z = (y+2) * 5;                v[numofvert +8].z = (y+1) * 5;

                /*Triangle #4*/
                v[numofvert +9].x = (x-1) * 5;                v[numofvert +10].x = (x-0.5) * 5;              v[numofvert +11].x = x * 5;
                v[numofvert +9].y = h[POINT(x-1, y+2)].h;     v[numofvert +10].y = h[POINT(x, y+1)].h;       v[numofvert +11].y = h[POINT(x, y+2)].h;
                v[numofvert +9].z = (y+2) * 5;                v[numofvert +10].z = (y+1) * 5;                v[numofvert +11].z = (y+2) * 5;

                /*Triangle #5*/
                v[numofvert +12].x = (x-1) * 5;               v[numofvert +13].x = (x-1.5) * 5;              v[numofvert +14].x = (x-0.5) * 5;
                v[numofvert +12].y = h[POINT(x-1, y+2)].h;    v[numofvert +13].y = h[POINT(x-1, y+3)].h;     v[numofvert +14].y = h[POINT(x, y+3)].h;
                v[numofvert +12].z = (y+2) * 5;               v[numofvert +13].z = (y+3) * 5;                v[numofvert +14].z = (y+3) * 5;

                /*Triangle #6*/
                v[numofvert +15].x = (x-1) * 5;                v[numofvert +16].x = x * 5;                  v[numofvert +17].x = (x-0.5) * 5;
                v[numofvert +15].y = h[POINT(x-1, y+2)].h;     v[numofvert +16].y = h[POINT(x, y+2)].h;     v[numofvert +17].y = h[POINT(x, y+3)].h;
                v[numofvert +15].z = (y+2) * 5;                v[numofvert +16].z = (y+2) * 5;              v[numofvert +17].z = (y+3) * 5;

                numberoftriangles += 6;
                numofvert = numberoftriangles * 3;
            }
        }
    }
    return numofvert;
}

void Map_SetColor(Vertexarray *v, int height, int index)
{
    float r,g,b;

    r = g = b = 0;

    if(height == 0)
    {
        printf("H = %d: Väri sininen\n",height);
        r = 0; g = 0; b = 1;
    }
    else
    {
      //  printf("H = %d: Väri musta\n",height);
        r = 0; g = 1; b = 0;
    }

    v[index].x = r;
    v[index].y = g;
    v[index].z = b;
}

int Map_CreateColorData(Vertexarray *v, MAP *map, int MapSize)
{
    int x, y, i;
    int numofvert = 0;

    for (x = 0; x < MapSize; x+=3)
    {
        for (y = 0; y < MapSize; y+=2)
        {
            if(x <= MapSize - 3)
            {
                if(y == MapSize -1) break;

                for(i = 0; i < 18; i++)
                {
                    Map_SetColor(v, map[POINT(x,y)].h, numofvert);
                    numofvert += 1;
                }
            }
            if(x != 0)
            {
                if(y == MapSize - 3) break; /*If we are at the second last cell of the row, then stop*/

                for(i = 0; i < 18; i++)
                {
                    Map_SetColor(v, map[POINT(x,y)].h, numofvert);
                    numofvert += 1;
                }
            }
        }
    }
    return numofvert;
}

int Map_CreateNormalData(Vertexarray *v, MAP *h, int MapSize)
{
    int x, y;
    int numoftri = 0;

    for (x = 0; x < MapSize; x+=3)
    {
        for (y = 0; y < MapSize; y+=2)
        {
            if(x <= MapSize - 3)
            {
                Triangle t;
                Vector normal;

                if(y == MapSize -1) break;

                t.v[0].x = x * 5;                t.v[1].x = (x-0.5) * 5;            t.v[2].x = (x+0.5) * 5;
                t.v[0].y = h[POINT(x, y)].h;     t.v[1].y = h[POINT(x, y+1)].h;     t.v[2].y = h[POINT(x+1, y+1)].h;
                t.v[0].z = y * 5;                t.v[1].z = (y+1) * 5;              t.v[2].z = (y+1) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;

                /*Triangle #2*/
                t.v[0].x = x * 5;                t.v[1].x = (x+1) * 5;              t.v[2].x = (x+0.5) * 5;
                t.v[0].y = h[POINT(x, y)].h;     t.v[1].y = h[POINT(x+1, y)].h;     t.v[2].y = h[POINT(x+1, y+1)].h;
                t.v[0].z = y * 5;                t.v[1].z = y * 5;                  t.v[2].z = (y+1) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;

                /*Triangle #3*/
                t.v[0].x = (x-0.5) * 5;            t.v[1].x = x * 5;                  t.v[2].x = (x+0.5) * 5;
                t.v[0].y = h[POINT(x, y+1)].h;     t.v[1].y = h[POINT(x, y+2)].h;     t.v[2].y = h[POINT(x+1, y+1)].h;
                t.v[0].z = (y+1) * 5;              t.v[1].z = (y+2) * 5;              t.v[2].z = (y+1) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;

                /*Triangle #4*/
                t.v[0].x = x * 5;                  t.v[1].x = (x+0.5) * 5;              t.v[2].x = (x+1) * 5;
                t.v[0].y = h[POINT(x, y+2)].h;     t.v[1].y = h[POINT(x+1, y+1)].h;     t.v[2].y = h[POINT(x+1, y+2)].h;
                t.v[0].z = (y+2) * 5;              t.v[1].z = (y+1) * 5;                t.v[2].z = (y+2) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;

                /*Triangle #5*/
                t.v[0].x = (x+1) * 5;              t.v[1].x = (x+0.5) * 5;              t.v[2].x = (x+1.5) * 5;
                t.v[0].y = h[POINT(x+1, y)].h;     t.v[1].y = h[POINT(x+1, y+1)].h;     t.v[2].y = h[POINT(x+2, y+1)].h;
                t.v[0].z = y * 5;                  t.v[1].z = (y+1) * 5;                t.v[2].z = (y+1) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;

                /*Triangle #6*/
                t.v[0].x = (x+0.5) * 5;              t.v[1].x = (x+1) * 5;                t.v[2].x = (x+1.5) * 5;
                t.v[0].y = h[POINT(x+1, y+1)].h;     t.v[1].y = h[POINT(x+1, y+2)].h;     t.v[2].y = h[POINT(x+2, y+1)].h;
                t.v[0].z = (y+1) * 5;                t.v[1].z = (y+2) * 5;                t.v[2].z = (y+1) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;
            }
            if(x != 0)
            {
                Triangle t;
                Vector normal;

                if(y == MapSize - 3) break; /*If we are at the second last cell of the row, then stop*/

                t.v[0].x = x * 5;                t.v[1].x = (x-0.5) * 5;            t.v[2].x = (x+0.5) * 5;
                t.v[0].y = h[POINT(x, y)].h;     t.v[1].y = h[POINT(x, y+1)].h;     t.v[2].y = h[POINT(x+1, y+1)].h;
                t.v[0].z = y * 5;                t.v[1].z = (y+1) * 5;              t.v[2].z = (y+1) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;

                /*Triangle #2*/
                t.v[0].x = x * 5;                t.v[1].x = (x+1) * 5;              t.v[2].x = (x+0.5) * 5;
                t.v[0].y = h[POINT(x, y)].h;     t.v[1].y = h[POINT(x+1, y)].h;     t.v[2].y = h[POINT(x+1, y+1)].h;
                t.v[0].z = y * 5;                t.v[1].z = y * 5;                  t.v[2].z = (y+1) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;

                /*Triangle #3*/
                t.v[0].x = (x-0.5) * 5;            t.v[1].x = x * 5;                  t.v[2].x = (x+0.5) * 5;
                t.v[0].y = h[POINT(x, y+1)].h;     t.v[1].y = h[POINT(x, y+2)].h;     t.v[2].y = h[POINT(x+1, y+1)].h;
                t.v[0].z = (y+1) * 5;              t.v[1].z = (y+2) * 5;              t.v[2].z = (y+1) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;

                /*Triangle #4*/
                t.v[0].x = x * 5;                  t.v[1].x = (x+0.5) * 5;              t.v[2].x = (x+1) * 5;
                t.v[0].y = h[POINT(x, y+2)].h;     t.v[1].y = h[POINT(x+1, y+1)].h;     t.v[2].y = h[POINT(x+1, y+2)].h;
                t.v[0].z = (y+2) * 5;              t.v[1].z = (y+1) * 5;                t.v[2].z = (y+2) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;

                /*Triangle #5*/
                t.v[0].x = (x+1) * 5;              t.v[1].x = (x+0.5) * 5;              t.v[2].x = (x+1.5) * 5;
                t.v[0].y = h[POINT(x+1, y)].h;     t.v[1].y = h[POINT(x+1, y+1)].h;     t.v[2].y = h[POINT(x+2, y+1)].h;
                t.v[0].z = y * 5;                  t.v[1].z = (y+1) * 5;                t.v[2].z = (y+1) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;

                /*Triangle #6*/
                t.v[0].x = (x+0.5) * 5;              t.v[1].x = (x+1) * 5;                t.v[2].x = (x+1.5) * 5;
                t.v[0].y = h[POINT(x+1, y+1)].h;     t.v[1].y = h[POINT(x+1, y+2)].h;     t.v[2].y = h[POINT(x+2, y+1)].h;
                t.v[0].z = (y+1) * 5;                t.v[1].z = (y+2) * 5;                t.v[2].z = (y+1) * 5;

                normal = Map_CalculateNormal(t);

                v[numoftri].x = normal.x;
                v[numoftri].y = normal.y;
                v[numoftri].z = normal.z;

                numoftri++;
            }
        }
    }
    return numoftri * 3;
}

Vector Map_CalculateNormal(Triangle t)
{
    Vector normal;
    Vector U;
    Vector V;
    double magnitude;

    U.x = t.v[1].x - t.v[0].x;
    U.y = t.v[1].y - t.v[0].y;
    U.z = t.v[1].z - t.v[0].z;

    V.x = t.v[2].x - t.v[0].x;
    V.y = t.v[2].y - t.v[0].y;
    V.z = t.v[2].z - t.v[0].z;

    normal.x = (U.y * V.z) - (U.z * V.y);
    normal.y = (U.z * V.x) - (U.x * V.z);
    normal.z = (U.x * V.y) - (U.y * V.x);

    magnitude = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

    normal.x = normal.x/magnitude;
    normal.y = normal.y/magnitude;
    normal.z = normal.z/magnitude;

    return normal;
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
