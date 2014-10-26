#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>

#include "cRenderer.h"
#include "common.h"

bool Renderer::instanceFlag_ = false;
Renderer* Renderer::thisPointer_ = NULL;

Renderer* Renderer::getInstance()
{
    if(instanceFlag_)
    {
        return thisPointer_;
    }
    else
    {
        thisPointer_ = new Renderer();
        instanceFlag_ = true;
        return thisPointer_;
    }
}

Renderer::Renderer() : screen_width_(800), screen_height_(600)
{
    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stderr, "Error occured while initializing SDL: %s", SDL_GetError());
        SDL_Quit();
    }

    // set the opengl context version
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); //Disable deprecated function calls

    if(SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE, &screen_, &renderer_) != 0)
    {
        fprintf(stderr, "Error occured while creating window and renderer: %s", SDL_GetError());
        SDL_Quit();
    }

    if((glcontext_ = SDL_GL_CreateContext(screen_)) == NULL)
    {
        fprintf(stderr, "Error occured while creating GL context: %s", SDL_GetError());
        SDL_Quit();
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");   //Make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(renderer_, 800, 600);           //Make resolution of the screen device independent

    //Loading of window icon
    if((window_icon_ = SDL_LoadBMP("./Images/icon.bmp")) == NULL)
    {
        fprintf(stderr, "Error occured while loading window icon: %s", SDL_GetError());
    }

    /*Finally set window title and icon*/
    SDL_SetWindowTitle(screen_, "Mechforce v.0.0");
    SDL_SetWindowIcon(screen_, window_icon_);
}

Renderer::~Renderer()
{
    SDL_FreeSurface(window_icon_);
    window_icon_ = NULL;

    SDL_GL_DeleteContext(glcontext_);
    SDL_DestroyWindow(screen_);
    std::cout << "Renderer: Destroyed" << std::endl;
}

SDL_Renderer* Renderer::getRenderer()
{
    return renderer_;
}

void Renderer::setOwner(Game *g)
{
    owner_ = g;
}

void Renderer::enable3D()
{
    int w, h;

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    SDL_GetWindowSize(this->screen_, &w, &h);

    glViewport(0, 0, w, h);
    gluPerspective (45, (GLfloat)w / (GLfloat)h, 0.1, 2000.0);
}

void Renderer::enable2D()
{

}

void Renderer::update(float frametime)
{
    AssetManager    *am     = owner_->getAssetManager();
    Map             *map    = am->getMap();

    std::vector<Vector3> map_data = map->getMapData();
    std::vector<GLuint>  tile_textures = am->getTileMap();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    enable3D();

    glTranslatef(-map->getMapWidth() / 2 + camX, map->getMapHeight() / 2 + camY, camZ);

    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    //glColor4f(0,0,0,0.5);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_LINE_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    //SDL_Texture* texture = tile_textures.front();
    //float texw = 0;
    //float texh = 0;

    std::vector<GLuint>::iterator it;
    int counter = 0;
    int line = 0;
    for(it = tile_textures.begin(); it < tile_textures.end(); ++it)
    {
        if(counter % 4 == 0)
        {
            counter = 0;
            line ++;
        }

        glBindTexture(GL_TEXTURE_2D, (*it));
        glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(0+(counter * 5),0,0+(line * 5));
        glTexCoord2f(1,0); glVertex3f(5+(counter * 5),0,0+(line * 5));
        glTexCoord2f(1,1); glVertex3f(5+(counter * 5),0,5+(line * 5));
        glTexCoord2f(0,1); glVertex3f(0+(counter * 5),0,5+(line * 5));
        glEnd();
        counter++;
    }

/*
    glEnableClientState(GL_VERTEX_ARRAY);

    for(int i = 0; i < map->getMapHeight(); i++)
    {
        glVertexPointer(3, GL_FLOAT, 0, &map_data[0]);
        glDrawArrays(GL_LINE_STRIP, i * map->getMapHeight(), map->getMapWidth());
        glVertexPointer(3, GL_FLOAT, 0, &map_data[map->getMapWidth() * map->getMapHeight()]);
        glDrawArrays(GL_LINE_STRIP, i * map->getMapHeight() , map->getMapHeight());
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
*/
    SDL_GL_SwapWindow(screen_);  //Refresh the screen
}
