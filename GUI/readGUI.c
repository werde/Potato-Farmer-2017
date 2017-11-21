#include "readGUI.h"

GUIElem* eatElem(GUI* gui, FILE* f);
int eatRightArrow(FILE* f);

void readGUI(GUI* gui, const char* path)
{
    FILE* f = fopen(path, "r");

    GUIElem* parentStack[256];
    int szParentStack = 0;

    int lastLevel = 0;
    char tempChar;
    while((tempChar = fgetc(f)) && (tempChar != EOF))
    {
        ungetc(tempChar, f);
        int level = eatRightArrow(f);
        GUIElem* pCurEl = eatElem(gui, f);

        if (level != 0)
        {
            setParent(parentStack[level - 1], pCurEl);  // level will never be less than 1
        }

        parentStack[level] = pCurEl;
        fgetc(f);
    }
};

GUIElem* eatElem(GUI* gui, FILE* f)
{
    char* elTypes[] = {"GUI_GENERIC_ELEMENT", "GUI_LABEL", "GUI_BUTTON", "GUI_TAB", "GUI_TAB_CONTAINER", "GUI_SPINNER"};

    // type
    char typeStr[32];
    fscanf(f, "%s", typeStr);
    GUIElementsTypes type;
    for (int i = 0; i < sizeof(elTypes)/sizeof(char*); i++)
    {
        if (strcmp(elTypes[i], typeStr) == 0) type = i;
    }
    GUIElem *pEl = makeGUIElem(type);

    // tag and resTag
    fscanf(f, "%s", pEl->tag);
    fscanf(f, "%s", pEl->resTag);

    // coords
    int tx, ty, tw, th;
    fscanf(f, " [%d, %d, %d, %d]", &tx, &ty, &tw, &th);
    setCoords(pEl, tx, ty, tw, th);

    GUIElem* pCurEl = pushBackElem(gui, pEl);

    readFlags(f, pCurEl);

    switch(type)
    {
    case GUI_SPINNER:
        initSpinner(gui, pCurEl);

        int wordsRead = readStrings(f, getPLabelExt(pCurEl->pChildren[1])->strings);
        getPLabelExt(pCurEl->pChildren[1])->nStrings = wordsRead;
        break;
    }

    free(pEl);

    return pCurEl;
}

int eatRightArrow(FILE* f)
{
    char tChar = ' ';
    int i = 0;
    while ((tChar = fgetc(f)) && (tChar == '>'))
    {
        //printf("%d \n", i);
        i++;
    }
    ungetc(tChar, f);
    printf("%d %d\n", i, tChar);
    return i;
}

int readStrings(FILE* f, char str[10][20])
{
    int i = 0;
    printf("  %s", str[i]);
    char tChar = ' ';
    while ((tChar = fgetc(f)) && (tChar != '\n'))
    {
        ungetc(tChar, f);
        fscanf(f, "%s", str[i]);
        printf("%s", str[i]);
        i++;
    }
    ungetc(tChar, f);

    return i;
}

int readFlags(FILE* f, GUIElem* e)
{
    int i = 0;
    char tChar = fgetc(f);
    if (tChar != '[')
    {
        ungetc(tChar, f);
        return;
    }
    while ((tChar = fgetc(f)) && (tChar != ']') && (tChar != '\n'))
    {
        int visibility = 1;
        ungetc(tChar, f);
        fscanf(f, "%d", &visibility);

        if (visibility == 0)
        {
            e->flags = e->flags & ~GUI_E_VISIBLE;
        }
    }
    if (tChar != ']') ungetc(tChar, f);

    return i;
}

