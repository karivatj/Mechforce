/* Mechforce
 *
 * @author Kari Vatjus-Anttila <karidaserious@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 *
 * main.c 1.00 by Kari Vatjus-Anttila
 *
 */

#ifdef WIN32
#include <windows.h>
#endif

#define WINDOWS

#include <time.h>

#include "main.h"
#include "Core/SDL_Engine.h"
#include "Core/SDL_Textures.h"
#include "Core/SDL_DrawText.h"
#include "Sound/Sound.h"
#include "Utils/Utilities.h"
#include "MapRoutines.h"
#include "EventHandler.h"
#include "Widget.h"

#include <FreeImage.h>

STATE state;

int main(int argc, char **argv)
{
    printf("ASD");
    int done = 0, value = 0;
    orthogonalEnabled = 1;

    SCREEN_WIDTH = 800;
    SCREEN_HEIGHT = 600;

    srand (time(NULL));   //Initialize random seed

    //FreeImage_SetOutputMessage(Texture_HandleFreeImageError);

    camerax = 0;
    cameray = 15;
    cameraz = -200;

    rotx = 45;
    roty = 0;

    Init_SDL(SCREEN_WIDTH, SCREEN_HEIGHT);
    Init_GL();

    SDL_EnableKeyRepeat(50, 10);

    Texture_LoadTextures();
    Texture_GenerateTilemap();
    Widget_ReadButtonData();
    Sound_LoadSounds();
    Map_AllocateMemoryForMapData();
    Map_GenerateMap();

#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif // FREEIMAGE_LIB

    if((font[0] = FTGL::ftglCreatePixmapFont("../Fonts/kimberle.ttf")) == NULL)
    {
        printf("ERROR*** Failed to load ../Fonts/kimberle.ttf.\n");
        SDL_Close(-1);
    }

    printf("\t../Fonts/kimberle.ttf OK\n");

    FTGL::ftglSetFontCharMap(font[0], ft_encoding_unicode);

    if((font[1] = FTGL::ftglCreatePixmapFont("../Fonts/cour.ttf")) == NULL)
    {
        printf("ERROR*** Failed to load ../Fonts/cour.ttf.\n");
        SDL_Close(-1);
    }

    printf("\t../Fonts/cour.ttf OK\n\n");
    FTGL::ftglSetFontCharMap(font[1], ft_encoding_unicode);

    state = STATE_RELOAD_CONFIG;

    while(!done)			//The main loop
    {
        framestart = SDL_GetTicks();

        value = MF_Event_Handler();

        SDL_DrawScene(SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_GL_SwapBuffers();

        Utils_CountFPS();

        if(value == 1)
            done = 1;
    }

    SDL_Close(0);
    return(0);
}
