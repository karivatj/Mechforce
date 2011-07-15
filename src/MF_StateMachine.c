#include "Mechforce.h"
#include "SDL_Engine.h"
#include "Map.h"

void MF_StateMachine (void) //Tilakone
{
    switch(state)
    {
        case STATE_QUIT:
        SDL_Close(0);
        break;

        case STATE_MAINMENU:
        OrthogonalStart();
        glBindTexture(GL_TEXTURE_2D,backgrounds[0]);
        glCallList(background);
        OrthogonalEnd();
        break;

        case STATE_GAME:
        glLoadIdentity();
        MAP_DrawFractalMap();
        //MAP_DrawTileMap();
        MF_DrawHUD();
        break;

        case STATE_OPTIONS:
        OrthogonalStart();
        glBindTexture(GL_TEXTURE_2D,backgrounds[1]);
        glCallList(background);
        OrthogonalEnd();
        break;

        default:
        break;
    }
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
