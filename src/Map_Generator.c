#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Map.h"

void MAP_GenerateMap()   //Generoidaan randomilla kartta
{
#if 1
    float *fa = alloc2DFractArray(32);
    int size = 32;
    float heightScale= 200;
    float h = 1;

    int	stride;
    int	oddline;
    int subSize;
	float ratio, scale;

    /* subSize is the dimension of the array in terms of connected line
       segments, while size is the dimension in terms of number of
       vertices. */

    subSize = size;
    size++;

    /* initialize random number generator */
    srand (time(NULL));

	/* Set up our roughness constants.
	   Random numbers are always generated in the range 0.0 to 1.0.
	   'scale' is multiplied by the randum number.
	   'ratio' is multiplied by 'scale' after each iteration
	   to effectively reduce the randum number range.
	   */

	ratio = (float) pow (2.0,-h);
	scale = heightScale * ratio;

    /* Seed the first four values. For example, in a 4x4 array, we
       would initialize the data points indicated by '*':

           *   .   .   .   *

           .   .   .   .   .

           .   .   .   .   .

           .   .   .   .   .

           *   .   .   .   * */

    stride = subSize / 2;

    fa[(0*size)+0] =  0.f;
    fa[(subSize*size)+0] =  0.f;
    fa[(subSize*size)+subSize] = 0.f;
    fa[(0*size)+subSize] = 0.f;

    /* Now we add ever-increasing detail based on the "diamond" seeded
       values. We loop over stride, which gets cut in half at the
       bottom of the loop. Since it's an int, eventually division by 2
       will produce a zero result, terminating the loop. */
    while (stride)
    {
        int i,j;

		for (i=stride; i<subSize; i+=stride)
		{
			for (j=stride; j<subSize; j+=stride)
			{
				fa[(i * size) + j] = scale * randnum(0, 2) + avgSquareVals (i, j, stride, size, fa);
				j += stride;
			}
			i += stride;
		}

		/* Take the existing "diamond" data and make it into
	       "squares". Back to our 4X4 example: The first time we
	       encounter this code, the existing values are represented by
	       "X"s, and the values we want to generate here are "*"s:

               X   .   *   .   X

               .   .   .   .   .

               *   .   X   .   *

               .   .   .   .   .

               X   .   *   .   X

	       i and j represent our (x,y) position in the array. The
	       first value we want to generate is at (i=2,j=0), and we use
	       "oddline" and "stride" to increment j to the desired value.
	       */
		oddline = 0;
		for (i=0; i<subSize; i+=stride)
		{
		    oddline = (oddline == 0);

			for (j=0; j<subSize; j+=stride)
			{
				if ((oddline) && !j)
                j+=stride;

				/* i and j are setup. Call avgDiamondVals with the
				   current position. It will return the average of the
				   surrounding diamond data points. */
				fa[(i * size) + j] = scale * randnum (0, 2) + avgDiamondVals (i, j, stride, size, subSize, fa);

				/* To wrap edges seamlessly, copy edge values around
				   to other side of array */
				if (i==0)
					fa[(subSize*size) + j] = fa[(i * size) + j];

				if (j==0)
					fa[(i*size) + subSize] = fa[(i * size) + j];

				j+=stride;
			}
		}

		/* reduce random number range. */
		scale *= ratio;
		stride /= 2;   //Siirretään yks bitti oikealle
    }

//

    CopyArray(fa);

    //FindMinMax(fa,32);

    //MAP_SetTile();
    //dump2DFractArray (fa, 32);
    free(fa);

#endif
}


float randnum (float min, float max)
{
        int random;
        float	x;

        random = rand ();
        x = (float)(random & 0x7fff) / (float)0x7fff;

        return (x * (max - min) + min);
}

