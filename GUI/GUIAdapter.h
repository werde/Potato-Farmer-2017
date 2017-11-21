#ifndef GUIADAPTER_H_INCLUDED
#define GUIADAPTER_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "GUI.h"
#include "../grafix.h"

typedef struct
{

} GUIAdapter;

void handleEvent(GUI*, SDL_Event*);
GUIResource* makeResource(SpritesArray256*, const char*);
void drawElem(GUIElem*);
#endif // GUIADAPTER_H_INCLUDED
