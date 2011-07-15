
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
#include "Map.h"
#include "Button.h"

STATE state;

int main(int argc, char **argv)
{
    int done = 0, value = 0;
    srand (time(NULL));   //Initialize rand seed

    MAP_GenerateMap();

    camerax = 0;
    cameray = 15;
    cameraz = 25;
    rotx = 45;
    roty = 0;

    Init_SDL();
    Init_GL(WIDTH,HEIGHT);

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    SDL_LoadTextures();
    SDL_GenerateTilemap();
    BTN_ReadButtonData();
    SDL_LoadSounds();

    printf("Loading fonts.\nLoaded following fonts succesfully:\n");
    if((font[0] = ftglCreatePixmapFont("../Fonts/kimberle.ttf")) == NULL) {
    printf("ERROR*** Failed to load ../Fonts/kimberle.ttf.\n");
    SDL_Close(-1);
    }

    printf("\t../Fonts/kimberle.ttf OK\n");
    ftglSetFontCharMap(font[0], ft_encoding_unicode);

    if((font[1] = ftglCreatePixmapFont("../Fonts/cour.ttf")) == NULL) {
    printf("ERROR*** Failed to load ../Fonts/cour.ttf.\n");
    SDL_Close(-1);
    }

    printf("\t../Fonts/cour.ttf OK\n\n");
    ftglSetFontCharMap(font[1], ft_encoding_unicode);

    state = STATE_MAINMENU;

    while(!done)			//Pää looppi missä kaikki taika tapahtuu
    {
        framestart = SDL_GetTicks();

        value = MF_Event_Handler();     //Käydään joka framella kattomassa onko eventtejä jonossa (hiiren klikkaukset, näppäimistö yms.)

        SDL_DrawScene();

        SDL_GL_SwapBuffers();

        SDL_CountFPS();

        if(value == 1)
        done = 1;
    }

    SDL_Close(0);
    return(0);
}

void Init_SDL(void)
{
    const SDL_VideoInfo *videoInfo;

    SDL_Surface* icon = SDL_LoadBMP("./Images/icon.bmp");	        //Tehdään ikkunalle oma pieni ikoni. Ladataan eka kuva

    printf("Initializing SDL.\n");

    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) != 0 ) 				//Alustetaa SDL käyttöön Videon ja Audion osalta
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

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0) 		//Avataan mixeri käyttöön perus asetuksilla.
    {
        fprintf(stderr, "Error encountered while initializing SDL_mixer: %s\n", Mix_GetError());
        SDL_Close(1);
    }

    SDL_WM_SetCaption("Mechforce: Rearmed","MechForce: Rearmed");	//Nimetään vielä ikkuna mieleiseksemme
    SDL_WM_SetIcon(icon, NULL);					                    //Ja asetetaan ikoni tällä käskyllä ikkunalle
    SDL_FreeSurface(icon);						                    //Lopuksi vielä vapautetaan kuva ettei se jää muistiin

    flags = SDL_OPENGL;

    if ( videoInfo->hw_available )
	flags |= SDL_HWSURFACE;
    else
	flags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
	flags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /* get a SDL surface */
    if ((screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, flags )) == NULL )
    {
        fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
        SDL_Close(1);
    }
}

void Init_GL(int Width, int Height)	        // We call this right after our OpenGL window is created.
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

    ftglDestroyFont(font[0]);
    ftglDestroyFont(font[1]);

    printf("Fonts destroyed.\n");

    glDeleteTextures(3,&backgrounds[0]);
    glDeleteTextures(11,&buttontextures[0]);
    glDeleteTextures(MAX_TILES,&tiletexture[0]);

    printf("Textures deleted.\n");

    glDeleteLists(background,1);
    glDeleteLists(tile,1);

    printf("Shutting down SDL.\n");

    SDL_QuitSubSystem(SDL_INIT_AUDIO|SDL_INIT_VIDEO);

    printf("\nAll Done. Thank You for trying MF - Rearmed!. Bye Bye.\n");

    SDL_Quit();

    exit(code);
}

void SDL_DrawScene(void)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glEnable3D();

    MF_StateMachine();  //Tilakone
    OrthogonalStart();
    BTN_DrawButtonScene();

    SDL_DrawText(25,760,570,1,1,0,0,"%.0f",fps);

    for (i=0; i<MAX_TXT_EVENTS; i++)
    {
            if (text_events[i].reserved == 0)
                break;

            Print(text_events[i].size, text_events[i].x, text_events[i].y, text_events[i].r, text_events[i].g, text_events[i].b, text_events[i].font, text_events[i].text);
            text_events[i].reserved = 0;
    }

    OrthogonalEnd();

}

void SDL_DrawTile(int index, int x, int y)   //Piirretään vain tile ruutuun.
{
    glLoadIdentity();
    glTranslated(x,y,0);
    glBindTexture(GL_TEXTURE_2D, tiletexture[index]);
    glCallList(tile);
}

void SDL_BuildDisplayLists (void)
{
    /*Buttons for mainmenu*/
    background = glGenLists(1);
    glNewList(background,GL_COMPILE);
    glBegin(GL_QUADS);
        glTexCoord2d(0,1);        glVertex2d(0 ,    0);
        glTexCoord2d(1,1);        glVertex2d(WIDTH, 0);
        glTexCoord2d(1,0);        glVertex2d(WIDTH, HEIGHT);
        glTexCoord2d(0,0);        glVertex2d(0 ,    HEIGHT);
    glEnd();
    glEndList();

    /*Tile Displaylist*/
    tile = glGenLists(1);
    glNewList(tile,GL_COMPILE);
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
    glViewport (0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    gluPerspective (45, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1, 1000.0);
/*
    glTranslatef(camerax,cameray,-cameraz);     //Move Camera

    glRotatef(rotx,1,0,0);
    glRotatef(roty,0,1,0);
*/
    gluLookAt(camerax, cameray, cameraz,0,0,-10,0,1,0);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}

void OrthogonalStart()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0, WIDTH, 0, HEIGHT);

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