float avgDiamondVals (int i, int j, int stride, int size, int subSize, float *fa)
{
    /* In this diagram, our input stride is 1, the i,j location is
       indicated by "X", and the four value we want to average are
       "*"s:
           .   *   .

           *   X   *

           .   *   .
       */

    /* In order to support tiled surfaces which meet seamless at the
       edges (that is, they "wrap"), We need to be careful how we
       calculate averages when the i,j diamond center lies on an edge
       of the array. The first four 'if' clauses handle these
       cases. The final 'else' clause handles the general case (in
       which i,j is not on an edge).
     */
    if (i == 0)
	return ((float)(fa[(i*size) + j-stride] + fa[(i*size) + j+stride] + fa[((subSize-stride)*size) + j] + fa[((i+stride)*size) + j]) * 0.25f);

    else if (i == size-1)
	return ((float)(fa[(i*size) + j-stride] +  fa[(i*size) + j+stride] + fa[((i-stride)*size) + j] + fa[((0+stride)*size) + j]) * 0.25f);

    else if (j == 0)
	return ((float)(fa[((i-stride)*size) + j] + fa[((i+stride)*size) + j] + fa[(i*size) + j+stride] + fa[(i*size) + subSize-stride]) * 0.25f);

    else if (j == size-1)
	return ((float)(fa[((i-stride)*size) + j] + fa[((i+stride)*size) + j] + fa[(i*size) + j-stride] + fa[(i*size) + 0+stride]) * 0.25f);

    else
	return ((float)(fa[((i-stride)*size) + j] + fa[((i+stride)*size) + j] + fa[(i*size) + j-stride] + fa[(i*size) + j+stride]) * 0.25f);
}

float avgSquareVals (int i, int j, int stride, int size, float *fa)
{
        /* In this diagram, our input stride is 1, the i,j location is
           indicated by "*", and the four value we want to average are
           "X"s:
               X   .   X

               .   *   .

               X   .   X
           */
        return ((float) (fa[((i-stride)*size) + j-stride] + fa[((i-stride)*size) + j+stride] + fa[((i+stride)*size) + j-stride] + fa[((i+stride)*size) + j+stride]) * 0.25f);
}

float *alloc2DFractArray (int size)
{
        /* For a sizeXsize array, we need (size+1)X(size+1) space. For
           example, a 2x2 mesh needs 3x3=9 data points:

               *   *   *

               *   *   *

               *   *   *

           To account for this, increment 'size'. */
        size++;

    return ((float *) malloc (sizeof(float) * size * size));
}

void dump2DFractArray (float *fa, int size)
{
    int	i, j;

    for (i=0; i<MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            printf ("[%.0f] [%.0f] [%.0f]\n", Map_VertexArray[PAIKKA(j,i)].x,Map_VertexArray[PAIKKA(j,i)].y,Map_VertexArray[PAIKKA(j,i)].z);
        }
        printf ("\n");
    }
}

int FindMinMax(float *fa, int size)
{
    float min = 0;
    float max = 0;

    int	i, j;

    for (i=0; i<size; i++)
    {

        for (j=0; j<size; j++)
        {
                if(max <= fa[(i*size)+j])
                max = fa[PAIKKA(j,i)];

                else if(min >= fa[(i*size)+j])
                min = fa[(i*size)+j];
        }

    }
    printf("Suurin: %.2f Pienin: %.2f \n",max, min);
    return min;
}

void CopyArray(float *fa)
{
    int	x, y;
    //int pienin = FindMinMax(fa,20);

    for (y=0; y<MAP_HEIGHT; y++)
    {
        for (x=0; x<MAP_WIDTH; x++)
        {
            Map_VertexArray[PAIKKA(x,y)].x = -10 + x;

            if(fa[PAIKKA(x,y)] > 250)
            Map_VertexArray[PAIKKA(x,y)].y = 250 / 100;

            else
            Map_VertexArray[PAIKKA(x,y)].y = fa[PAIKKA(x,y)]/ 100;

            Map_VertexArray[PAIKKA(x,y)].z = y - 20;
        }
    }
}
