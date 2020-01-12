#pragma once

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef struct
{
    double h;       /*Height of the point*/
    int type;       /*Type eg. forest, mountain, water, plain. Maybe used when drawing the map in 2D*/
}MAP;

struct Vector
{
    Vector() {}
    Vector (float new_x, float new_y, float new_z)
    {
        x = new_x;
        y = new_y;
        z = new_z;
    }
    Vector(const Vector &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    //Operator overloads
    Vector operator+(Vector v)        {return Vector(v.x + x, v.y + y, v.z + z);}
    Vector operator-(Vector v)        {return Vector(x - v.x, y - v.y, z - v.z);}
    Vector operator*(float number)	  {return Vector(x*number, y*number, z*number);}
    Vector operator/(float number)	  {return Vector(x/number, y/number, z/number);}
    Vector operator=(Vector v)        {return Vector(v.x, v.y, v.z);}

    float x, y, z;

};

struct Triangle
{
    Triangle() {}
    Triangle (Vector new_x, Vector new_y, Vector new_z)
    {
        a = new_x;
        b = new_y;
        c = new_z;
    }

    //Operator overloads
    Triangle operator+(Triangle t)      {return Triangle(t.a + a, t.b + b, t.c + c);}
    Triangle operator-(Triangle t)      {return Triangle(a - t.a, b - t.b, c - t.c);}
    Triangle operator*(float number)    {return Triangle(a*number, b*number, c*number);}
    Triangle operator/(float number)    {return Triangle(a/number, b/number, c/number);}

    Vector a, b, c;
};

typedef struct {
    Triangle t[6];
} Hexagon;

typedef struct {
    Vector v;
} Vertexarray ;

typedef struct{
    Vector c;
} Colorarray;

#endif
