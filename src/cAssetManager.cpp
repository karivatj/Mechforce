#include <iostream>

#include "common.h"
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
    map_data_ = new Map(32,32);
    std::cout << "AssetManager: Created" << std::endl;
}

AssetManager::~AssetManager()
{
    delete map_data_;

    std::vector<GLuint>::iterator iter;
    for (iter = tilemap_textures_.begin();iter < tilemap_textures_.end();++iter)
    {
        glDeleteTextures(1,&(*iter));
        tilemap_textures_.erase(iter);
    }
    std::cout << "AssetManager: Destroyed" << std::endl;
}

void AssetManager::setOwner(Game *g)
{
    owner_ = g;
}

/*Loads the given BMP file and returns a openGL textureId to the caller*/
GLuint loadBMP(std::string filename, bool flip)
{
    GLuint texture;

    bool error = false;

    std::cout << "Reading file: " << filename << std::endl;

    int imgWidth;
    int imgHeight;

    // BMP file header data
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;

    // Pointer to image data
    unsigned char * data;

    FILE * file = fopen(filename.c_str(), "rb");

    /*Error checking*/
    if (!file)
        error = true;
    if (fread(header, 1, 54, file) != 54 )     //If length smaller than 54 theres a problem
        error = true;
    if (header[0]!='B' || header[1]!='M' )    //If the file does not start with BM its not right
        error = true;
    if (*(int*)&(header[0x1E])!=0  )           //Check if compression is set to none
        error = true;
    if (*(int*)&(header[0x1C])!=24 )           //Check if the file is 24bpp
        error = true;

    if(error)   //If an error occured
    {
        std::cout << "Could not open image or invalid BMP file!" << std::endl;
        return -1;
    }

    // Read the information about the image
    dataPos       = *(int*)&(header[0x0A]);
    imageSize     = *(int*)&(header[0x22]);
    imgWidth      = *(int*)&(header[0x12]);
    imgHeight     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0)
        imageSize = imgWidth * imgHeight*3; //1 byte for R, G and B
    if (dataPos == 0)
        dataPos = 54;

    // Create a buffer
    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose (file);

    /*Switch the red and blue bits*/
    for(int i = 0; i < imgWidth * imgHeight ; ++i)
    {
        int index = i * 3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];

        data[index] = R;
        data[index+2] = B;
    }

    if (flip)   //If flipping of the image is requested
    {
        unsigned char * tmpBuffer = new unsigned char[imgWidth*3];
        int size = imgWidth*3;
        for (int i = 0; i < imgHeight / 2; i++)
        {
                // copy row i to tmp
                memcpy(tmpBuffer, data + imgWidth * 3 * i, size);
                // copy row h-i-1 to i
                memcpy(data + imgWidth * 3 * i, data + imgWidth * 3 * (imgHeight - i - 1), size);
                // copy tmp to row h-i-1
                memcpy(data + imgWidth * 3 * (imgHeight - i - 1), tmpBuffer, size);
        }

        delete [] tmpBuffer;
    }

    //Generate GL texture out of the data
    glGenTextures(1, &texture);
    glBindTexture( GL_TEXTURE_2D, texture);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    delete data;
    return texture;
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

    SDL_Surface *tilemap = SDL_LoadBMP("./Images/tilemap.bmp");

    if(tilemap == NULL)
    {
        std::cout << "Error while loading tilemap: " << SDL_GetError() << std::endl;
        return false;
    }

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
/*
            SDL_Renderer *renderer = owner_->getRenderer()->getRenderer();

            if(renderer == NULL)
            {
                fprintf(stderr, "getRenderer failed!\n");
                return false;
            }

            SDL_Texture *tile_tex = NULL;
            tile_tex = SDL_CreateTextureFromSurface(renderer, temp);

            if (tile_tex == NULL)
            {
                fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
                return false;
            }
*/
            GLuint texture;

            //Generate GL texture out of the data
            glGenTextures(1, &texture);
            glBindTexture( GL_TEXTURE_2D, texture);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);

            if((glGetError()) != GL_NO_ERROR)
                fprintf(stderr, "ERROR*** GL Error: %x", glGetError());

            tilemap_textures_.push_back(texture);
            SDL_FreeSurface(temp);
            temp = NULL;
        }
    }

    SDL_FreeSurface(tilemap);
    tilemap = NULL;

    //Generate initial map
    map_data_->generateMap();

    //Everything ok return true
    return true;
}
