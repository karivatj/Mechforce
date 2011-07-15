#include "SDL_Engine.h"
#include "Mechforce.h"

int SDL_ScreenShot(void)
{
#if 0
    SDL_Surface *temp = NULL;
    unsigned char *pixels;
    int i;

    if (!(screen->flags & SDL_OPENGL))
    {
            SDL_SaveBMP(temp, "../Screenshots/Screenshot.bmp");
            return 0;
    }

    temp = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 24,
    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
    0x000000FF, 0x0000FF00, 0x00FF0000, 0
    #else
    0x00FF0000, 0x0000FF00, 0x000000FF, 0
    #endif
    );

    if (temp == NULL)
            return -1;

    pixels = malloc(3 * screen->w * screen->h);

    if (pixels == NULL)
    {
            SDL_FreeSurface(temp);
            return -1;
    }

    glReadPixels(0, 0, screen->w, screen->h, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    for (i=0; i<screen->h; i++)
            memcpy(((char *) temp->pixels) + temp->pitch * i, pixels + 3*screen->w * (screen->h-i-1), screen->w*3);

    free(pixels);

    SDL_SaveBMP(temp, "../Screenshots/Screenshot.bmp");

    SDL_FreeSurface(temp);
#endif
    return 0;
}

void SDL_CountFPS(void)
{
    static GLint T0 = 0;
    static GLint Frames = 0;
    GLint deltatime = 0;

    Frames++;

    deltatime = SDL_GetTicks() - framestart;

    if ((framestart - T0) >= 1000)
    {
        GLfloat seconds = (framestart - T0) / 1000.0;
        fps = Frames / seconds;
        printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
        T0 = framestart;
        Frames = 0;
    }

    if(deltatime < (1000 / FPS))
    {
        SDL_Delay((1000 / FPS) - deltatime);
        printf("delayed %d ms\n",(1000 / FPS) - deltatime);
    }
}

