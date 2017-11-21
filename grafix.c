#include "grafix.h"
#include "potato.h"

SpritesArray256* readRes(char* path)
{
    SpritesArray256* pSArray = (SpritesArray256*) malloc(sizeof(SpritesArray256));
    pSArray->lSprites = 0;

    int pathLength = 256;
    FILE* f = fopen(path, "r");
    char* sprPath = (char*) malloc(pathLength*sizeof(char));

    //load Texture for sprites
    fscanf(f, "%s", sprPath);
    SDL_Texture* tex = loadTexture(sprPath);

    //construct sprites
    char tempChar;
    while((tempChar = fgetc(f)) && (tempChar != EOF))
    {
        ungetc(tempChar, f);
        Sprite* spr = constructSprite(f);
        spr->tex = tex;
        addSpriteToArray256(spr, pSArray);
    }
    //printf("%s %s\n", pSArray->sprites[0]->name, pSArray->sprites[1]->name);
    fclose(f);

    return pSArray;
}

Sprite* constructSprite(FILE* f)
{
    if (f == NULL)
        return NULL;
    Sprite* spr = (Sprite*) malloc(sizeof(Sprite));
    //name
    fscanf(f, "%s", spr->name);

    //sources
    int sIt = 0;
    char tempChar;
    while((tempChar = fgetc(f)) && (tempChar != EOF) && (tempChar != ']'))
    {
        printf("%c", tempChar);
        switch (tempChar)
        {
        case '{':
            printf("case { %c %d\n", tempChar, sIt);
            if (sIt < 12)
                fscanf(f, "%d, %d, %d, %d}", &(spr->sources[sIt].x), &(spr->sources[sIt].y), &(spr->sources[sIt].w), &(spr->sources[sIt].h));
            sIt++;
            break;
        case '[': case ',': case ' ': case '}':
        default:
            break;
        }
    }

    return spr;
};

SDL_Texture* loadTexture(char* path)
{
    if (!path)
    {
        printf("improper path sprite \n");
        return NULL;
    }

    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
    {
        printf("failed load sprite %s\n", path);
        return NULL;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (tex == NULL)
    {
        printf("SDL_CreateTextureFromSurface failed\n");
        return NULL;
    }

    free(loadedSurface);
    return tex;
}

void addSpriteToArray256(Sprite* spr, SpritesArray256* pSArray)
{
    pSArray->sprites[pSArray->lSprites] = spr;
    pSArray->lSprites++;
};

Sprite* getPSpriteByName(SpritesArray256* sprArr, const char* name)
{
    printf("g1 \n");

    for (int i = 0; i < sprArr->lSprites; i++)
    {
        if (strcmp(sprArr->sprites[i]->name, name) == 0)
        {
            printf("g2 \n");
            return sprArr->sprites[i];
        }
    }

    printf("g2 null for %s \n", name);

    return NULL;
};

