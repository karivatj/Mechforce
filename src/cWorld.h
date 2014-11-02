#ifndef WORLD_H
#define WORLD_H

#include "cGame.h"

class World
{
public:

    static World* getInstance();

    void update(float frametime);

    void setOwner(Game *g)      { owner_ = g; }

    EntityVector getEntities()  { return entities_; }

protected:

    World();

    ~World();

private:

    EntityVector    entities_;

    float           frametime_;

    static World    *thisPointer_;

    static bool     instanceFlag_;

    Game            *owner_;
};

#endif //WORLD_H
