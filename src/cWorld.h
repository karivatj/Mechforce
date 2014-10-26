#ifndef WORLD_H
#define WORLD_H

#include "cGame.h"

class Game;

class World
{
public:

    static World* getInstance();

    ~World();

    void update(float frametime);

    void setOwner(Game *g);

private:

    World();

    float           frametime_;

    static World    *thisPointer_;

    static bool     instanceFlag_;

    Game            *owner_;
};

#endif //WORLD_H
