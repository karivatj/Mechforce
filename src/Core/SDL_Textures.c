#include "SDL/SDL_image.h"
#include "../main.h"
#include "SDL_Engine.h"
#include "SDL_Textures.h"
#include "FreeImage.h"

void Texture_LoadTextures(void)
{
    printf("Loading textures.\n");

    backgrounds[0] = Texture_LoadImage("../Images/MainMenu_BG.jpg", 1, JPEG_DEFAULT);
    backgrounds[1] = Texture_LoadImage("../Images/Options_BG.jpg", 1, JPEG_DEFAULT);
    backgrounds[2] = Texture_LoadImage("../Images/HUD.jpg", 1, JPEG_DEFAULT);
    backgrounds[3] = Texture_LoadImage("../Images/HUD_Mask.jpg", 1, JPEG_DEFAULT);

    buttontextures[0] = Texture_LoadImage("../Images/ButtonRect.png", 1, PNG_DEFAULT);
    buttontextures[1] = Texture_LoadImage("../Images/ButtonRect_Mouseover.png", 1, PNG_DEFAULT);
    buttontextures[2] = Texture_LoadImage("../Images/ButtonRect_Pressed.png", 1, PNG_DEFAULT);
    buttontextures[3] = Texture_LoadImage("../Images/ButtonRect_Mask.png", 2, PNG_DEFAULT);
    buttontextures[4] = Texture_LoadImage("../Images/ButtonRectSmall.png", 1, PNG_DEFAULT);
    buttontextures[5] = Texture_LoadImage("../Images/ButtonRectSmall_Mouseover.png", 1, PNG_DEFAULT);
    buttontextures[6] = Texture_LoadImage("../Images/ButtonRectSmall_Pressed.png", 1, PNG_DEFAULT);
    buttontextures[7] = Texture_LoadImage("../Images/ButtonRectSmall_Mask.png", 1, PNG_DEFAULT);
    buttontextures[8] = Texture_LoadImage("../Images/ButtonRound_Red.png", 1, PNG_DEFAULT);
    buttontextures[9] = Texture_LoadImage("../Images/ButtonRound_Green.png", 1, PNG_DEFAULT);
    buttontextures[10]= Texture_LoadImage("../Images/ButtonRound_Mask.png", 1, PNG_DEFAULT);
    buttontextures[11]= Texture_LoadImage("../Images/CheckBox.png", 1, PNG_DEFAULT);
    buttontextures[12]= Texture_LoadImage("../Images/CheckBox_Enabled.png", 1, PNG_DEFAULT);
    buttontextures[13]= Texture_LoadImage("../Images/CheckBox_Mask.png", 1, PNG_DEFAULT);

    printf("Finished loading textures.\n\n");
}

GLuint Texture_LoadImage(char *filename, int type, int flag)
{
    GLuint texture, width, height;

    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN; //Variable used in determining the imageformat
    FIBITMAP *bitmap;                    //Pointer to the image
    BYTE* bits;                          //Pointer to the image data

    fif = FreeImage_GetFileType(filename, 0);

    if(fif == FIF_UNKNOWN)  //If filetype is still unknown
        fif = FreeImage_GetFIFFromFilename(filename);

    if(fif == FIF_UNKNOWN)  //If all failed then quit
        SDL_Close(-2);

    if(FreeImage_FIFSupportsReading(fif))   //Does freeimage support reading this image type
        bitmap = FreeImage_Load(fif, filename, flag);

    if(!bitmap)
        SDL_Close(-2);

    FreeImage_FlipVertical(bitmap); //Flip the image vertically its upside down by default

    bits   = FreeImage_GetBits(bitmap);
    width  = FreeImage_GetWidth(bitmap);
    height = FreeImage_GetHeight(bitmap);

    if((bits == 0) || (width == 0) || (height == 0)) //If something failed
        SDL_Close(-2);

    texture = Texture_GenerateGLTexture(bits, type, width, height); //Generate a GL texture from the freeimage data
    printf("Loaded image %s\n", filename);

    FreeImage_Unload(bitmap);   //Free the memory

    return texture;
}

GLuint Texture_GenerateGLTexture(BYTE *bits, int type, GLuint width, GLuint height)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    switch(type)
    {
        case 0:
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, bits);
        break;

        case 1:
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits );
        break;

        case 2:
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits );
        break;

        default:
            SDL_Close(-1);
        break;
    }

    if((glGetError()) != GL_NO_ERROR)
        fprintf(stderr, "ERROR*** GL Error: %x", glGetError());

    return texture;
}

void Texture_GenerateTilemap()
{
#if 0
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
#endif
}

int Texture_Cleanup()
{
    printf("Deleting textures");
    SDL_FreeSurface(screen);
    glDeleteTextures(3,&backgrounds[0]);
    glDeleteTextures(11,&buttontextures[0]);
    glDeleteTextures(MAX_TILES,&tiletexture[0]);

    glDeleteLists(background,1);
    glDeleteLists(state,1);

    return 0;
}
