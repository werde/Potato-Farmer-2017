#include "GUI.h"

GUIElem* makeGeneric();
GUIElem* makeLabel();
GUIElem* makeButton();
GUIElem* makeTab();
GUIElem* makeTabContainer();
GUIElem* makeSpinner();
void setLabelExtensionText(LabelExtension*, char*);

GUIElem* makeGUIElem(GUIElementsTypes type)
{
    switch (type)
    {
    case GUI_LABEL:
        return makeLabel();
        break;
    case GUI_BUTTON:
        return makeButton();
        break;
    case GUI_TAB:
        return makeTab();
        break;
    case GUI_TAB_CONTAINER:
        return makeTabContainer();
        break;
    case GUI_SPINNER:
        return makeSpinner();
        break;
    default:
        return makeGeneric();
        break;
    }

    return 0;
}

GUIElem* makeGeneric()
{
    GUIElem* e = malloc(sizeof(GUIElem));
    e->flags = 0;
    e->flags = e->flags | GUI_E_VISIBLE | GUI_E_VALID;
    e->type = GUI_GENERIC_ELEMENT;
    e->coords.x = 0;
    e->coords.y = 0;
    e->coords.w = 0;
    e->coords.h = 0;

    e->pExtension = 0;
    e->pChildren[0] = 0;
    e->nChildren = 0;
    e->pParent = 0;
    e->click = 0;

    e->res = malloc(sizeof(GUIResource));

    return e;
}

uint8_t setParent(GUIElem* p, GUIElem* c)
{
    if (c->pParent)
        return 0;
    p->pChildren[p->nChildren] = c;
    p->nChildren++;
    c->pParent = p;

    return 1;
}

GUIElem* putElem(GUIElem* pe, GUIElementsTypes type)
{
    GUIElem* temp = makeGUIElem(type);
    memcpy(pe, temp, sizeof(GUIElem));
    free(temp);

    return pe;
}

GUIElem* makeLabel()
{
    GUIElem* e = makeGeneric();
    e->type = GUI_LABEL;
    e->flags |= GUI_E_MOVABLE | GUI_E_PARENT_CONNECTED | GUI_E_INTERACTIVE;

    // Label extension
    LabelExtension* lExt = malloc(sizeof(LabelExtension));
    lExt->delimeter = ';';
    lExt->stage = 0;
    e->pExtension = lExt;

    return e;
}

void setLabelExtensionText(LabelExtension* pLblExt, char* text)
{
    strcpy(pLblExt->stages, text);
};

void setGUIRes(GUIElem* e, GUIResource* res)
{
    e->res = res;
};

GUIElem* pushBackElem(GUI* gui, GUIElem* e)
{
    GUIElem* temp = gui->elemsByZ;
    int szElems = sizeof(gui->elemsByZ)/sizeof(GUIElem) - 1;
    int i = 0;
    while ((temp->flags & GUI_E_VALID) && (i < szElems))
    {
        i++;
        temp++;
    }

    if (i < szElems)
    {
        memcpy(temp, e, sizeof(GUIElem));
        return temp;
    }

    return NULL;
};

GUIElem* getElemByTag(GUI* gui, const char* tag)
{
    GUIElem* pb = gui->elemsByZ;

    while (pb->flags & GUI_E_VALID)
    {
        if (strcmp(tag, pb->tag) == 0) return pb;
    }

    return 0;
};

GUIElem* makeButton()
{
    GUIElem* e = makeGeneric();
    e->type = GUI_BUTTON;
    e->type |= GUI_E_INTERACTIVE;

    return e;
}

GUIElem* makeTab()
{
    GUIElem* e = makeGeneric();
    e->type = GUI_TAB;
    e->type |= GUI_E_INTERACTIVE;
    e->click = tabClick;

    return e;
}

GUIElem* makeSpinner()
{
    GUIElem* e = makeGeneric();
    e->type = GUI_SPINNER;
    e->flags |= GUI_E_INTERACTIVE;
    e->click = 0;

    return e;
}

void initSpinner(GUI* gui, GUIElem* sp)
{
    GUIElem* left = makeButton();
    left = pushBackElem(gui, left);
    strcpy(left->resTag, "left");
    setCoords(left, sp->coords.x + sp->coords.w + 10, sp->coords.y, 50, 50);
    setParent(sp, left);

    GUIElem* label = makeLabel();
    label = pushBackElem(gui, label);
    strcpy(label->resTag, "blank");
    setCoords(label, left->coords.x + left->coords.w + 10, sp->coords.y, 200, 50);
    setParent(sp, label);

    GUIElem* right = makeButton();
    right = pushBackElem(gui, right);
    strcpy(right->resTag, "right");
    setCoords(right, label->coords.x + label->coords.w + 10, sp->coords.y, 50, 50);
    setParent(sp, right);
}

GUIElem* makeTabContainer()
{
    GUIElem* e = makeGeneric();
    e->type = GUI_TAB_CONTAINER;
    e->flags |= GUI_E_HASNOT_RES;

    return e;
}

void setCoords(GUIElem* e, int x, int y, int w, int h)
{
    e->coords.x = x;
    e->coords.y = y;
    e->coords.w = w;
    e->coords.h = h;
};

void toggleVisibility(GUIElem* e)
{
    e->flags = (e->flags & GUI_E_VISIBLE) ? e->flags ^ GUI_E_VISIBLE : e->flags | GUI_E_VISIBLE;
};

