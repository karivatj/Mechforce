#ifndef CRENDERER_H
#define CRENDERER_H

#include <GL/gl.h>
#include "SDL2/SDL.h"

#include "cGame.h"
#include "cFPSCamera.h"

class Game;
class FPSCamera;

class Renderer
{
public:  

    ~Renderer();

    static Renderer* getInstance();

    void update(float frametime);

    SDL_Renderer* getRenderer();

    void setOwner(Game *g);

    void enable3D();

    void enable2D();

private:

    Renderer();

    GLuint          loadShaders();

    float           frametime_;

    const int       screen_width_;

    const int       screen_height_;

    static bool     instanceFlag_;

    static Renderer *thisPointer_;

    SDL_Window      *screen_;

    SDL_Renderer    *renderer_;

    SDL_GLContext   glcontext_;

    GLuint          shaderProgramID_;

    GLuint          vertexArrayID_;

    //Icon for the window
    SDL_Surface     *window_icon_;

    Game            *owner_;

    FPSCamera       *camera;
};

#endif // CRENDERER_H
