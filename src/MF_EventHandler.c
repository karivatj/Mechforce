#include "Mechforce.h"
#include "Map.h"
#include "SDL_Engine.h"

int MF_Event_Handler(void)
{
     while(SDL_PollEvent(&event))
     {
        switch(event.type)
        {
            case SDL_QUIT:
                    return (1);
            break;

            case SDL_KEYDOWN:	//If a button was pressed

                    switch(event.key.keysym.sym)	//What was the button pressed?
                    {
                        case SDLK_ESCAPE:
                                return (1);
                                break;
                        case SDLK_F1:
                            return (4);
                            break;
                        case SDLK_4:
                            MAP_GenerateMap();
                            //MAP_LoadMap("KARTTA3");
                            break;
                        default:
                        break;
                    }
                            break;

                    case SDL_MOUSEBUTTONDOWN:
                          MF_MouseEventPress(event.type, event.button.button, event.button.state, event.button.x, event.button.y);
                    break;

                    case SDL_MOUSEBUTTONUP:
                        MF_MouseEventRelease(event.type, event.button.button, event.button.state, event.button.x, event.button.y);
                    break;

                    case SDL_MOUSEMOTION:
                        MF_MouseEventMotion(event.type, event.button.button, event.button.state, event.motion.x, event.motion.y);
                    break;

                    default:
                    break;
        }
    }

    return(0);
}

