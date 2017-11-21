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
                GUI_LabelExtension* pLE = (GUI_LabelExtension*) e->pExtension;
                printf("stage #%d -- %s\n", pLE->stage, pLE->strings[0]);
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

    if (e->type == GUI_LABEL)
    {
        printText(e->res->spr->tex, getLabelExtText(e), &e->coords);
    }
};

void printText(SDL_Texture* t, const char* text, SDL_Rect* rect)
{
    TTF_Font* font = TTF_OpenFont("./assets/ahronbd.ttf", 18);
    if (font == NULL) printf("NULL font %s\n", TTF_GetError());

    SDL_Color colour = {255, 255, 0};

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, colour);

    SDL_Texture* mes = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0;  //controls the rect's x coordinate
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect

    SDL_RenderCopy(renderer, mes, NULL, rect);
}


