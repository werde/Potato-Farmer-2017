#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "GUIResource.h"

//TODO use standart library
typedef unsigned char uint8_t;

//TODO use standart library
typedef struct
{
    int x;
    int y;
    int w;
    int h;
} GUI_Rect;

// Flags for GUI element state
typedef enum
{
    GUI_E_VISIBLE = 0x00000001,                 /**< visible element        */
    GUI_E_INTERACTIVE = 0x00000002,             /**< interactive element    */
    GUI_E_CHOSEN = 0x00000004,                  /**< is active element      */
    GUI_E_MOVABLE = 0x00000008,                 /**< is element movable     */
    GUI_E_PARENT_CONNECTED = 0x00000010,        /**< is connected to parent while moving parent */
    GUI_E_HASNOT_RES = 0x00000020,              /**< */
    GUI_E_MINIMIZED = 0x00000040,               /**< */
    GUI_E_MAXIMIZED = 0x00000080,               /**< */
    GUI_E_INPUT_GRABBED = 0x00000100,           /**< */
    GUI_E_INPUT_FOCUS = 0x00000200,             /**< */
    GUI_E_MOUSE_FOCUS = 0x00000400,             /**< */
    GUI_E_FOREIGN = 0x00000800,                 /**< */
    GUI_E_VALID = 0x00002000                    /**< */
} GUI_ElementsFlags;

// GUI elements types
typedef enum {GUI_GENERIC_ELEMENT, GUI_LABEL, GUI_BUTTON, GUI_TAB, GUI_TAB_CONTAINER, GUI_SPINNER} GUIElementsTypes;

// GUI element structure
typedef struct GUIElem
{
    GUI_ElementsFlags flags;
    GUIElementsTypes type;
    char tag[16];

    GUI_Rect coords;
    uint8_t z;

    void* pExtension;
    // children and parent
    struct GUIElem* pChildren[10];
    uint8_t nChildren;
    struct GUIElem* pParent;
    // functions
    void (*click)(struct GUIElem*);
    // external resoirce
    char resTag[16];
    GUIResource* res;
} GUIElem;

typedef struct
{
    char strings[10][20];
    int nStrings;
    int stage;
} GUI_LabelExtension;

char* getLabelExtText(GUIElem* pLbl);
GUI_LabelExtension* getPLabelExt(GUIElem* pLbl);

void leftArrowClick(GUIElem* pLBtn);
void rightArrowClick(GUIElem* pRBtn);
// Container struct for GUI elements
typedef struct
{
    GUIElem elemsByZ[25];
    GUIElem* elemsActive[25];
} GUI;

GUI* initTestGUI();
void sortElemsByZ(GUI*);
void setGUIRes(GUIElem*, GUIResource*);
GUIElem* pushBackElem(GUI*, GUIElem*);
GUIElem* getElemByTag(GUI*, const char* tag);

GUIElem* getClickedElem(GUI*, int, int);
void tabClick(GUIElem*);

#endif // GUI_H_INCLUDED
