#include <iostream>
#include <SDL2/SDL.h>

#include "cGame.h"
#include "common.h"

Game::Game() :  frametime_(0),
                framedelta_(0),
                framebudget_(0),
                framerate_(60.f)
{
    std::cout << "Mechforce: Starting up" << std::endl;

    appRunning = true;

    framebudget_ = 1000.f/framerate_;

    try
    {
        world_ = World::getInstance();
        eventhandler_ = EventHandler::getInstance();

        renderer_ = Renderer::getInstance();
        renderer_->setOwner(this);

        assetmanager_ = AssetManager::getInstance();
        assetmanager_->setOwner(this);

        if(!assetmanager_->loadAssets())    //Assetmanager failed to load assets
        {
            appRunning = false;
            return;
        }
    }
    catch(std::bad_alloc &e)
    {
        std::cout << "Error: " << e.what() << " while trying to allocate memory." << std::endl;
        appRunning = false;
    }
}

Game::~Game()
{
    std::cout << "Mechforce: Shutting down" << std::endl;

    delete world_;
    delete eventhandler_;
    delete renderer_;
    delete assetmanager_;
}

Renderer* Game::getRenderer()
{
    return renderer_;
}

AssetManager* Game::getAssetManager()
{
    return assetmanager_;
}

void Game::Start()
{
    while(appRunning)
    {
        frametime_ = SDL_GetTicks();

        world_->update(frametime_);
        renderer_->update(frametime_);
        eventhandler_->update();

        framedelta_ = SDL_GetTicks() - frametime_;

        if(framedelta_ < framebudget_)
            SDL_Delay((int)framebudget_ - (int)framedelta_);
    }
}
