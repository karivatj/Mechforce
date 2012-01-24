#include "SDL/SDL_image.h"
#include "../Mechforce.h"
#include "SDL_Engine.h"

void SDL_LoadTextures(void)
{
    GLuint LoadImg(char *filename, int type)
    {
        SDL_Surface *btnSurface;
        GLuint texture;

        btnSurface = IMG_Load(filename);

        if(!btnSurface)
        {
            printf("ERROR*** SDL Image error: %s\n",IMG_GetError());
            SDL_Close(-2);
        }

        texture = glGenerateTexture(btnSurface, type);
        printf("Loaded image %s\n", filename);

        SDL_FreeSurface(btnSurface);

        return texture;
    }

    printf("Loading textures.\n");

    backgrounds[0] = LoadImg("../Images/MainMenu_BG.jpg", 1);
    backgrounds[1] = LoadImg("../Images/Options_BG.jpg", 1);
    backgrounds[2] = LoadImg("../Images/HUD.jpg", 1);
    backgrounds[3] = LoadImg("../Images/HUD_Mask.jpg", 1);

    buttontextures[0] = LoadImg("../Images/ButtonRect.png", 1);
    buttontextures[1] = LoadImg("../Images/ButtonRect_Mouseover.png", 1);
    buttontextures[2] = LoadImg("../Images/ButtonRect_Pressed.png", 1);
    buttontextures[3] = LoadImg("../Images/ButtonRect_Mask.png", 2);
    buttontextures[4] = LoadImg("../Images/ButtonRectSmall.png", 1);
    buttontextures[5] = LoadImg("../Images/ButtonRectSmall_Mouseover.png", 1);
    buttontextures[6] = LoadImg("../Images/ButtonRectSmall_Pressed.png", 1);
    buttontextures[7] = LoadImg("../Images/ButtonRectSmall_Mask.png", 1);
    buttontextures[8] = LoadImg("../Images/ButtonRound_Red.png", 1);
    buttontextures[9] = LoadImg("../Images/ButtonRound_Green.png", 1);
    buttontextures[10]= LoadImg("../Images/ButtonRound_Mask.png", 1);
    buttontextures[11]= LoadImg("../Images/CheckBox.png", 1);
    buttontextures[12]= LoadImg("../Images/CheckBox_Enabled.png", 1);
    buttontextures[13]= LoadImg("../Images/CheckBox_Mask.png", 1);

    printf("Finished loading textures.\n\n");
}

GLuint glGenerateTexture(SDL_Surface *surface, int type)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    switch(type)
    {
        case 0:
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, surface->w, surface->h, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
        break;

        case 1:
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels );
        break;

        case 2:
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
        break;

        default:
            SDL_Close(-1);
        break;
    }

    if((glGetError()) != GL_NO_ERROR)
        fprintf(stderr, "ERROR*** GL Error: %x", glGetError());

    return texture;
}

void SDL_GenerateTilemap()
{
    int i;
    int x,y;
    int slice_x, slice_y;

    SDL_Surface *tilemap;               //tilemap surface
    SDL_Surface *tiles[MAX_TILES];    //16 state surfaces
    Uint32 rmask, gmask, bmask, amask;

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

    printf("Generating tilemap.\n");

    if((tilemap = IMG_Load("../Images/tilemap.png")) == NULL)
    {
        fprintf(stderr,"ERROR*** Error while opening tilemap.png: %s\n", SDL_GetError());
        SDL_Close(-1);
    }

    printf("Succesfully loaded tilemap to memory.\n");

    for(i = 0; i < MAX_TILES; i++)
        tiles[i] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, TILE_WIDTH, TILE_HEIGHT, 32, rmask, gmask, bmask, amask);

    for(y = 0; y < TILEMAP_HEIGHT; y++)
    {
        for(x = 0; x < TILEMAP_WIDTH; x++)
        {
            slice_x = x * TILE_WIDTH;
            slice_y = y * TILE_HEIGHT;

            SDL_Rect srcRect;
            srcRect.x = slice_x;
            srcRect.y = slice_y;
            srcRect.w = TILE_WIDTH;
            srcRect.h = TILE_HEIGHT;

            SDL_Rect dstRect;
            dstRect.x = 0;
            dstRect.y = 0;

            int i =  x + (TILEMAP_WIDTH * y);

            SDL_SetColorKey(tilemap, SDL_SRCCOLORKEY, SDL_MapRGB(tilemap->format, 255, 255, 255 ));
            SDL_BlitSurface(tilemap, &srcRect, tiles[i], &dstRect);
        }
    }

    SDL_FreeSurface(tilemap);

    /*Generate OpenGL Textures*/
    for(i = 0; i < MAX_TILES; i++)
    {
        if(tiles[i]->pixels != NULL)
            tiletexture[i] = glGenerateTexture(tiles[i], 2);

        SDL_FreeSurface(tiles[i]);
    }

    printf("Generated tilemap succesfully.\n\n");
}

