#include "GUIAdapter.h"
#include "../potato.h"

void handleMouseButtonDown(GUI* gui, GUIElem* e)
{
    if (e->click)
    {
        e->click(e);
        return;
    }


    switch (e->type)
    {
    case GUI_BUTTON:
        //fire function assigned to button
        break;
    case GUI_TAB:
        tabClick(e);
        break;
    default:
        break;
    }
};

void handleClick(GUI* gui, GUIElem* e, SDL_Event* ev)
{
    switch (ev->type)
    {
    case SDL_MOUSEBUTTONDOWN:
        {
            handleMouseButtonDown(gui, e);
        }
    case SDL_MOUSEBUTTONUP:
        {
            //stop dragging
        }
    default:
        break;
    }
};

void handleEvent(GUI* gui, SDL_Event* ev)
{
    switch (ev->type)
    {
    case SDL_MOUSEMOTION:
        {

        }
        break;
    case SDL_MOUSEBUTTONDOWN: SDL_MOUSEBUTTONUP:
        {
            GUIElem* e = getClickedElem(gui, ev->button.x, ev->button.y);
            if (!e) return;
            printf("clicked on %s\n", e->tag);
            if (e->pExtension && (e->type == GUI_LABEL))
            {
                LabelExtension* pLE = (LabelExtension*) e->pExtension;
                printf("stage #%d -- %s\n", pLE->stage, pLE->stages);
            }
            handleClick(gui, e, ev);
        }
        break;
    case SDL_KEYDOWN:
        break;
    default:
        break;
    }
}

GUIResource* makeResource(SpritesArray256* sprArr, const char* tag)
{
    printf("1 \n");
    GUIResource* pGUIRes = malloc(sizeof(GUIResource));

    pGUIRes->spr = getPSpriteByName(sprArr, tag);
    pGUIRes->frame = 0;
    printf("%s %s \n", tag, pGUIRes->spr->name);
    printf("2 \n");
    return pGUIRes;
}

void drawElem(GUIElem* e)
{
    if (e->flags & GUI_E_HASNOT_RES) return;
    SDL_RenderCopy(renderer, e->res->spr->tex, &(e->res->spr->sources[0]), (SDL_Rect*) &e->coords);
};
