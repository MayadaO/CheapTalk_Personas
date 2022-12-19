#include "Xprt.h"

Xprt::Xprt() {
    printf("incomplete Xprt constructor\n");
}

Xprt::Xprt(int _me, MarkovGame *_mg, char tipito[1024], bool _utilitarian) {
    me = _me;
    comms = new CommAgent(NONE_TIPO, 4000, me);

    strcpy(nickname, tipito);
    
    expertID = 0;
    ab = new mega(_mg, me, true, comms, 0.99, _utilitarian);
    if (!strcmp(tipito, "maxmin")) {
        ab->computeMaxminExpert(0.99);
    }
    else if ((!strcmp(tipito, "mbrl")) || (!strcmp(tipito, "umbrl"))) {
        ab->computeMBRLExpert(0.99, true);
    }
    else if (!strcmp(tipito, "folk")) {
        ab->computeFolkEgalExpert(0.99, true);
        if (ab->xprts[0]->culpable) {
            printf("que pasa????\n");
            exit(1);
        }
            
    }
    else if (!strcmp(tipito, "bully")) {
        ab->computeBullyExpert(0.99, true);
    }
    else if (!strcmp(tipito, "coop")) {
        printf("********here\n"); fflush(stdout);
        ab->computeFolkEgalExpert(0.99, false);
    }
    else if (!strcmp(tipito, "poop")) {
        ab->computeBullyExpert(0.99, false);
    }
    else {
        printf("expert NOT found\n");
        exit(1);
    }
    
    ab->determineHighLevelActions();
    
    lastState = NULL;
    rPayout[0] = rPayout[1] = 0.0;
    previousPayoffs[0] = previousPayoffs[1] = -99999;
    
    ab->xprts[expertID]->reset(previousPayoffs, me);
}

Xprt::~Xprt() {
    delete ab;
    delete comms;
}

int Xprt::Move(GameEngine *ge) {//bool validActions[NUMACTIONS], bool _onGoal, bool _verbose) {
    ge->getValidActions(lastValidActs[0], 0);
    ge->getValidActions(lastValidActs[1], 1);
    lastState = ge->getState(ab->mg, lastValidActs[0], lastValidActs[1]);
    //printf("<%i: %i> ... ", me, lastState->ID); fflush(stdout);
    
    int b = ab->xprts[expertID]->Move(lastState, previousPayoffs, me);
    int a = ge->convertIt(b, lastValidActs[me], NUMACTIONS*2);

    //printf("(%i<%i>: %i -> %i)   ", me, lastState->ID, b, a);
    
    corA = -1;
    if ((ab->xprts[expertID]->tipo == FOLLOWER) || (ab->xprts[expertID]->tipo == LEADER) || (ab->xprts[expertID]->tipo == LEADER2)) {
        //printf("%i,", ab->xprts[expertID]->currentStep); fflush(stdout);
        //printf("%i: ", ab->xprts[expertID]->strat[ab->xprts[expertID]->currentStep]);
        int c = lastState->qsets[ab->xprts[expertID]->strat[ab->xprts[expertID]->currentStep]]->correctA(1-me);
        //printf("%i^", c); fflush(stdout);
        corA = ge->convertIt(c, lastValidActs[me], NUMACTIONS*2);
    }
    
    //printf("cA%i ... ", corA);
    
    return a;
}

//ge->actions, ge->getCurrentReward(j),
int Xprt::moveUpdate(GameEngine *ge, int actions[2], double dollars[2]) {
    int acts[2];
    ge->deduceActions(acts, actions, lastValidActs);
    
    //printf("f"); fflush(stdout);
    
    rPayout[0] += dollars[0];
    rPayout[1] += dollars[1];
    
    ge->getValidActions(lastValidActs[0], 0);
    ge->getValidActions(lastValidActs[1], 1);
    
    //printf("o"); fflush(stdout);
    
    State* sprime = ge->getState(ab->mg, lastValidActs[0], lastValidActs[1]);

    //printf("l<%i>", corA); fflush(stdout);
    
    //printf("need to update expert aspiration??????\n"); fflush(stdout);
    ab->moveUpdate(lastState, sprime, acts, expertID, rPayout, ab->xprts[expertID]->barR[me], corA);
    
    //printf("k"); fflush(stdout);
    
    return 0;
}

int Xprt::roundUpdate() {
    //printf("updating the round\n"); fflush(stdout);
    
    ab->roundUpdate(rPayout);
    
    //printf("Rresult (%i): %.2lf, %.2lf\n", me, rPayout[0], rPayout[1]);
    if ((ab->xprts[expertID]->tipo == FOLLOWER) || (ab->xprts[expertID]->tipo == LEADER)) {
        bool heldTrue = false;
        if ((ab->xprts[expertID]->tipo == FOLLOWER) || (ab->xprts[expertID]->tipo == LEADER)) {
            if (ab->lastActions[1-me][ab->xprts[expertID]->strat[ab->xprts[expertID]->currentStep]+2] == 1) {
                //printf("held true\n");
                heldTrue = true;
            }
        }
    }
    
    //printf("Xprt: do I need to update the aspiration level????\n");
    //printf("Xprt: i need to at least update the `heldTrue' value I think\n");
    
    bool heldTrue = false;
    if ((ab->xprts[expertID]->tipo == FOLLOWER) || (ab->xprts[expertID]->tipo == LEADER) || (ab->xprts[expertID]->tipo == LEADER2)) {
        if (ab->lastActions[1-me][ab->xprts[expertID]->strat[ab->xprts[expertID]->currentStep]+2] == 1) {
            //printf("-held true-\n");
            heldTrue = true;
        }
    }
    double asp = ab->xprts[expertID]->barR[me];
    ab->xprts[expertID]->Update(rPayout, me, 100.0, heldTrue, asp);

    
    previousPayoffs[0] = rPayout[0];
    previousPayoffs[1] = rPayout[1];
    rPayout[0] = rPayout[1] = 0.0;

    return 0;
}

