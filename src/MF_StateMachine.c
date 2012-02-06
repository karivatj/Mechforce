#include "Mechforce.h"
#include "Core/SDL_Engine.h"
#include "Core/SDL_Textures.h"
#include "Utils/Utilities.h"
#include "Prefs/Preferences.h"
#include "MF_MapRoutines.h"


void MF_StateMachine(void)
{
    switch(state)
    {
        case STATE_QUIT:
            SDL_Close(0);
        break;

        case STATE_MAINMENU:
            OrthogonalStart();

            /* Draw the Backgroung for this state */
            glBindTexture(GL_TEXTURE_2D,backgrounds[0]);
            glBegin(GL_QUADS);
                glTexCoord2d(0,1);        glVertex2d(0 ,    0);
                glTexCoord2d(1,1);        glVertex2d(SCREEN_WIDTH, 0);
                glTexCoord2d(1,0);        glVertex2d(SCREEN_WIDTH, SCREEN_HEIGHT);
                glTexCoord2d(0,0);        glVertex2d(0 ,    SCREEN_HEIGHT);
            glEnd();

            //glCallList(background);
            OrthogonalEnd();
        break;

        case STATE_OPTIONS:
            OrthogonalStart();

            /* Draw the Backgroung for this state */
            glBindTexture(GL_TEXTURE_2D,backgrounds[1]);
            glBegin(GL_QUADS);
                glTexCoord2d(0,1);        glVertex2d(0 ,    0);
                glTexCoord2d(1,1);        glVertex2d(SCREEN_WIDTH, 0);
                glTexCoord2d(1,0);        glVertex2d(SCREEN_WIDTH, SCREEN_HEIGHT);
                glTexCoord2d(0,0);        glVertex2d(0 ,    SCREEN_HEIGHT);
            glEnd();

            //glCallList(background);
            OrthogonalEnd();
        break;

        case STATE_GAME:
            MAP_Draw3DTerrain();
            //MAP_LoadMapFromFile("MAP1");
            SDL_DrawHUD();
            //MAP_Draw2DTerrain();
        break;

        case STATE_RELOAD_CONFIG:
            Utils_ReadConfigFile();

	        if(pref_fullscreen != fullscreen_enabled)
            {
                #ifdef WIN32
                SDL_Toggle_Fullscreen();
                #endif
                #ifdef unix
                SDL_WM_ToggleFullScreen(screen);
                #endif
                fullscreen_enabled = pref_fullscreen;
            }

            state = STATE_MAINMENU;
        break;

        default:
        break;
    }
}
