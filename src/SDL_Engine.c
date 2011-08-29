#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "Mechforce.h"
#include "SDL_Engine.h"
#include "SDL_Utils.h"
#include "Map.h"
#include "Button.h"

STATE state;

int main(int argc, char **argv)
{
    int done = 0, value = 0;
    orthogonalEnabled = 1;

    SCREEN_WIDTH = 800;
    SCREEN_HEIGHT = 600;

    srand (time(NULL));   //Initialize random seed

    MAP_GenerateMap();

    camerax = 0;
    cameray = 15;
    cameraz = -440;

    rotx = 45;
    roty = 0;

    Init_SDL();
    Init_GL();

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    SDL_LoadTextures();
    SDL_GenerateTilemap();
    BTN_ReadButtonData();
    SDL_LoadSounds();

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

void Init_SDL(void)
{
    const SDL_VideoInfo *videoInfo;

    SDL_Surface* icon = SDL_LoadBMP("./Images/icon.bmp");

    printf("Initializing SDL.\n");

    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stderr, "Error occured while initializing SDL: %s", SDL_GetError());
        SDL_Close(1);
    }

    videoInfo = SDL_GetVideoInfo();

    if (!videoInfo)
    {
        fprintf( stderr, "Video query failed: %s\n", SDL_GetError( ) );
        SDL_Close(1);
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
    {
        fprintf(stderr, "Error encountered while initializing SDL_mixer: %s\n", Mix_GetError());
        SDL_Close(1);
    }

    SDL_WM_SetCaption("Mechforce","Mechforce");
    SDL_WM_SetIcon(icon, NULL);
    SDL_FreeSurface(icon);

    flags = SDL_OPENGL;
    flags |= SDL_RESIZABLE;

    if (videoInfo->hw_available )
	flags |= SDL_HWSURFACE;
    else
	flags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if (videoInfo->blit_hw )
	flags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /* get a SDL surface */
    if ((screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags )) == NULL )
    {
        fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
        SDL_Close(1);
    }
}

void Init_GL()	        // We call this right after our OpenGL window is created.
{
    printf("Initializing OpenGL.\n\n");

    glShadeModel(GL_SMOOTH);			    // Enables Smooth Color Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);           // This Will Clear The Background Color To Black
    glClearDepth(1.0);                              // Enables Clearing Of The Depth Buffer
    glEnable(GL_DEPTH_TEST);			    // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    SDL_BuildDisplayLists();

    glEnable(GL_TEXTURE_2D);
}

void SDL_Close(int code)
{
    int i;

    printf("\nShutting down.\n");

    for(i = 0;i < MAX_SOUNDS; i++)
        Mix_FreeChunk(sounds[i]);

    printf("Soundfiles freed.\n");

    SDL_FreeSurface(screen);

    Mix_CloseAudio();

    printf("Audio closed.\n");

    if(font[0] != NULL && font[1] != NULL)
    {
        ftglDestroyFont(font[0]);
        ftglDestroyFont(font[1]);
        printf("Fonts destroyed.\n");
    }

    glDeleteTextures(3,&backgrounds[0]);
    glDeleteTextures(11,&buttontextures[0]);
    glDeleteTextures(MAX_TILES,&tiletexture[0]);

    printf("Textures deleted.\n");

    glDeleteLists(background,1);
    glDeleteLists(state,1);

    printf("Shutting down SDL.\n");

    SDL_QuitSubSystem(SDL_INIT_AUDIO|SDL_INIT_VIDEO);

    printf("\nAll Done. Thank You for trying MF - Rearmed!\nExited with code %d\n", code);

    SDL_Quit();

    exit(code);
}

void SDL_DrawScene(void)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    glEnable3D();

    MF_StateMachine();

    OrthogonalStart();

    BTN_DrawButtonScene();
    //MAP_Draw2DTerrain();
    SDL_DrawText(25,760,570,1,1,0,0,"%.0f",fps);

    for (i=0; i<MAX_TXT_EVENTS; i++)
    {
            if (text_events[i].reserved == 0)
                break;

            Print(text_events[i].size, text_events[i].x, text_events[i].y, text_events[i].r,
                  text_events[i].g, text_events[i].b, text_events[i].font, text_events[i].text);

            text_events[i].reserved = 0;
    }

    OrthogonalEnd();

}

void SDL_DrawHUD(void)
{
    OrthogonalStart();

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST );

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

void SDL_DrawTile(int index, int x, int y)
{
    glLoadIdentity();
    glTranslated(x,y,0);
    glBindTexture(GL_TEXTURE_2D, tiletexture[index]);
    glCallList(state);
}

/*Build displaylists*/
void SDL_BuildDisplayLists(void)
{
    /*Buttons for mainmenu*/
    background = glGenLists(1);
    glNewList(background,GL_COMPILE);
    glBegin(GL_QUADS);
        glTexCoord2d(0,1);        glVertex2d(0 ,    0);
        glTexCoord2d(1,1);        glVertex2d(SCREEN_WIDTH, 0);
        glTexCoord2d(1,0);        glVertex2d(SCREEN_WIDTH, SCREEN_HEIGHT);
        glTexCoord2d(0,0);        glVertex2d(0 ,    SCREEN_HEIGHT);
    glEnd();
    glEndList();

    /*Tile Displaylist*/
    tile = glGenLists(1);
    glNewList(tile, GL_COMPILE);
    glBegin(GL_QUADS);
        glTexCoord2d(0,1);        glVertex2d(0 , 0);
        glTexCoord2d(1,1);        glVertex2d(32, 0);
        glTexCoord2d(1,0);        glVertex2d(32, 32);
        glTexCoord2d(0,0);        glVertex2d(0 , 32);
    glEnd();
    glTranslated(32,0,0);
    glEndList();
}

void glEnable3D()
{
    glViewport (0, 0, (GLsizei)SCREEN_WIDTH, (GLsizei)SCREEN_HEIGHT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    gluPerspective (45, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1, 1000.0);

    glTranslatef(camerax, cameray, cameraz);

    glRotatef(rotx, 1, 0, 0);
    glRotatef(roty, 0, 1, 0);

    glMatrixMode (GL_MODELVIEW);

    glLoadIdentity();
}

void OrthogonalStart()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void OrthogonalEnd()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
