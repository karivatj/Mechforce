#include <stdlib.h>
#include <stdio.h>

#include "Map.h"

//Generate the map using Diamond-Square Algorithm

void MAP_GenerateMap()
{
    int sideLength;

    int x;
    int y;
    double h = 500.0;   //the range (-h -> +h) for the average offset

    //seed the data
    Map[POINT(0,0)] = Map[POINT(0,MAP_SIZE-1)] = Map[POINT(MAP_SIZE-1,0)] = Map[POINT(MAP_SIZE-1,MAP_SIZE-1)] = SEED;

    //side length is distance of a single square side
    //or distance of diagonal in diamond

    //side length must be >= 2 so we always have
    //a new value (if its 1 we overwrite existing values
    //on the last iteration)

    //each iteration we are looking at smaller squares
    //diamonds, and we decrease the variation of the offset

    for(sideLength = MAP_SIZE-1; sideLength >= 2; sideLength /=2, h/= 2.0)
    {
        //half the length of the side of a square
        //or distance from diamond center to one corner
        //(just to make calcs below a little clearer)
        int halfSide = sideLength/2;

        //generate the new square values
        for(x=0;x<MAP_SIZE-1;x+=sideLength)
        {
            for(y=0;y<MAP_SIZE-1;y+=sideLength)
            {
            //x, y is upper left corner of square
            //calculate average of existing corners
            double avg = Map[POINT(x,y)] + //top left
            Map[POINT(x+sideLength,y)] +//top right
            Map[POINT(x,y+sideLength)] + //lower left
            Map[POINT(x+sideLength,y+sideLength)];//lower right
            avg /= 4.0;

            //center is average plus random offset
            Map[POINT(x+halfSide,y+halfSide)] =
            //We calculate random value in range of 2h
            //and then subtract h so the end value is
            //in the range (-h, +h)
            avg + ((double)rand()/(double)RAND_MAX * 2 * h) - h; //        (r.nextDouble()*2*h) - h;
            }
        }

        //generate the diamond values
        //since the diamonds are staggered we only move x
        //by half side
        //NOTE: if the Map shouldn't wrap then x < MAP_SIZE
        //to generate the far edge values
        for(x=0;x<MAP_SIZE-1;x+=halfSide)
        {
          //and y is x offset by half a side, but moved by
          //the full side length
          //NOTE: if the data shouldn't wrap then y < MAP_SIZE
          //to generate the far edge values
            for(y=(x+halfSide)%sideLength;y<MAP_SIZE-1;y+=sideLength)
            {
                //x, y is center of diamond
                //note we must use mod  and add MAP_SIZE for subtraction
                //so that we can wrap around the array to find the corners
                double avg =
                Map[POINT((x - halfSide + MAP_SIZE) % MAP_SIZE, y)] + //left of center
                Map[POINT((x + halfSide) % MAP_SIZE, y)] + //right of center
                Map[POINT(x, (y + halfSide) % MAP_SIZE)] + //below center
                Map[POINT(x,(y - halfSide + MAP_SIZE) % MAP_SIZE)]; //above center

                avg /= 4.0;

                //new value = average plus random offset
                //We calculate random value in range of 2h
                //and then subtract h so the end value is
                //in the range (-h, +h)
                avg = avg + ((double)rand() / (double)RAND_MAX * 2 * h) - h;
                //update value for center of diamond
                Map[POINT(x,y)] = avg;

                //wrap values on the edges, remove
                //this and adjust loop condition above
                //for non-wrapping values.
                if(x == 0)  Map[POINT(MAP_SIZE-1,y)] = avg;
                if(y == 0)  Map[POINT(x,MAP_SIZE-1)] = avg;
            }
        }
    }

#if 0
    //Print out the Map
    for(x = 0; x < MAP_SIZE; x++)
    {
        for(y = 0; y < MAP_SIZE; y++)
        {
            printf("[%f8.3] ",Map[POINT(x,y)]);
        }
        printf("\n");
    }
#endif
}







