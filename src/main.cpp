#ifdef WIN32
#include <windows.h>
#endif

#include<iostream>
#include<stdlib.h>
#include<time.h>

#include "cGame.h"


bool appRunning;
float camX, camY, camZ;
float rotX, rotY, rotZ;

int main(int argc, char **argv)
{
    camX = 0;
    camY = 0;
    camZ = -45;
    rotX = 90;
    rotY = 0;

    srand(time(NULL));

    Game *mechforce = new Game();

    mechforce->Start();

    delete mechforce;
    SDL_Quit();
    return(0);
}
