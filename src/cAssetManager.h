#ifndef AssetManager_H
#define AssetManager_H

#include <vector>

#include "SDL2/SDL.h"
#include "cRenderer.h"
#include "cMap.h"
#include "common.h"

class Renderer;
class Map;

class AssetManager
{
public:

    static AssetManager* getInstance();

    ~AssetManager();

    Map* getMap() { return map_data_; }

    std::vector<GLuint> getTileMap() { return tileTextures_; }

    bool loadAssets();

    void setOwner(Game *g);

private:

    AssetManager();

    static AssetManager         *thisPointer_;

    static bool                 instanceFlag_;

    Game                        *owner_;

    //Tilemap
    std::vector<GLuint>         tileTextures_;

    Map                         *map_data_;
};

#endif //AssetManager_H
