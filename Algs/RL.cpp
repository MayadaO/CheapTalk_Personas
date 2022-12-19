#include "RL.h"

RL::RL() {
    printf("incomplete RL constructor\n");
    exit(1);
}

RL::RL(int _me, MarkovGame *_mg, int _tipo) {
    me = _me;
    mg = _mg;
    tipo = _tipo;
    
    // create fps
    stages = new stageGame*[mg->numStates];
    int i;
    for (i = 0; i < mg->numStates; i++)
        stages[i] = new stageGame(me, mg->states[i], tipo);
    
    currentState = -1;
    
    vIteration(0);
    
    strcpy(nickname, "RL");
}

RL::~RL() {
    for (int i = 0; i < mg->numStates; i++)
        delete stages[i];
    delete stages;
    
    delete mg;
}

int RL::Move(GameEngine *ge) {
    ge->getValidActions(lastValidActs[0], 0);
    ge->getValidActions(lastValidActs[1], 1);
    currentState = ge->getState(mg, lastValidActs[0], lastValidActs[1])->ID;
    
    //printf("currentState: %i\n", currentState); fflush(stdout);

    //stages[currentState]->print();
    
    //printf("%i*********: ", currentState); fflush(stdout);

    int b = stages[currentState]->selectAction();
    int a = ge->convertIt(b, lastValidActs[me], NUMACTIONS*2);
    
    return a;
}

int RL::moveUpdate(GameEngine *ge, int actions[2], double dollars[2]) {
    //printf("moveUpdate ... "); fflush(stdout);
    int acts[2];
    ge->deduceActions(acts, actions, lastValidActs);
    stages[currentState]->update(acts);
    //printf("done\n"); fflush(stdout);
    
    return 0;
}

int RL::roundUpdate() {
    vIteration(0);

    return 0;
}


void RL::vIteration(int iter) {
    int s, i, j, k, jaction;
    double reward[2], Vprime[2];
    double old, tchange = 0.0;

    int act[2] = {-10, -10};
    for (s = 0; s < mg->numStates; s++) {
        for (i = 0; i < mg->states[s]->numActions[0]; i++) {
            for (j = 0; j < mg->states[s]->numActions[1]; j++) {
                reward[0] = reward[1] = 0.0;
                Vprime[0] = Vprime[1] = 0.0;
                
                jaction = mg->states[s]->jointCode(i, j);
                
                for (k = 0; k < mg->states[s]->numTrans[jaction]; k++) {
                    reward[me] = mg->states[s]->rewards[jaction][k][me];
                    Vprime[me] += stages[mg->states[s]->nextState[jaction][k]->ID]->V[me];
                    reward[1-me] = mg->states[s]->rewards[jaction][k][1-me];
                    Vprime[1-me] += stages[mg->states[s]->nextState[jaction][k]->ID]->V[1-me];
                }
                
                for (k = 0; k < 2; k++) {
                    old = stages[s]->M[k][i][j];
                    stages[s]->M[k][i][j] = reward[k] + Vprime[k];
                    tchange += fabs(stages[s]->M[k][i][j] - old);
                }
            }
        }
    }
    
    for (s = 0; s < mg->numStates; s++) {
        stages[s]->update(act);//updateV();
    }

    //printf("tchange: %.2lf\n", tchange); fflush(stdout);
    if ((tchange > 0.1) && (iter < 32))
        vIteration(iter+1);
}
