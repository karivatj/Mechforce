#ifdef WIN32
#include <windows.h>
#endif

#include <time.h>

#include "Core/SDL_Engine.h"
#include "Core/SDL_Textures.h"
#include "Core/SDL_DrawText.h"
#include "main.h"
#include "MapRoutines.h"
#include "EventHandler.h"
#include "Widget.h"
#include "Sound/Sound.h"
#include "Utils/Utilities.h"

STATE state;

int main(int argc, char **argv)
{
    int done = 0, value = 0;
    orthogonalEnabled = 1;

    SCREEN_WIDTH = 800;
    SCREEN_HEIGHT = 600;

    srand (time(NULL));   //Initialize random seed

    Map_GenerateMap();

    camerax = 0;
    cameray = 15;
    cameraz = -200;

    rotx = 45;
    roty = 0;

    Init_SDL();
    Init_GL();

    SDL_EnableKeyRepeat(50, 10);

    Texture_LoadTextures();
    Texture_GenerateTilemap();
    BTN_ReadButtonData();
    Sound_LoadSounds();

    if((font[0] = ftglCreatePixmapFont("../Fonts/kimberle.ttf")) == NULL)
    {
        printf("ERROR*** Failed to load ../Fonts/kimberle.ttf.\n");
        SDL_Close(-1);
    }

    printf("\t../Fonts/kimberle.ttf OK\n");

    ftglSetFontCharMap(font[0], ft_encoding_unicode);

    if((font[1] = ftglCreatePixmapFont("../Fonts/cour.ttf")) == NULL)
    {
        printf("ERROR*** Failed to load ../Fonts/cour.ttf.\n");
        SDL_Close(-1);
    }

    printf("\t../Fonts/cour.ttf OK\n\n");
    ftglSetFontCharMap(font[1], ft_encoding_unicode);

    state = STATE_RELOAD_CONFIG;

    while(!done)			//The main loop
    {
        framestart = SDL_GetTicks();

        value = MF_Event_Handler();

        SDL_DrawScene();

        SDL_GL_SwapBuffers();

        Utils_CountFPS();

        if(value == 1)
            done = 1;
    }

    SDL_Close(0);
    return(0);
}
