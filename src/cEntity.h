#ifndef ENTITY_H
#define ENTITY_H

#include "cGame.h"

/*Interface class to create Entity type objects*/
class Entity
{

public:
    virtual ~Entity() {}
    virtual void initialize() = 0;  //Function where necessary stuff is initialized
    virtual void draw() = 0;        //Function where the entity is drawn.

protected:
    VertexVector vertices_;
    GLuint  vbo_;
};


#endif
