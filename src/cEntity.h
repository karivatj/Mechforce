#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"

/*Interface class to create Entity type objects*/
class Entity
{

public:
    virtual ~Entity() {}
    virtual void draw() = 0;

protected:
    VertexVector vertices_;
};


#endif
