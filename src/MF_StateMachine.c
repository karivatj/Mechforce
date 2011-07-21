#include "Mechforce.h"
#include "SDL_Engine.h"
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
            glCallList(background);
            OrthogonalEnd();
        break;

        case STATE_GAME:
            glLoadIdentity();
            //MAP_DrawFractalMap();
            MAP_DrawTileMap();
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
