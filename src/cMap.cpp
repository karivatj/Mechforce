#include <iostream>
#include <stdlib.h>

#include "cMap.h"

Map::Map()
{
    width_ = 32;
    heigth_ = 32;
    initialize();
}

Map::~Map()
{
    std::cout << "Map: Destroyed" << std::endl;
}
void Map::initialize()
{
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };
    generateMap();
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    //glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3)*vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
}

void Map::draw()
{
    std::cout << "Map Here" << std::endl;
    glUseProgram(Renderer::getInstance()->getShaderProgram());
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

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
}

float Map::getValueAt(int x, int y, Vector3 *verts)
{
    return verts[(x & (width_ - 1)) + (y & (heigth_ - 1)) * width_].y;
}

void Map::setValue(int x, int y, float value, Vector3 *verts)
{
    verts[(x & (width_ - 1)) + (y & (heigth_ - 1)) * width_].y = value;
}

void Map::sampleSquare(int x, int y, int size, float value, Vector3 *verts)
{
    int hs = size / 2;

    float a = getValueAt(x - hs, y - hs, verts);
    float b = getValueAt(x + hs, y - hs, verts);
    float c = getValueAt(x - hs, y + hs, verts);
    float d = getValueAt(x + hs, y + hs, verts);

    setValue(x, y, ((a + b + c + d) / 4.0) + value, verts);
}

void Map::sampleDiamond(int x, int y, int size, float value, Vector3 *verts)
{
    int hs = size / 2;

    float a = getValueAt(x - hs, y, verts);
    float b = getValueAt(x + hs, y, verts);
    float c = getValueAt(x, y - hs, verts);
    float d = getValueAt(x, y + hs, verts);

    setValue(x, y, ((a + b + c + d) / 4.0) + value, verts);
}

void Map::generateMap()
{
    Vector3 *verts = new Vector3[width_ * heigth_];

    memset(verts, 0, sizeof(Vector3) * width_ * heigth_);

    int stepsize = 32;

    float scale = 1;

    /*Generate map using Diamond-Square algorithm*/
    while (stepsize > 1)
    {
        int halfstep = stepsize / 2;

        for (int y = halfstep; y < heigth_ + halfstep; y += stepsize)
        {
            for (int x = halfstep; x < width_+ halfstep; x += stepsize)
            {
                sampleSquare(x, y, stepsize, (double)rand()/(double)RAND_MAX * 2 * scale, verts);
            }
        }

        for (int y = 0; y < heigth_; y += stepsize)
        {
            for (int x = 0; x < width_; x += stepsize)
            {
                sampleDiamond(x + halfstep, y, stepsize, (double)rand()/(double)RAND_MAX * 2 * scale, verts);
                sampleDiamond(x, y + halfstep, stepsize, (double)rand()/(double)RAND_MAX * 2 * scale, verts);
            }
        }

        stepsize /= 2;
        scale /= 2.0;
    }

    /*Populate vertex vector*/
    for(int i = 0; i < width_ * heigth_; i++)
    {
        vertices_.push_back(Vector3(verts[i].x, verts[i].y, verts[i].z));
    }

    for(int i = 0; i < width_; i++)
    {
        for(int j = 0; j < heigth_; j++)
        {
            vertices_.push_back(Vector3(verts[i + width_ * j].x, verts[i + width_ * j].y, verts[i + width_ * j].z));
        }
    }

    delete[] verts;
}
