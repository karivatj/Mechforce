#include <iostream>
#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>

#include <vector>

#include "cRenderer.h"

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

    // After we succesfully initialized SDL lets ask SDL for specific OpenGL version: 3.0
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    if(SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE, &screen_, &renderer_) != 0)
    {
        std::cout << "Error occured while creating window and renderer: " << SDL_GetError() << std::endl;;
        SDL_Quit();
    }

    if((glcontext_ = SDL_GL_CreateContext(screen_)) == NULL)
    {
        std::cout << "Error occured while creating GL context: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW Init Error!" << std::endl;
        SDL_Quit();
    }

    if((window_icon_ = SDL_LoadBMP("../Images/icon.bmp")) == NULL)
    {
        std::cout << "Error occured while loading window icon: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

    //Load and compile shaders
    shaderProgramID_ = loadShaders();

    //Create the VAO object
    glGenVertexArrays(1, &vertexArrayID_);
    glBindVertexArray(vertexArrayID_);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");   //Make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(renderer_, 800, 600);          //Make resolution of the screen device independent

    //Finally set window title and icon
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

void Renderer::setWorld(World *w)
{
    world_ = w;
}

void Renderer::enable3D()
{
///TODO: Add proper implementation for viewport etc.
#if 1
    int w, h;

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    SDL_GetWindowSize(this->screen_, &w, &h);

    glViewport(0, 0, w, h);
    gluPerspective (45, (GLfloat)w / (GLfloat)h, 0.1, 2000.0);
#endif
}

void Renderer::enable2D()
{
///TODO: Add implementation for switching to orthographic view
}

GLuint Renderer::loadShaders()
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream("../Data/vshader.glsl", std::ios::in);

    if(VertexShaderStream.is_open())
    {
        std::string line = "";
        while(getline(VertexShaderStream, line))
            VertexShaderCode += "\n" + line;

        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream("../Data/fshader.glsl", std::ios::in);

    if(FragmentShaderStream.is_open())
    {
        std::string line = "";
        while(getline(FragmentShaderStream, line))
            FragmentShaderCode += "\n" + line;

        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    std::cout << "Renderer: Compiling vertex shader...";
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    std::cout << &VertexShaderErrorMessage[0] << std::endl;

    // Compile Fragment Shader
    std::cout << "Renderer: Compiling fragment shader...";
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    std::cout << &FragmentShaderErrorMessage[0];

    // Link the program
    std::cout << "Linking program..." << std::endl;
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    std::cout << &ProgramErrorMessage[0];

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    std::cout << "Shaders compiled and OK" << std::endl;

    return ProgramID;
}

void Renderer::update(float frametime)
{
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if 1
    AssetManager    *am     = owner_->getAssetManager();
    Map             *map    = am->getMap();

    VertexVector map_data = map->getMapData();
    std::vector<GLuint>  tile_textures = am->getTileMap();


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    enable3D();

    glTranslatef(-map->getMapWidth() / 2 + camX, map->getMapHeight() / 2 + camY, camZ);

    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
   // glColor4f(0,0,0,0.5);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

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
#endif
#if 0

    ///Move this to entity initialize
    // GL-code start for triangle
    glUseProgram(shaderProgramID_);

    ///TODO: This should be defined per entity basis. Atm here just for testing.
    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    //////////

    ///Move rest of this stuff to entity draw()
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
#endif
    SDL_GL_SwapWindow(screen_);  //Refresh the screen
}
