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

    std::vector<GLuint> getTileMap() { return tilemap_textures_; }

    bool loadAssets();

    GLuint loadBMP();

    void setOwner(Game *g);

private:

    AssetManager();

    static AssetManager         *thisPointer_;

    static bool                 instanceFlag_;

    Game                        *owner_;

    //Tilemap
    std::vector<GLuint>         tilemap_textures_;

    Map                         *map_data_;
};

#endif //AssetManager_H
