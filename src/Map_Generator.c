#include <stdlib.h>
#include <stdio.h>

#include "Map.h"

//Generate the map using Diamond-Square Algorithm
void MAP_GenerateMap()
{
    int sideLength;

    int x;
    int y;
    double h = 50;

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
                double  avg =   Map[POINT((x - halfSide  + MAP_SIZE) % (MAP_SIZE), y)].height +        //left
                                Map[POINT((x + halfSide) % MAP_SIZE - 1, y)].height +                  //right
                                Map[POINT(x, (y + halfSide) % MAP_SIZE - 1)].height +                  //below
                                Map[POINT(x, (y - halfSide  + MAP_SIZE - 1) % (MAP_SIZE - 1))].height;   //above center

                avg /= 4.0;

                avg = avg + ((double)rand() / (double)RAND_MAX * 2 * h) - h;

                Map[POINT(x,y)].height = avg;

                if(x == 0)  Map[POINT(MAP_SIZE-1,y)].height = avg;
                if(y == 0)  Map[POINT(x,MAP_SIZE-1)].height = avg;
            }
        }
    }

    for (x = 0; x < MAP_SIZE; x++)
    {
        for (y = 0; y < MAP_SIZE; y++)
        {
            Map_VertexArray[POINT(x,y)].x = -200 + x * 5;

            Map_VertexArray[POINT(x,y)].y = Map[POINT(x,y)].height;

            Map_VertexArray[POINT(x,y)].z = -200 + y * 5;
        }
    }

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
#endif
}







