#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Mechforce.h"
#include "SDL_Engine.h"
#include "Map.h"

void MAP_LoadMapFromFile(char keyword[])
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
                            Map[POINT(x,y)].height = 0;
                            x++;
                            break;
                    }

                    if(done)
                    break;

                    token = strtok (NULL, " ");
                }

                y++; x=0;

                if(done)
                    break;
            }
        }
        if(done)
            break;
    }

    fclose (file);
}


void MAP_Draw2DTerrain(void)
{

    int x,y;
    OrthogonalStart();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    for(y = 0; y < MAP_SIZE; y++)
    {
        for(x = 0; x < MAP_SIZE; x++)
        {
            if(y % 2)
                SDL_DrawTile(Map[POINT(x,y)].type, 68 + (x * 32), 500 - (y * 16) - (y * 8));
            else
                SDL_DrawTile(Map[POINT(x,y)].type, 84 + (x * 32), 500 - (y * 16) - (y * 8));
        }
    }

    glDisable(GL_BLEND);
    OrthogonalEnd();
}

void MAP_Draw3DTerrain(void)
{
    int i,j;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

    Vertexarray temp[MAP_SIZE * MAP_SIZE];

    glEnableClientState(GL_VERTEX_ARRAY);   //We want a vertex array

    glVertexPointer(3, GL_FLOAT, 0, &Map_VertexArray[0]);   //All values are grouped to three Floats, we start at the beginning of the array (offset=0) and want to use as VertexArray

    for(i = 0; i<MAP_SIZE; i++)
    {
        glDrawArrays(GL_LINE_STRIP, MAP_SIZE * i, MAP_SIZE); //We draw the first three vertices in the array as a triangle
    }

    for(i = 0; i < MAP_SIZE; i++)
    {
        for(j = 0; j < MAP_SIZE; j++)
        {
            temp[i * MAP_SIZE + j].x = Map_VertexArray[j * MAP_SIZE + i].x;
            temp[i * MAP_SIZE + j].y = Map_VertexArray[j * MAP_SIZE + i].y;
            temp[i * MAP_SIZE + j].z = Map_VertexArray[j * MAP_SIZE + i].z;
        }
    }

    glVertexPointer(3, GL_FLOAT, 0, &temp[0]);   //All values are grouped to three Floats, we start at the beginning of the array (offset=0) and want to use as VertexArray

    for(i = 0; i<MAP_SIZE; i++)
    {
        glDrawArrays(GL_LINE_STRIP, MAP_SIZE * i, MAP_SIZE);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
}

void MAP_SmoothTerrain(float k)
{
    int x,y;

#if 1
    /* Rows, left to right */
    for(x = 1;x < MAP_SIZE; x++)
        for (y = 0;y < MAP_SIZE; y++)
            Map[POINT(x,y)].height = Map[POINT(x-1,y)].height * (1-k) + Map[POINT(x,y)].height * k;

    /* Rows, right to left*/
    for(x = MAP_SIZE-2;x < -1; x--)
        for (y = 0;y < MAP_SIZE; y++)
            Map[POINT(x,y)].height = Map[POINT(x+1,y)].height * (1-k) + Map[POINT(x,y)].height * k;

    /* Columns, bottom to top */
    for(x = 0;x < MAP_SIZE; x++)
        for (y = 1;y < MAP_SIZE; y++)
           Map[POINT(x,y)].height = Map[POINT(x,y-1)].height * (1-k) + Map[POINT(x,y)].height * k;

    /* Columns, top to bottom */
    for(x = 0;x < MAP_SIZE; x++)
        for (y = MAP_SIZE; y < -1; y--)
            Map[POINT(x,y)].height = Map[POINT(x,y+1)].height * (1-k) + Map[POINT(x,y)].height * k;

    for (x = 0; x < MAP_SIZE; x++)
    {
        for (y = 0; y < MAP_SIZE; y++)
        {
            Map_VertexArray[POINT(x,y)].x = -200 + x * 5;

            Map_VertexArray[POINT(x,y)].y = Map[POINT(x,y)].height;

            Map_VertexArray[POINT(x,y)].z = -200 + y * 5;
        }
    }
#endif
}

//Generate the map using Diamond-Square Algorithm
void MAP_GenerateMap()
{
    int sideLength;

    int x;
    int y;
    double h = 100;

    //Seed the data
    Map[POINT(0,0)].height = Map[POINT(0,MAP_SIZE-1)].height = Map[POINT(MAP_SIZE-1,0)].height = Map[POINT(MAP_SIZE-1,MAP_SIZE-1)].height = SEED;

    for(sideLength = MAP_SIZE-1; sideLength >= 2; sideLength /=2, h/= 2.0)
    {
        int halfSide = sideLength/2;

        for(x=0;x<MAP_SIZE-1;x+=sideLength)
        {
            for(y=0;y<MAP_SIZE-1;y+=sideLength)
            {
                double avg =    Map[POINT(x,y)].height +                       //top left
                                Map[POINT(x+sideLength,y)].height +            //top right
                                Map[POINT(x,y+sideLength)].height +            //lower left
                                Map[POINT(x+sideLength,y+sideLength)].height;  //lower right
                avg /= 4.0;

                //center is average plus random offset
                Map[POINT(x+halfSide,y+halfSide)].height =

                avg + ((double)rand()/(double)RAND_MAX * 2 * h) - h;
            }
        }

        for(x=0;x<MAP_SIZE-1;x+=halfSide)
        {
            for(y=(x+halfSide)%sideLength;y<MAP_SIZE-1;y+=sideLength)
            {
                double  avg =   Map[POINT((x - halfSide  + MAP_SIZE) % (MAP_SIZE), y)].height +         //left
                                Map[POINT((x + halfSide) % MAP_SIZE - 1, y)].height +                   //right
                                Map[POINT(x, (y + halfSide) % MAP_SIZE - 1)].height +                   //below
                                Map[POINT(x, (y - halfSide  + MAP_SIZE - 1) % (MAP_SIZE - 1))].height;  //above center

                avg /= 4.0;

                avg = avg + ((double)rand() / (double)RAND_MAX * 2 * h) - h;

                Map[POINT(x,y)].height = avg;

                if(x == 0)  Map[POINT(MAP_SIZE-1,y)].height = avg;
                if(y == 0)  Map[POINT(x,MAP_SIZE-1)].height = avg;
            }
        }
    }

    MAP_SmoothTerrain(0.75);
    MAP_SmoothTerrain(0.75);
    MAP_SmoothTerrain(0.75);

#if 0
    //Print out the Map
    for(x = 0; x < MAP_SIZE; x++)
    {
        for(y = 0; y < MAP_SIZE; y++)
        {
            printf("[%.3f]\t",Map[POINT(x,y)].height);
        }
        printf("\n");
    }
    puts("");
#endif
}
