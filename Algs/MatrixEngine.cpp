#include "MatrixEngine.h"

MatrixEngine::MatrixEngine() {
    moveTime = true;
    numAgents = 2;
    actions = new int[numAgents];
}

MatrixEngine::~MatrixEngine() {
    delete actions;
}

State *MatrixEngine::getState(MarkovGame *mg, bool *validActs1, bool *validActs2) {
    s = mg->states[0];
    
    if (moveTime) {
        return mg->states[0];
    }
    else {
        return mg->states[1];
    }
}

void MatrixEngine::MoveAgents() {
    moveTime = false;
}

void MatrixEngine::initRound() {
    moveTime = true;
}

bool MatrixEngine::OnGoal(int index) {
    if (!moveTime)
        return true;
    
    return false;
}

void MatrixEngine::getValidActions(bool *validActs, int index) {
    int i;
    
    if (moveTime) {
        for (i = 0; i < 5; i++)
            validActs[i] = true;
    }
    else {
        for (i = 0; i < 5; i++)
            validActs[i] = false;
    }
}

double MatrixEngine::getCurrentReward(int index) {
    //printf("reward for %i (%i, %i): %lf\n", index, actions[0], actions[1], s->rewards[index][actions[0]][actions[1]]);
    int ja = actions[0] * s->numActions[1] + actions[1];

    return s->rewards[ja][0][index];
}

int MatrixEngine::convertIt(int accion, bool *validActions, int numActs) {
    return accion;
}

void MatrixEngine::deduceActions(int rawActions[2], int geActions[2], bool lastValidActs[2][2*NUMACTIONS]) {
    rawActions[0] = geActions[0];
    rawActions[1] = geActions[1];    
}
