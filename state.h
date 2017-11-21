#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <stdlib.h>
#include <SDL2/SDL.h>

#include "grafix.h"
#include "GUI/GUIAdapter.h"

struct n_sMachine;
typedef struct n_sMachine sMachine;

typedef struct _State
{
    int id;
    struct _State* eto;
    void (*render)(struct _State* eto);
    void (*proc)(struct _State* eto, SDL_Event* ev);
    GUI* gui;
    SpritesArray256* pSprArray;
    SDL_Texture* screen;
    sMachine* sm;
} State;

State* constructState(SDL_Texture* screen, sMachine* sm);

#endif // STATE_H_INCLUDED
