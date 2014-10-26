#ifndef MAP_H
#define MAP_H

#include <vector>

#include "SDL2/SDL.h"

#include "cRenderer.h"
#include "common.h"

class Renderer;

class Map
{
public:

    Map();

    Map(int w, int h);

    void generateMap();

    int getMapWidth() { return width_; }

    int getMapHeight() { return heigth_; }

    std::vector<Vector3> getMapData() { return vertices_; }

    ~Map();

    int width_;

    int heigth_;

private:

    std::vector<Vector3> vertices_;

    float getValueAt(int x, int y, Vector3 *verts);

    void setValue(int x, int y, float value, Vector3 *verts);

    void sampleSquare(int x, int y, int size, float value, Vector3 *verts);

    void sampleDiamond(int x, int y, int size, float value, Vector3 *verts);
};

#endif //Map_H
