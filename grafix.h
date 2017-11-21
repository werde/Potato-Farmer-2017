#ifndef GRAFIX_H_INCLUDED
#define GRAFIX_H_INCLUDED

//#include "potato.h"
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    SDL_Rect dest;
    SDL_Rect sources[12];
    int lSources;
    char name[64];
    SDL_Texture* tex;
} Sprite;

typedef struct
{
    int lSprites;
    Sprite* sprites[256];
} SpritesArray256;

SpritesArray256* readRes(char*);
SDL_Texture* loadTexture(char*);
Sprite* constructSprite(FILE*);
void addSpriteToArray256(Sprite*, SpritesArray256*);
Sprite* getPSpriteByName(SpritesArray256*, const char*);
#endif // GRAFIX_H_INCLUDED
