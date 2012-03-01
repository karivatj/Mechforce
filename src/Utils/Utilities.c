#include <FreeImage.h>
#include "Utilities.h"
#include "../Core/SDL_Engine.h"
#include "../Prefs/Preferences.h"
#include "../main.h"
#include "../Widget.h"

int Utils_ScreenShot(void)
{
    unsigned char pixels[3 * SCREEN_WIDTH * SCREEN_HEIGHT]; //Array which holds the pixel data of the OpenGL screen

    glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_BGR, GL_UNSIGNED_BYTE, pixels);  //Read the pixels from the OpenGL buffer

    /* Convert the pixel data to FreeImage format */
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, SCREEN_WIDTH, SCREEN_HEIGHT, 3 * SCREEN_WIDTH, 24,
                                                   #if SDL_BYTEORDER == SDL_LIL_ENDIAN
                                                   0x000000FF, 0x0000FF00, 0x00FF0000, 0
                                                   #else
                                                   0x00FF0000, 0x0000FF00, 0x000000FF, 0
                                                   #endif
                                                   );

    /* Save the file in PNG format */
    FreeImage_Save(FIF_PNG, image, "../Screenshots/Screenshot.png", 0);

    free(image);    // Lets free our image data we dont need it anymore

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
                fprintf(file, "Fullscreen=%d\n", Buttons[i].enabled);

            if(strcmp(Buttons[i].caption, "Mute Sounds") == 0)
                fprintf(file, "Sound=%d\n", Buttons[i].enabled);
        }
    }

    fclose(file);
}

void Utils_ReadConfigFile(void)
{
    FILE *file;
    char string[64];
    char *token = NULL;
    int i;

    printf("Reading configuration from file.\n");

    if((file = fopen ("../Data/config.ini", "r")) == NULL)
    {
        fprintf(stderr,"ERROR*** Couldn't open config file! (./Data/config.ini)\n");
        return;
    }

    while(!feof(file))
    {
        if((fgets(string,64,file)) == NULL)
            break;

        token = strtok(string,"=");

        while(token != NULL)
        {
            if(strcmp(token, "Fullscreen") == 0)
            {
                token = strtok (NULL, "=");
                pref_fullscreen = atoi(token);
            }
            else if(strcmp(token, "Sound") == 0)
            {
                token = strtok (NULL, "=");
                pref_soundsoff = atoi(token);
            }
            else token = strtok(NULL, "=");
        }
    }

    for(i = 0; i < MAX_BUTTONS; i++)
    {
        if(Buttons[i].state == STATE_OPTIONS)
        {
            if(pref_fullscreen == 1 && (strcmp(Buttons[i].caption,"Fullscreen")) == 0)
                Buttons[i].enabled = TRUE;
            if(pref_soundsoff == 1 && (strcmp(Buttons[i].caption,"Mute Sounds")) == 0)
                Buttons[i].enabled = TRUE;
        }
    }

    fclose (file);
}
