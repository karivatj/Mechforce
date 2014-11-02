#ifndef CGAME_H
#define CGAME_H

#include <vector>

#include "common.h"

#include "cWorld.h"
#include "cRenderer.h"
#include "cEventHandler.h"
#include "cAssetManager.h"
#include "cEntity.h"
#include "cMap.h"

class Game
{
public:

    Game();

    ~Game();

    void Start();

private:

    bool loadAssets();

    World *world_;

    Renderer *renderer_;

    EventHandler *eventhandler_;

    AssetManager *assetmanager_;

    /*FPS related variables*/
    float frametime_;
    float framedelta_;
    float framebudget_;
    float framerate_;
};

#endif // CGAME_H
