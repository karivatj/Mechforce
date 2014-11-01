#ifndef CGAME_H
#define CGAME_H

#include <vector>

#include "SDL2/SDL.h"

#include "cRenderer.h"
#include "cWorld.h"
#include "cEventHandler.h"
#include "cAssetManager.h"

class Renderer;
class AssetManager;
class EventHandler;
class World;

class Game
{
public:

    Game();

    ~Game();

    void Start();

    Renderer* getRenderer();

    AssetManager* getAssetManager();

private:

    bool loadAssets();

    World *world_;

    Renderer *renderer_;

    EventHandler *eventhandler_;

    AssetManager *assetmanager_;

    EntityVector entities_;

    /*FPS related variables*/
    float frametime_;
    float framedelta_;
    float framebudget_;
    float framerate_;
};

#endif // CGAME_H
