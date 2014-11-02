#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SDL2/SDL.h>

#include "cGame.h"

class Map : public Entity
{
public:

    Map();

    ~Map();

    void initialize();

    void draw();

    void generateMap();

    int getMapWidth()           { return width_; }

    int getMapHeight()          { return heigth_; }

    VertexVector getMapData()   { return vertices_; }

    int width_;

    int heigth_;

private:

    float getValueAt(int x, int y, Vector3 *verts);

    void setValue(int x, int y, float value, Vector3 *verts);

    void sampleSquare(int x, int y, int size, float value, Vector3 *verts);

    void sampleDiamond(int x, int y, int size, float value, Vector3 *verts);
};

#endif //Map_H
