#ifndef AssetManager_H
#define AssetManager_H

#include <vector>
#include <SDL2/SDL.h>

#include "cGame.h"

class AssetManager
{

public:

    static AssetManager* getInstance();

    Map* getMap() { return map_data_; }

    std::vector<GLuint> getTileMap() { return tileTextures_; }

    bool loadAssets();

    void setOwner(Game *g) { owner_ = g; }

protected:

    AssetManager();

    ~AssetManager();

private:

    static AssetManager         *thisPointer_;

    static bool                 instanceFlag_;

    Game                        *owner_;

    //Tilemap
    std::vector<GLuint>         tileTextures_;

    Map                         *map_data_;
};

#endif //AssetManager_H
