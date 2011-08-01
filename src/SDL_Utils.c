#include "SDL_Engine.h"
#include "Mechforce.h"
#include "Button.h"

int Utils_ScreenShot(void)
{
#if 0
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

void Utils_CountFPS(void)
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

STATE Utils_ResolveState(int id)
{
    switch(id)
    {
        case    0:  return STATE_QUIT;
        case    1:  return STATE_MAINMENU;
        case    2:  return STATE_GAME;
        case    3:  return STATE_OPTIONS;
        case    -1: return STATE_NULL;
        default:    return STATE_NULL;
    }
}

void Utils_WriteConfigFile(void)
{
    FILE *file = NULL;
    int i;

    printf("Writing configuration to file.\n");

    if((file = fopen ("../Data/config.ini", "w")) == NULL)
    {
        fprintf(stderr,"ERROR*** Couldn't create config file for writing (./Data/config.ini)\n");
        SDL_Close(-1);
    }

    for(i = 0; i < MAX_BUTTONS; i++)
    {
        if(Buttons[i].state == STATE_OPTIONS)
        {
            if(strcmp(Buttons[i].caption, "Fullscreen") == 0)
                fprintf(file, "Fullscreen=%d", Buttons[i].enabled);

        }
    }
    fclose(file);
}

void Utils_ReadConfigFile(void)
{
    FILE *file;
    char string[64];
    char *token = NULL;

    printf("Reading configuration from file.\n");

    if((file = fopen ("../Data/config.ini", "r")) == NULL)
    {
        fprintf(stderr,"ERROR*** Couldn't open config file! (./Data/config.ini)\n");
        return;
    }

    while(!feof(file))
    {
        fgets(string,64,file);
        token = strtok(string,"=");

        while(token != NULL)
        {
            //if(strcmp(string, "Fullscreen") == 0)
            printf("%s\n",token);
            token = strtok (NULL, "=");
        }
    }

    fclose (file);
}
