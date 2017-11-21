#include "sMachine.h"

int initSDL(Settings* s)
{
    //SDL PART
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    };
    TTF_Init();

    if (SDL_CreateWindowAndRenderer(s->width, s->height, s->window_flags, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    };

    SDL_SetRenderDrawColor(renderer, 120, 60, 60, 255);

    screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, s->width, s->height);

    return 0;
};

int initSM(sMachine* sm)
{
    quit = 0;
    State* st = constructState(screen, sm);
    changeState(sm, st);

    return 0;
};

void processEvents(sMachine* sm)
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev) > 0)
    {
        sm->cState->proc(sm->cState, &ev);
    };
}

void mainLoop(sMachine* sm)
{
    while (!quit)
    {
        processEvents(sm);

        SDL_RenderClear(renderer);

        SDL_SetRenderTarget(renderer, screen);
        SDL_RenderClear(renderer);
        for (int i = 0; i < sm->lStates; i++)
        {
            sm->states[i]->render(sm->states[i]);
        }
        SDL_SetRenderTarget(renderer, NULL);

        SDL_RenderCopy(renderer, screen, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
    }
};

int changeState(sMachine* sm, State* s)
{
    addState(sm, s);
    sm->cState = s;

    return 0;
};

int setState(sMachine* sm, int i)
{
    switchState(sm, i, sm->lStates - 1);
    sm->cState = sm->states[sm->lStates - 1];

    return 0;
};

int switchState(sMachine* sm, int i1, int i2)
{
    if ((i1 >= sm->lStates) && (i2 >= sm->lStates)) return 1;

    State* temp;
    temp = sm->states[i1];
    sm->states[i1] = sm->states[i2];
    sm->states[i2] = temp;

    return 0;
};

int addState(sMachine* sm, State* s)
{
    if (sm->lStates < 5)
    {
        sm->states[sm->lStates] = s;
        sm->lStates++;
        return 0;
    }

    return 1;
}

int popState(sMachine* sm)
{
    if (sm->lStates > 0)
    {
        sm->lStates--;
        return 0;
    }

    return 1;
}

void flushStates(sMachine* sm)
{
    sm->lStates = 0;
};

void destroyStates(sMachine* sm)
{
    int i = 0;
    for(; i < sm->lStates; i++)
    {
        free(sm->states[i]);
    }

    sm->lStates = 0;
};

