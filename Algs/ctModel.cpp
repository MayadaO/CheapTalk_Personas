#include "ctModel.h"

ctModel::ctModel() {
    currentStep = -1;
    lastProposed[0][0] = lastProposed[0][1] = lastProposed[1][0] = lastProposed[1][1] = -1;
    enforcer = vengeful = happy = false;
    tsProposal = 99999;
    alreadyPickedLast = false;
    notThis = -1;
    
    elapsed = -1.0;
}

ctModel::~ctModel() {
}

bool ctModel::update(char buf[10000], int _A[2], bool _XAI) {
    char *tkn;
    int msgType;
    
    tsProposal ++;
    bool secondChance = false;
    
    tkn = strtok(buf, ";");
    //printf("____________\n");
    numMessagesReceived = 0;
    while (true) {
        //printf("\t%s: ", tkn);
        
        if (!strncmp(tkn, "$", 1)) {
            //printf("\n");
            elapsed = atof(tkn+2);
            break;
        }
        
        msgType = atoi(tkn);
        
        numMessagesReceived ++;
        
        if (_XAI) {
            switch (msgType) {
                    //printf("(%i) ", msgType);
                case LETS_COOP:
                    lastProposed[0][0] = lastProposed[1][0] = lookUp(tkn[2], 0, _A);
                    lastProposed[0][1] = lastProposed[1][1] = lookUp(tkn[3], 1, _A);
                    currentStep = 0;
                    vengeful = false;
                    tsProposal = 0;
                    alreadyPickedLast = false;
                    
                    //printf("pure solution: %i %i", lastProposed[0][0], lastProposed[0][1]);
                    break;
                case TAKE_TURNS:
                    lastProposed[0][0] = lookUp(tkn[2], 0, _A);
                    lastProposed[0][1] = lookUp(tkn[3], 1, _A);
                    lastProposed[1][0] = lookUp(tkn[5], 0, _A);
                    lastProposed[1][1] = lookUp(tkn[6], 1, _A);
                    currentStep = -1;
                    vengeful = false;
                    tsProposal = 0;
                    alreadyPickedLast = false;
                    
                    //printf("alternating solution: (%i, %i) then (%i, %i)\n", lastProposed[0][0], lastProposed[0][1], lastProposed[1][0], lastProposed[1][1]);
                    break;
                case SPECIFY:
                    //printf("SPECIFY: ");
                    
                    int acts[2];
                    
                    acts[0] = lookUp(tkn[2], 0, _A);
                    acts[1] = lookUp(tkn[3], 1, _A);
                    
                    //printf("%i, %i\n", acts[0], acts[1]);
                    
                    currentStep = -1;
                    if ((acts[0] == lastProposed[0][0]) && (acts[1] == lastProposed[0][1]))
                        currentStep = 0;
                    else if ((acts[0] == lastProposed[1][0]) && (acts[1] == lastProposed[1][1]))
                        currentStep = 1;
                    
                    //if (currentStep >= 0)
                    //    printf("time to play: %i %i.  ", lastProposed[currentStep][0], lastProposed[currentStep][1]);
                    else {
                        lastProposed[0][0] = lastProposed[1][0] = acts[0];
                        lastProposed[0][1] = lastProposed[1][1] = acts[1];
                        currentStep = 0;
                        
                        vengeful = false;
                        tsProposal = 0;
                        alreadyPickedLast = false;
                        
                        //printf("pure solution: %i %i.  ", lastProposed[0][0], lastProposed[0][1]);
                    }
                    break;
                case 99: numMessagesReceived = 0; break;
                default:
                    printf("%s\n", tkn);
                    break;
            }
        }
        //printf("\n");
        
        tkn = strtok(NULL, ";");
    }
    //printf("-------------\n\n");
    
    return secondChance;
}

int ctModel::lookUp(char ch, int index, int _A[2]) {
    if (index == 0) {
        if (ch == 'A')
            return 0;
        else if (ch == 'B')
            return 1;
        else if (ch == 'C')
            return 2;
    }
    else if (index == 1) {
        if (ch == 'X')
            return 0;
        else if (ch == 'Y')
            return 1;
        else if (ch == 'Z')
            return 2;
    }
    
    printf("(CT) unidentified action: %c (index = %i)\n", ch, index);
    
    return -1;
}