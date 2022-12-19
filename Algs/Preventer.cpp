#include "Preventer.h"


Preventer::Preventer() {
    printf("incomplete Preventer constructor\n");
    exit(1);
}

Preventer::Preventer(int _me, MarkovGame *_mg, double _myWeight, bool _originalMG) {
    me = _me;
    mg = _mg;
    myWeight = _myWeight;
    originalMG = _originalMG;
    
    rPayouts[0] = rPayouts[1] = 0.0;
    
    //printf("going to compute reals ... "); fflush(stdout);
    printf("initializing the preventer ... ");
    initQreals();
    mg->computeReal(true);
    //printf("done\n"); fflush(stdout);
    
    //printf("adjusting prior model of the associate ... "); fflush(stdout);
    adjustPriors();
    //printf("done\n"); fflush(stdout);
    
    initQreals();
    mg->computePrevent(0);//me, myWeight);
    simulateVsMBRL(me, 5);
    printf("done\n");
}

Preventer::~Preventer() {

    if (originalMG)
        delete mg;
}

int Preventer::Move(GameEngine *ge) {
    ge->getValidActions(lastValidActs[0], 0);
    ge->getValidActions(lastValidActs[1], 1);
    currentState = ge->getState(mg, lastValidActs[0], lastValidActs[1]);
    
    int b = selectAction(currentState);
    
    int a = ge->convertIt(b, lastValidActs[me], NUMACTIONS*2);
    
    //printf("selected %i (%i)\n\n", b, a);
    
    return a;
}

int Preventer::mover(State *s) {
    double probs[NUMACTIONS*2];
    
    currentState = s;
    
    currentState->getProbs_real(probs, me);
    
    return selectAction(currentState);
}

// intent must be to (1) discourage defection and (2) encourage cooperation; not necessarily to equalize payoffs
int Preventer::selectAction(State *s) {
    double probs[NUMACTIONS*2];

    s->getProbs_real(probs, me);
    
    double vals[NUMACTIONS*2];
    double maxs[2][NUMACTIONS*2];
    int i, j;
    for (i = 0; i < s->numActions[me]; i++) {
        vals[i] = rPayouts[0] - rPayouts[1];
        maxs[0][i] = maxs[1][i] = 0.0;
        for (j = 0; j < s->numActions[1-me]; j++) {
            if (me == 0) {
                vals[i] += probs[j] * (s->Qreal[i][j][0] - s->Qreal[i][j][1]);
                maxs[0][i] += probs[j] * s->Qreal[i][j][me];
                maxs[1][i] += probs[j] * s->Qreal[i][j][1-me];
            }
            else {
                vals[i] += probs[j] * (s->Qreal[j][i][0] - s->Qreal[j][i][1]);
                maxs[0][i] += probs[j] * s->Qreal[j][i][me];
                maxs[1][i] += probs[j] * s->Qreal[j][i][1-me];
            }
        }
    }
    
    int b = 0;
    for (i = 1; i < s->numActions[me]; i++) {
        if ((fabs(vals[i]) < fabs(vals[b])) || ((fabs(vals[i]) == fabs(vals[b])) && (rand() % 2)))
            b = i;
    }
    
    return b;
}

int Preventer::moveUpdate(GameEngine *ge, int actions[2], double dollars[2]) {
    int acts[2];
    ge->deduceActions(acts, actions,lastValidActs);
    
    int i, j;
    
    currentState->cuentas_real[acts[0]][acts[1]] += 1.0;

    for (j = 0; j < 2; j++)
        rPayouts[j] += dollars[j];
    
    return 0;
}

int Preventer::moverUpdate(int acts[2], double dollars[2]) {
    currentState->cuentas_real[acts[0]][acts[1]] += 1.0;

    int j;
    for (j = 0; j < 2; j++)
        rPayouts[j] += dollars[j];
    
    return 0;
}

