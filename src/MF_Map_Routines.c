#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Mechforce.h"
#include "SDL_Engine.h"
#include "Map.h"

void MAP_LoadMap (char keyword[])
{
    FILE *file;
    char string[128];
    char *token = NULL;
    int done = 0;
    int x = 0, y = 0;

    if((file = fopen ("../Data/map.txt", "r")) == NULL)
    {
        fprintf(stderr,"ERROR*** Couldn't open Map file! (./Data/map.txt)\n");
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


void MAP_DrawTileMap()
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

void MAP_DrawFractalMap(void)
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

void MAP_SetTile()
{
    int x,y;

    for(y = 0; y < MAP_SIZE; y++)
    {
        for(x = 0; x < MAP_SIZE; x++)
        {
            if(Map[POINT(x,y)].height < 10)
                Map[POINT(x,y)].type = 11;

            else if(Map[POINT(x,y)].height < -0.16 && Map[POINT(x,y)].height >= -0.20)//vesi
                Map[POINT(x,y)].type = 10;

            else if(Map[POINT(x,y)].height < -0.13 && Map[POINT(x,y)].height >= -0.16)
                Map[POINT(x,y)].type = 9;

            else if(Map[POINT(x,y)].height < -0.07 && Map[POINT(x,y)].height >= -0.13)
                Map[POINT(x,y)].type = 8;

            else if(Map[POINT(x,y)].height < -0.05 && Map[POINT(x,y)].height >= -0.07)//metsät
                Map[POINT(x,y)].type = 4;

            else if(Map[POINT(x,y)].height > 0.5 && Map[POINT(x,y)].height <= 0.10)
                Map[POINT(x,y)].type = 5;

            else if(Map[POINT(x,y)].height > 0.10 && Map[POINT(x,y)].height <= 0.14)
                Map[POINT(x,y)].type = 6;

            else if(Map[POINT(x,y)].height > 0.14 && Map[POINT(x,y)].height <= 0.15)
                Map[POINT(x,y)].type = 7;

            else if(Map[POINT(x,y)].height > 0.15 && Map[POINT(x,y)].height <= 0.20)//vuoret
                Map[POINT(x,y)].type = 12;

            else if(Map[POINT(x,y)].height > 0.20 && Map[POINT(x,y)].height <= 0.24)
                Map[POINT(x,y)].type = 13;

            else if(Map[POINT(x,y)].height > 0.24 && Map[POINT(x,y)].height <= 0.26)
                Map[POINT(x,y)].type = 14;

            else if(Map[POINT(x,y)].height > 0.26)
                Map[POINT(x,y)].type = 15;

            else
                Map[POINT(x,y)].type = 0;
        }
    }
}
