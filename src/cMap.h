#ifndef MAP_H
#define MAP_H

#include <vector>

#include "SDL2/SDL.h"

#include "cEntity.h"
#include "cRenderer.h"
#include "common.h"

class Entity;
class Renderer;

class Map : public Entity
{
public:

    Map();

    Map(int w, int h);

    ~Map();

    void draw();

    void generateMap();

    int getMapWidth() { return width_; }

    int getMapHeight() { return heigth_; }

    VertexVector getMapData() { return vertices_; }

    int width_;

    int heigth_;

private:

    float getValueAt(int x, int y, Vector3 *verts);

    void setValue(int x, int y, float value, Vector3 *verts);

    void sampleSquare(int x, int y, int size, float value, Vector3 *verts);

    void sampleDiamond(int x, int y, int size, float value, Vector3 *verts);
};

#endif //Map_H
