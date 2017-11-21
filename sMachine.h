#ifndef SMACHINE_H_INCLUDED
#define SMACHINE_H_INCLUDED

#include "potato.h"
#include "state.h"

typedef struct n_sMachine
{
    State* cState;
    State* states[5];
    int lStates;
} sMachine;

int initSDL(Settings* s);
int initSM(sMachine*);
void mainLoop(sMachine* sm);

int changeState(sMachine* sm, State* s);
int switchState(sMachine* sm, int i1, int i2);

int addState(sMachine* sm, State* s);
int popState(sMachine* sm);
void flushStates(sMachine* sm);
void destroyStates(sMachine* sm);
#endif // SMACHINE_H_INCLUDED