int Preventer::roundUpdate() {
    //myWeight *= 0.9;
    //printf("myWeight = %.2lf\n", myWeight);
    
    mg->computePrevent(0);//me, myWeight);
    //mg->computeBR_SARSA(me);
    rPayouts[0] = rPayouts[1] = 0.0;

    return 0;
}

int Preventer::rounderUpdate() {
    //myWeight *= 0.9;
    //printf("myWeight = %.2lf\n", myWeight);
    
    mg->computePrevent(0);//me, myWeight);
    //mg->computeBR_SARSA(me);
    rPayouts[0] = rPayouts[1] = 0.0;
    
    return 0;
}
/*
double Preventer::getPreventativeValue(double *probs, int ind, int a) {
    int j;
    double v[2] = {rPayouts[0], rPayouts[1]};
    
    for (j = 0; j < currentState->numActions[1-ind]; j++) {
        if (ind == 0) {
            v[0] += currentState->Qreal[a][j][0] * probs[j];
            v[1] += currentState->Qreal[a][j][1] * probs[j];
        }
        else {
            v[0] += currentState->Qreal[j][a][0] * probs[j];
            v[1] += currentState->Qreal[j][a][1] * probs[j];
        }
    }
    
    double rmax = 22.0;
    
    //if (me == 0)
    //   printf("%i: (mine: %.2lf; fair: %.2lf)\n", a, (v[me]-rPayouts[me]) * myWeight, (rmax - fabs(v[0] - v[1])));
    
    //double val = ((rmax + 1.0) / (fabs(v[0] - v[1]) + 1.0)) + ((v[me]-rPayouts[me]) * myWeight);
    double val = (rmax - fabs(v[0] - v[1])) + ((v[me]-rPayouts[ind]) * myWeight);
    
    return val;
}*/

void Preventer::adjustPriors() {
    int s, a, j, br;
    double probs[NUMACTIONS*2];
    double v[NUMACTIONS*2];
    double mag, low, high;
    int highCount;
    
    double pwrVal = 1;
    
    for (s = 0; s < mg->numStates; s++) {
        // find out what a myopic him will do
        mg->states[s]->getProbs_real(probs, 1-me);
        
        mag = 0.0;
        low = 99999;
        high = -99999;
        highCount = 0;
        for (a = 0; a < mg->states[s]->numActions[1-me]; a++) {
            v[a] = 0.0;
            for (j = 0; j < mg->states[s]->numActions[me]; j++) {
                if (me == 1) {
                    v[a] += mg->states[s]->Qreal[a][j][1-me] * probs[j];
                }
                else {
                    v[a] += mg->states[s]->Qreal[j][a][1-me] * probs[j];
                }
            }
            //if (v[a] < low)
            //    low = v[a];
            //mag += v[a];//pow(v[a], pwrVal);
            
            if ((high+0.00001) < v[a]) {
                highCount = 1;
                high = v[a];
            }
            else if (fabs(high - v[a]) < 0.00001)
                highCount ++;
        }
        //printf("high = %lf; highCount = %i\n", high, highCount);
        
/*        for (a = 0; a < mg->states[s]->numActions[1-me]; a++) {
            v[a] -= low;
            mag -= low;
        }
        
        if (mag > 0.0001) {
            for (a = 0; a < mg->states[s]->numActions[1-me]; a++)
                v[a] /= mag;
        }
        else {
            for (a = 0; a < mg->states[s]->numActions[1-me]; a++)
                v[a] = 1.0 / mg->states[s]->numActions[1-me];
        }*/
        
        for (a = 0; a < mg->states[s]->numActions[1-me]; a++) {
            if (fabs(v[a] - high) < 0.00001)
                v[a] = 1.0 / highCount;
            else
                v[a] = 0.0;
        }
        
        br = getBestResponse(mg->states[s], v);
        
        for (a = 0; a < mg->states[s]->numActions[1-me]; a++) {
            for (j = 0; j < mg->states[s]->numActions[me]; j++) {
                if (me == 1)
                    mg->states[s]->cuentas_real[a][j] = 0.0;
                else
                    mg->states[s]->cuentas_real[j][a] = 0.0;
            }
            
            if (me == 1)
                mg->states[s]->cuentas_real[a][br] = v[a];
            else
                mg->states[s]->cuentas_real[br][a] = v[a];
        }
    }
}

