#include <iostream>

#include "cWorld.h"

bool World::instanceFlag_ = false;
World* World::thisPointer_ = NULL;

World* World::getInstance()
{
    if(instanceFlag_)
    {
        return thisPointer_;
    }
    else
    {
        thisPointer_ = new World();
        instanceFlag_ = true;
        return thisPointer_;
    }
}

World::World()
{
    entities_.push_back(new Map());
}

World::~World()
{
}

void World::update(float frametime)
{
}
