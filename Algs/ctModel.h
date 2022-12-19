#ifndef CTMODEL_H
#define CTMODEL_H

#include "defs.h"


class ctModel {
public:
    ctModel();
    ~ctModel();
    
    bool update(char buf[10000], int _A[2], bool _XAI);
    int lookUp(char ch, int index, int _A[2]);
    
    int me;
    int lastProposed[2][2];
    int notThis;
    bool alreadyPickedLast;
    int currentStep;
    bool enforcer;
    bool vengeful;
    bool happy;
    int tsProposal;
    
    double elapsed;
    
    int numMessagesReceived;
};

#endif