#ifndef COMMON_H
#define COMMON_H

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