void toggleElem(GUIElem* e)
{
    toggleVisibility(e);
    for (int i = 0; i < e->nChildren; i++)
    {
        toggleElem(e->pChildren[i]);
    }
}

void openTab(GUIElem* tab)
{
    if (tab->type != GUI_TAB) return;
    for (int i = 0; i < tab->nChildren; i++)
    {
        //tab->pChildren[i]->flags = tab->pChildren[i]->flags | GUI_E_VISIBLE;
        toggleElem(tab->pChildren[i]);
    }
}

void closeTab(GUIElem* tab)
{
    if (tab->type != GUI_TAB) return;
    for (int i = 0; i < tab->nChildren; i++)
    {
        if (tab->pChildren[i]->flags & GUI_E_VISIBLE)
            toggleElem(tab->pChildren[i]);
            //tab->pChildren[i]->flags = tab->pChildren[i]->flags ^ GUI_E_VISIBLE;
    }
}

void tabClick(GUIElem* tab)
{
    // if Tab doesn't have children do nothing
    if (tab->nChildren < 1) return;

    // if Tab has parent
    // no more than one tab can be open at time
    if (tab->pParent)
    {
        // if clicked on opened tab - close tab
        if (tab->pChildren[0]->flags & GUI_E_VISIBLE)
        {
            closeTab(tab);
            return;
        }
        // open tab
        openTab(tab);

        // close other tabs
        GUIElem* parent = tab->pParent;
        for (int i = 0; i < parent->nChildren; i++)
        {
            if (parent->pChildren[i] != tab)
                closeTab(parent->pChildren[i]);
        }

        return;
    }

    // if Tab has no parent, toggle all children
    for (int i = 0; i < tab->nChildren; i++)
    {
        toggleElem(tab->pChildren[i]);
    }
};


GUIElem* getClickedElem(GUI* g, int x, int y)
{
    for (int i = 0; i < 25; i++)
    {
        GUIElem* e = &g->elemsByZ[i];
        if (!(e->flags & GUI_E_VALID)) break;
        //if (!(e->flags & GUI_E_INTERACTIVE) || !(e->flags & GUI_E_INTERACTIVE))
        //    continue;

        GUI_Rect* r = &(e->coords);
        printf("GUI_Rect* r x=%d y=%d w=%d h=%d!\n", r->x, r->y, r->w, r->h);
        if (((x < r->x + r->w)&&(x > r->x)) &&
            (y < r->y + r->h)&&(y > r->y))
        {
            return e;
        }
    }

    return 0;
};

void testButtonFunc(GUIElem* btn)
{
    printf("testButtonFunc\n");
    if ((btn->pChildren[0]->flags & GUI_E_VALID) && (btn->pChildren[0]->type == GUI_LABEL))
    {
        LabelExtension* pLblExt = (LabelExtension*) btn->pChildren[0]->pExtension;
        pLblExt->stage += 1;
    }
}

GUI* initTestGUI()
{
    GUI* gui = malloc(sizeof(GUI));

    for (int i = 0; i < sizeof(gui->elemsByZ)/sizeof(GUIElem); i++)
    {
        gui->elemsByZ[i].flags = gui->elemsByZ[i].flags & 0;
    }
/*

    // generic #1
    putElem(gui->elemsByZ, GUI_GENERIC_ELEMENT);
    strcpy(gui->elemsByZ[0].tag, "blank");
    // label #1
    putElem(&gui->elemsByZ[1], GUI_LABEL);
    // button #1
    putElem(&gui->elemsByZ[2], GUI_BUTTON);
    gui->elemsByZ[2].click = testButtonFunc;
    setLabelExtensionText((LabelExtension*) gui->elemsByZ[1].pExtension, "text 1;text 2; text 3");
    setParent(&gui->elemsByZ[2], &gui->elemsByZ[1]);

    putElem(&gui->elemsByZ[3], GUI_TAB);
    putElem(&gui->elemsByZ[4], GUI_BUTTON);
    setParent(&gui->elemsByZ[3], &gui->elemsByZ[4]);
    putElem(&gui->elemsByZ[5], GUI_BUTTON);
    setParent(&gui->elemsByZ[3], &gui->elemsByZ[5]);

    putElem(&gui->elemsByZ[6], GUI_TAB);
    putElem(&gui->elemsByZ[7], GUI_LABEL);

    putElem(&gui->elemsByZ[8], GUI_TAB_CONTAINER);
    setParent(&gui->elemsByZ[8], &gui->elemsByZ[3]);
    setParent(&gui->elemsByZ[8], &gui->elemsByZ[6]);
    closeTab(&gui->elemsByZ[3]);

    setCoords(gui->elemsByZ, 10, 10, 100, 100);
    setCoords(&gui->elemsByZ[1], 10, 120, 200, 50);
    setCoords(&gui->elemsByZ[2], 10, 240, 200, 50);

    setCoords(&gui->elemsByZ[3], 10, 360, 200, 50);
    setCoords(&gui->elemsByZ[4], 230, 360, 100, 50);
    setCoords(&gui->elemsByZ[5], 230, 240, 100, 100);

    setCoords(&gui->elemsByZ[6], 10, 480, 200, 50);
    setCoords(&gui->elemsByZ[7], 230, 480, 200, 50);
    setParent(&gui->elemsByZ[6], &gui->elemsByZ[7]);
*/
    readGUI(gui, "./data/main_menu.gui");

    return gui;
};

