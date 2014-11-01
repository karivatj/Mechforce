#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>
#include <SDL2/SDL.h>

#include <map>

#include "common.h"
#include "cGame.h"
#include "cWorld.h"
#include "cAssetManager.h"

class World;

class Renderer
{
public:  

    ~Renderer();

    static Renderer* getInstance();

    void update(float frametime);

    void setOwner(Game *g);

    void setWorld(World *w);

    void enable3D();

    void enable2D();

    SDL_Renderer* getRenderer();

private:

    Renderer();

    GLuint          loadShaders();

    GLuint          shaderProgramID_;

    GLuint          vertexArrayID_;

    SDL_Window      *screen_;

    SDL_Renderer    *renderer_;

    SDL_GLContext   glcontext_;

    SDL_Surface     *window_icon_;

    Game            *owner_;

    World           *world_;

    float           frametime_;

    const int       screen_width_;

    const int       screen_height_;

    static bool     instanceFlag_;

    static Renderer *thisPointer_;

    ///TODO: Make this better. I guess key cant be of type V/FShaderTypeku
    std::map<VertexShaderType, GLuint> vertexShaders_;
    std::map<FragmentShaderType, GLuint> fragmentShaders_;
};

#endif // RENDERER_H
