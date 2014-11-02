#ifndef COMMON_H
#define COMMON_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>

class Entity;
class Renderer;
class AssetManager;
class EventHandler;
class World;
class Game;
class Map;

extern bool appRunning;
extern float camX, camY, camZ;
extern float rotX, rotY, rotZ;

struct Vector3
{
    Vector3() {}
    Vector3 (float new_x, float new_y, float new_z)
    {
        x = new_x;
        y = new_y;
        z = new_z;
    }

    //Operator overloads
    Vector3 operator+(Vector3 v)      {return Vector3(v.x+x, v.y+y, v.z+z);}
    Vector3 operator-(Vector3 v)      {return Vector3(x-v.x, y-v.y, z-v.z);}
    Vector3 operator*(float number)    {return Vector3(x*number, y*number, z*number);}
    Vector3 operator/(float number)    {return Vector3(x/number, y/number, z/number);}
    Vector3 operator=(float number)    {return Vector3(x=number, y=number, z=number);}

    float x, y, z;

};

//Enum classes to differentiate between shaders and entities
enum class VertexShaderType { DEFAULT };
enum class FragmentShaderType { DEFAULT };
enum class EntityType { MAP };

typedef std::vector<Entity*> EntityVector;
typedef std::vector<Entity*>::iterator EntityIterator;
typedef std::vector<Vector3> VertexVector;

struct Surface
{
    int A;
    int B;
    int C;
};

struct Surface3D
{
    Surface surface;
    Vector3 normal;
};

#endif