int Preventer::getBestResponse(State *s, double probs[NUMACTIONS*2]) {
    int a, j, best = -1;
    double v[NUMACTIONS*2], mx = -99999;
    
    for (a = 0; a < s->numActions[me]; a++) {
        v[a] = 0.0;
        for (j = 0; j < s->numActions[1-me]; j++) {
            if (me == 0)
                v[a] += probs[j] * s->Qreal[a][j][me];
            else
                v[a] += probs[j] * s->Qreal[j][a][me];
        }
        
        if (v[a] > mx) {
            mx = v[a];
            best = a;
        }
    }
    
    if (best < 0) {
        printf("we have a problem\n");
        printf("probs: ");
        for (j = 0; j < s->numActions[me]; j++)
            printf("%.2lf ", probs[j]);
        printf("\n");
        printf("v: ");
        for (j = 0; j < s->numActions[me]; j++)
            printf("%.2lf ", v[j]);
        printf("\n");
        //printf("vs %lf\n", s->maxM(me));
        exit(1);
    }
    
    return best;
}

void Preventer::initQreals() {
    int s, i, j;
    
    for (s = 0; s < mg->numStates; s++) {
        for (i = 0; i < mg->states[s]->numActions[0]; i++) {
            for (j = 0; j < mg->states[s]->numActions[1]; j++) {
                mg->states[s]->Qreal[i][j][0] = mg->states[s]->Qreal[i][j][1] = 0.0;
            }
        }
        mg->states[s]->Vreal[0] = mg->states[s]->Vreal[1] = 0.0;
    }
}

void Preventer::simulateVsMBRL(int ind, int rounds) {
    int s, p, m, i, j, k;
    double probs[NUMACTIONS*2];
    double vals[NUMACTIONS*2];
    
    for (k = 0; k < rounds; k++) {
        for (s = 0; s < mg->numStates; s++) {
            //printf("%i: ", s); fflush(stdout);
            if (isGoalState(s))
                continue;
            
            // what is the preventer's move (player ind)
            p = selectAction(mg->states[s]);
            //printf("%i, ", p); fflush(stdout);
            // what is the MBRL response?
            mg->states[s]->getProbs_real(probs, 1-ind);
            m = -1;
            for (i = 0; i < mg->states[s]->numActions[1-ind]; i++) {
                vals[i] = 0.0;
                for (j = 0; j < mg->states[s]->numActions[ind]; j++) {
                    if (ind == 1) {
                        vals[i] += probs[j] * mg->states[s]->Qreal[i][j][1-ind];
                    }
                    else {
                        vals[i] += probs[j] * mg->states[s]->Qreal[j][i][1-ind];
                    }
                }
                if (m == -1)
                    m = i;
                else {
                    if (vals[m] < vals[i])
                        m = i;
                    //else if ((vals[m] == vals[i]) && (rand() % 2))
                    //    m = i;
                }
            }
            //printf("%i\n", m); fflush(stdout);
            
            if (ind == 0)
                mg->states[s]->cuentas_real[p][m] ++;
            else
                mg->states[s]->cuentas_real[m][p] ++;
        }
        
        mg->computePrevent(0);
    }
    
    // renormalize
    for (s = 0; s < mg->numStates; s++) {
        for (i = 0; i < mg->states[s]->numActions[0]; i++) {
            for (j = 0; j < mg->states[s]->numActions[1]; j++) {
                mg->states[s]->cuentas_real[i][j] /= (double)rounds;
            }
        }
    }
    
}

bool Preventer::isGoalState(int s) {
    int i;
    for (i = 0; i < mg->numGoalStates; i++) {
        if (s == mg->goalStates[i]->ID)
            return true;
    }
    
    return false;
}

