#include "Mechforce.h"
#include "SDL_Engine.h"
#include "SDL_Utils.h"
#include "Map.h"

void MF_StateMachine(void)
{
    switch(state)
    {
        case STATE_QUIT:
            SDL_Close(0);
        break;

        case STATE_MAINMENU:
            OrthogonalStart();
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
            MAP_Draw2DTerrain();
        break;

        case STATE_RELOAD_CONFIG:
            Utils_ReadConfigFile();
            state = STATE_MAINMENU;
        break;

        default:
        break;
    }
}
