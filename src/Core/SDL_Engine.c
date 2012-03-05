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

#include "SDL_Engine.h"
#include "SDL_Textures.h"
#include "SDL_DrawText.h"
#include "../main.h"
#include "../MapRoutines.h"
#include "../EventHandler.h"
#include "../StateMachine.h"
#include "../Widget.h"
#include "../Sound/Sound.h"
#include "../Utils/Utilities.h"
#include "../Prefs/Preferences.h"

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


    flags = SDL_OPENGL;
    flags |= SDL_RESIZABLE;

    if (videoInfo->hw_available )
	flags |= SDL_HWSURFACE;
    else
	flags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if (videoInfo->blit_hw )
	flags |= SDL_HWACCEL;

    if(pref_fullscreen == 1)
    flags |= SDL_FULLSCREEN;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /* get a SDL surface */
    if ((screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags )) == NULL )
    {
        fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
        SDL_Close(1);
    }
    SDL_WM_SetCaption("Mechforce","Mechforce");
    SDL_WM_SetIcon(icon, NULL);

    //SDL_FreeSurface(icon);
}

void Init_GL()	        // We call this right after our OpenGL window is created.
{
    GLfloat specular[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat global_ambient[]=  { 0.5f, 0.5f, 0.5f, 1.0f };
    printf("Initializing OpenGL.\n\n");

    glShadeModel(GL_SMOOTH);			    // Enables Smooth Color Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);           // This Will Clear The Background Color To Black
    glClearDepth(1.0);                              // Enables Clearing Of The Depth Buffer
    glEnable(GL_DEPTH_TEST);			    // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do

    //glEnable(GL_LIGHTING); //Enable lights
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    SDL_BuildDisplayLists();

    glEnable(GL_TEXTURE_2D);
}

void glEnable3D()
{
    glViewport (0, 0, (GLsizei)SCREEN_WIDTH, (GLsizei)SCREEN_HEIGHT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    gluPerspective (45, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1, 1000.0);

    glMatrixMode (GL_MODELVIEW);

    glLoadIdentity();

    //glTranslatef(0, 15, -440);

    glTranslatef(camerax, cameray, cameraz);

    glRotatef(rotx, 1, 0, 0);
    glRotatef(roty, 0, 1, 0);

    //glTranslatef(-camerax, -cameray, -cameraz);
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


void SDL_DrawScene(void)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

    glEnable3D();

    MF_StateMachine();

    OrthogonalStart(); //Start rendering in 2D

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
    glBegin(GL_QUADS);
        glTexCoord2d(0,1);        glVertex2d(0 ,    0);
        glTexCoord2d(1,1);        glVertex2d(SCREEN_WIDTH, 0);
        glTexCoord2d(1,0);        glVertex2d(SCREEN_WIDTH, SCREEN_HEIGHT);
        glTexCoord2d(0,0);        glVertex2d(0 ,    SCREEN_HEIGHT);
    glEnd();
    //glCallList(background);

    glLoadIdentity();

    glBlendFunc( GL_ONE, GL_ONE );
    glBindTexture(GL_TEXTURE_2D,backgrounds[2]);        //HUD

    glBegin(GL_QUADS);
        glTexCoord2d(0,1);        glVertex2d(0 ,    0);
        glTexCoord2d(1,1);        glVertex2d(SCREEN_WIDTH, 0);
        glTexCoord2d(1,0);        glVertex2d(SCREEN_WIDTH, SCREEN_HEIGHT);
        glTexCoord2d(0,0);        glVertex2d(0 ,    SCREEN_HEIGHT);
    glEnd();

    //glCallList(background);

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

void SDL_Close(int code)
{
    printf("\nShutting down...\n");

    Sound_Cleanup();
    Font_Cleanup();
    Texture_Cleanup();
    Map_Cleanup();

    printf("Shutting down SDL...\n");

    SDL_QuitSubSystem(SDL_INIT_AUDIO|SDL_INIT_VIDEO);

    printf("All Done. Thank You for trying MF - Rearmed!\nExited with code %d\n", code);

    SDL_Quit();
    exit(code);
}
