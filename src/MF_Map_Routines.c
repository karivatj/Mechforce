#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Mechforce.h"
#include "SDL_Engine.h"
#include "Map.h"

void MAP_LoadMap (char keyword[]) //Koetetaan ladata parametrina annettu kartta muistiin.
{
#if 0
    FILE *tiedosto;
    char string[128];
    char *token = NULL;
    int done = 0;
    int x = 0, y = 0;

    if((tiedosto = fopen ("../Data/map.txt", "r")) == NULL)
    {
        fprintf(stderr,"ERROR*** Couldn't open Map file! (./Data/map.txt)\n");
        SDL_Close(-1);
    }

    printf("Opening map datafile.\nPreparing to read mapdata.\n");

    while(!feof(tiedosto))
    {
        fgets(string,64,tiedosto);

        if(strstr(string, keyword))
        {
            printf("Found %s section. Starting to read data.\n",keyword);
            while(!feof(tiedosto))
            {
                fgets(string,64,tiedosto);

                token = strtok(string," ");

                while(token != NULL)
                {
                    switch((*token))
                    {
                        case 'E':
                            done = 1;
                            break;

                        default:
                            Map[PAIKKA(x,y)].type = atoi(token);
                            Map[PAIKKA(x,y)].korkeus = 0;
                            x++;
                            break;
                    }

                    if(done)
                    break;

                    token = strtok (NULL, " ");
                }

                y++;
                x=0;

                if(done)
                break;
            }
        }

        if(done)
        {
            printf("Succesfully read mapdata.\n");
            break;
        }
    }

    fclose (tiedosto);
#endif
}


void MAP_DrawTileMap()
{
#if 0
    int cx,cy;
    OrthogonalStart();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    for(cy = 0; cy < MAP_SIZE; cy++)
    {
        for(cx = 0; cx < MAP_SIZE; cx++)
        {
            if(cy % 2)
            {
                SDL_DrawTile(Map[PAIKKA(cx,cy)].type, 68 + (cx * 32), 500 - (cy * 16) - (cy * 8));
            }
            else
            {
                SDL_DrawTile(Map[PAIKKA(cx,cy)].type, 84 + (cx * 32), 500 - (cy * 16) - (cy * 8));
            }
        }
    }

    glDisable(GL_BLEND);
    OrthogonalEnd();
#endif
}

void MAP_DrawFractalMap(void)
{
    int i,j;
    Vertexarray temp[MAP_SIZE * MAP_SIZE];

	glEnableClientState(GL_VERTEX_ARRAY);   //We want a vertex array

	glVertexPointer(3, GL_FLOAT, 0, &Map_VertexArray[0]);   //All values are grouped to three Floats, we start at the beginning of the array (offset=0) and want to use as VertexArray

    for(i = 0; i<MAP_SIZE; i++)
    {
        glDrawArrays(GL_LINE_STRIP, MAP_SIZE * i, MAP_SIZE); //We draw the first three vertices in the array as a triangle
    }

        for(i=0; i < MAP_SIZE; i++)
	{
            for(j=0; j < MAP_SIZE; j++)
	    {
	        temp[i * 20 + j].x = Map_VertexArray[j * 20 + i].x;
	        temp[i * 20 + j].y = Map_VertexArray[j * 20 + i].y;
	        temp[i * 20 + j].z = Map_VertexArray[j * 20 + i].z;
	    }
	}

	glVertexPointer(3, GL_FLOAT, 0, &temp[0]);   //All values are grouped to three Floats, we start at the beginning of the array (offset=0) and want to use as VertexArray

    for(i = 0; i<MAP_SIZE; i++)
    {
        glDrawArrays(GL_LINE_STRIP, MAP_SIZE * i, MAP_SIZE);
    }

	glDisableClientState(GL_VERTEX_ARRAY);
}

void MAP_SetTile()
{
#if 0
    int x,y;

    for(y = 0; y < MAP_SIZE; y++)
    {
        for(x = 0; x < MAP_SIZE; x++)
        {
            if(Map[PAIKKA(x,y)].korkeus < -0.20)
            Map[PAIKKA(x,y)].type = 11;

            else if(Map[PAIKKA(x,y)].korkeus < -0.16 && Map[PAIKKA(x,y)].korkeus >= -0.20)//vesi
            Map[PAIKKA(x,y)].type = 10;

            else if(Map[PAIKKA(x,y)].korkeus < -0.13 && Map[PAIKKA(x,y)].korkeus >= -0.16)
            Map[PAIKKA(x,y)].type = 9;

            else if(Map[PAIKKA(x,y)].korkeus < -0.07 && Map[PAIKKA(x,y)].korkeus >= -0.13)
            Map[PAIKKA(x,y)].type = 8;

            else if(Map[PAIKKA(x,y)].korkeus < -0.05 && Map[PAIKKA(x,y)].korkeus >= -0.07)//metsät
            Map[PAIKKA(x,y)].type = 4;

            else if(Map[PAIKKA(x,y)].korkeus > 0.5 && Map[PAIKKA(x,y)].korkeus <= 0.10)
            Map[PAIKKA(x,y)].type = 5;

            else if(Map[PAIKKA(x,y)].korkeus > 0.10 && Map[PAIKKA(x,y)].korkeus <= 0.14)
            Map[PAIKKA(x,y)].type = 6;

            else if(Map[PAIKKA(x,y)].korkeus > 0.14 && Map[PAIKKA(x,y)].korkeus <= 0.15)
            Map[PAIKKA(x,y)].type = 7;

            else if(Map[PAIKKA(x,y)].korkeus > 0.15 && Map[PAIKKA(x,y)].korkeus <= 0.20)//vuoret
            Map[PAIKKA(x,y)].type = 12;

            else if(Map[PAIKKA(x,y)].korkeus > 0.20 && Map[PAIKKA(x,y)].korkeus <= 0.24)
            Map[PAIKKA(x,y)].type = 13;

            else if(Map[PAIKKA(x,y)].korkeus > 0.24 && Map[PAIKKA(x,y)].korkeus <= 0.26)
            Map[PAIKKA(x,y)].type = 14;

            else if(Map[PAIKKA(x,y)].korkeus > 0.26)
            Map[PAIKKA(x,y)].type = 15;

            else
            Map[PAIKKA(x,y)].type = 0;
        }
    }
#endif
}
