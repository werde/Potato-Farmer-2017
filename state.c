#include "state.h"
#include "potato.h"
#include "GUI/GUIAdapter.h"

void stateQuit(GUIElem*);

void defRender(State* eto)
{
    //printf("rendering %d\n", eto->id);
    //render content
    /*
    for (int i = 0; i < eto->pSprArray->lSprites; i++)
    {
        //Sprite* t = eto->pSprArray->sprites[i];
        //printf("\t render %d %d\n", i, t->tex);
        //SDL_RenderCopy(renderer, t->tex, NULL, &(t->dest)); //&(t->sources[0])
    }
*/

    if (eto->gui)
    {
        SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);

        GUIElem* e = eto->gui->elemsByZ;
        while(e->flags & GUI_E_VALID)
        {
            //printf("%s ", e->tag);
            //if (e->pParent)
            //    printf("(%s) ", e->pParent->tag);
            Sprite* t = eto->pSprArray->sprites[0];
            if (e->flags & GUI_E_VISIBLE)
                //SDL_RenderCopy(renderer, e->res->spr->tex, &(e->res->spr->sources[0]), (SDL_Rect*) &e->coords);
                //SDL_RenderFillRect(renderer, (SDL_Rect*) &e->coords);
                drawElem(e);
            e++;
        }
        //printf("\n");
        SDL_SetRenderDrawColor(renderer, 120, 60, 60, 255);
    }
}

void defProc(State* eto, SDL_Event* ev)
{
    sMachine* sm = eto->sm;
    //printf("defProc %d\n", quit);
/*
    switch (ev->type)
    {
    case SDL_KEYDOWN:
        switch (ev->key.keysym.scancode)
        {
        case SDL_SCANCODE_X:
            quit = 1;
            break;
        case SDL_SCANCODE_A:
            {
                //State* s;
                //s = constructState(screen, sm);
                //addState(sm, s);
            }
            break;
        case SDL_SCANCODE_P:
            {
                //popState(sm);
            }
            break;
        case SDL_SCANCODE_F:
            {
                //flushStates(sm);
            }
            break;
        case SDL_SCANCODE_D:
            {
                //destroyStates(sm);
            }
            break;
        default:
            printf("%d ", ev->key.keysym.scancode);
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        printf("SDL_MOUSEBUTTONDOWN\n");
        GUIElem* tempEl = getClickedElem(eto->gui, ev->button.x, ev->button.y);
        printf("clicked x=%d y=%d !\n", ev->button.x, ev->button.y);
        if (tempEl)
        {
            printf("clicked on %s\n", tempEl->tag);
            if (tempEl->type == GUI_TAB)
                tabClick(tempEl);
        }
        break;
    default:
        break;
    }
*/
    handleEvent(eto->gui, ev);
}

State* constructState(SDL_Texture* screen, sMachine* sm)
{
    State* s = (State*) malloc(sizeof(State));
    s->id = rand();
    s->eto = s;
    s->render = &defRender;
    s->proc = &defProc;
    s->screen = screen;
    s->sm = sm;

    s->pSprArray = readRes("./data/gui.res");

    s->gui = initTestGUI();

    // link res to gui
    for (GUIElem* el = s->gui->elemsByZ; el->flags & GUI_E_VALID; el++)
    {
        if (el->flags & GUI_E_HASNOT_RES) continue;
        el->res = makeResource(s->pSprArray, el->resTag);
        if (!el->res->spr) el->res->spr = s->pSprArray->sprites[0];
    }

    GUIElem* ng = getElemByTag(s->gui, "ng");
    ng->click = stateQuit;

    return s;
}

void stateQuit(GUIElem* el)
{
    printf("stateQuit");
    quit = 1;
}
