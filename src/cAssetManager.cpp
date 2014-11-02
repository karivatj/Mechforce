#include <iostream>

#include "cAssetManager.h"

bool AssetManager::instanceFlag_ = false;
AssetManager* AssetManager::thisPointer_ = NULL;

AssetManager* AssetManager::getInstance()
{
    if(instanceFlag_)
    {
        return thisPointer_;
    }
    else
    {
        thisPointer_ = new AssetManager();
        instanceFlag_ = true;
        return thisPointer_;
    }
}

AssetManager::AssetManager()
{
    map_data_ = new Map();
}

AssetManager::~AssetManager()
{
    delete map_data_;

    std::vector<GLuint>::iterator iter;
    for (iter = tileTextures_.begin();iter < tileTextures_.end();++iter)
    {
        glDeleteTextures(1,&(*iter));
        tileTextures_.erase(iter);
    }
    std::cout << "AssetManager: Destroyed" << std::endl;
}

bool AssetManager::loadAssets()
{
    Uint32 rmask, gmask, bmask, amask;
    int tilemap_width = 4;
    int tilemap_height = 4;

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
     rmask = 0xff000000;
     gmask = 0x00ff0000;
     bmask = 0x0000ff00;
     amask = 0x000000ff;
    #else
     rmask = 0x000000ff;
     gmask = 0x0000ff00;
     bmask = 0x00ff0000;
     amask = 0xff000000;
    #endif

    SDL_Surface *tilemap = SDL_LoadBMP("../Images/tilemap.bmp");

    if(tilemap == NULL)
    {
        std::cout << "Error while loading tilemap: " << SDL_GetError() << std::endl;
        return false;
    }

    /*Basic stuff, load tilemap from file and parse it through to generate 32x32 sized GL Textures*/
    for(int y = 0; y < tilemap_width; y++)
    {
        for(int x = 0; x < tilemap_height; x++)
        {
            SDL_Surface *temp = SDL_CreateRGBSurface(0, 32, 32, 32, rmask, gmask, bmask, amask);

            if(temp == NULL)
            {
                fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
                return false;
            }

            int slice_x = x * 32;
            int slice_y = y * 32;

            SDL_Rect srcRect;
            srcRect.x = slice_x;
            srcRect.y = slice_y;
            srcRect.w = 32;
            srcRect.h = 32;

            SDL_Rect dstRect;
            dstRect.x = 0;
            dstRect.y = 0;

            SDL_SetColorKey(tilemap, SDL_TRUE, 255);
            SDL_BlitSurface(tilemap, &srcRect, temp, &dstRect);

            GLuint texture;

            glGenTextures(1, &texture);
            glBindTexture( GL_TEXTURE_2D, texture);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);

            if((glGetError()) != GL_NO_ERROR)
                fprintf(stderr, "ERROR*** GL Error: %x", glGetError());

            tileTextures_.push_back(texture);
            SDL_FreeSurface(temp);
            temp = NULL;
        }
    }

    SDL_FreeSurface(tilemap);
    tilemap = NULL;

    map_data_->generateMap();

    return true;
}
