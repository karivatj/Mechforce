#ifndef ENTITY_H
#define ENTITY_H

/*Abstract class to create Entity type objects*/ 
class Entity {

public:
    virtual ~Entity() {}
    virtual void draw() = 0;
private:
    VertexVector vertices_;
};


#endif
