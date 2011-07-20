#include "SDL_Engine.h"
#include "Mechforce.h"

int SDL_ScreenShot(void)
{
#if 1
    SDL_Surface *temp = NULL;
    unsigned char *pixels;
    int i;

    temp = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 24,
    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
    0x000000FF, 0x0000FF00, 0x00FF0000, 0
    #else
    0x00FF0000, 0x0000FF00, 0x000000FF, 0
    #endif
    );

    if (temp == NULL)
            return -1;

    pixels = malloc(4 * screen->w * screen->h);

    if (pixels == NULL)
    {
            SDL_FreeSurface(temp);
            return -1;
    }

    glReadPixels(0, 0, screen->w, screen->h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    for (i=0; i<screen->h; i++)
            memcpy(((char *) temp->pixels) + temp->pitch * i, pixels + 4*screen->w * (screen->h-i-1), screen->w*4);

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
        T0 = framestart;
        Frames = 0;
    }

    if(deltatime < (1000 / FPS))
        SDL_Delay((1000 / FPS) - deltatime);
}

void MF_DrawHUD(void)
{
    OrthogonalStart();

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST );

    glLoadIdentity();

    glBlendFunc(GL_DST_COLOR, GL_ZERO);
    glBindTexture(GL_TEXTURE_2D,backgrounds[3]);        //Mask
    glCallList(background);

    glLoadIdentity();

    glBlendFunc( GL_ONE, GL_ONE );
    glBindTexture(GL_TEXTURE_2D,backgrounds[2]);        //HUD
    glCallList(background);

    glDisable(GL_BLEND);
    glEnable( GL_DEPTH_TEST );

    OrthogonalEnd();
}
