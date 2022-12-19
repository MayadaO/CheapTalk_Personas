#include "State.h"

State::State() {
    printf("incomplete state constructor\n");
    exit(1);
}

State::State(int _ID, int _numFeatures, double *_featureVector, int _numActions[2], double wvals[3]) {
    int i, j;
    
    ID = _ID;
    numFeatures = _numFeatures;
    featureVector = new double[numFeatures];
    for (i = 0; i < numFeatures; i++)
        featureVector[i] = _featureVector[i];
    
    numActions[0] = _numActions[0];
    numActions[1] = _numActions[1];
    numJointActions = numActions[0] * numActions[1];
    
    //printf("adding state w/ (%i x %i) actions\n", numActions[0], numActions[1]);

    numTrans = new int[numJointActions];
    nextState = new State**[numJointActions];
    transProb = new double*[numJointActions];
    transCount = new int*[numJointActions];
    rewards = new double**[numJointActions];
    for (i = 0; i < numJointActions; i++) {
        numTrans[i] = 0;
        nextState[i] = new State*[mTRANS];
        transProb[i] = new double[mTRANS];
        transCount[i] = new int[mTRANS];
        rewards[i] = new double*[mTRANS];
        for (j = 0; j < mTRANS; j++) {
            nextState[i][j] = NULL;
            transProb[i][j] = 0.0;
            transCount[i][j] = 0;
            rewards[i][j] = new double[2];
            rewards[i][j][0] = rewards[i][j][1] = 0.0;
        }
    }
    
    double lw = wvals[0];//0.1;
    double hw = wvals[1];//0.901;
    double inc = wvals[2];//0.1;
    numQsets = 1 + (int)(((hw - lw) / inc) + 0.00001);
    //printf("numQsets = %i\n", numQsets);
    qsets = new Qset*[numQsets];
    double w = lw;
    for (i = 0; i < numQsets; i++) {
        qsets[i] = new Qset(w, numActions);
        w += inc;
    }
    
    // value function
    cuentas = new double*[numActions[0]];
    for (i = 0; i < numActions[0]; i++) {
        cuentas[i] = new double[numActions[1]];
        for (j = 0; j < numActions[1]; j++) {
            cuentas[i][j] = 0.1;
        }
    }
    Qbr = new double[NUMACTIONS*2];
    for (i = 0; i < NUMACTIONS*2; i++) {
        Qbr[i] = 0.0;
    }
    Vbr = 0.0;
    Vsarsa = 0.0;
    
    cuentas_real = new double*[numActions[0]];
    for (i = 0; i < numActions[0]; i++) {
        cuentas_real[i] = new double[numActions[1]];
        for (j = 0; j < numActions[1]; j++) {
            cuentas_real[i][j] = 0.1;
        }
    }
    Qreal = new double**[numActions[0]];
    for (i = 0; i < numActions[0]; i++) {
        Qreal[i] = new double*[numActions[1]];
        for (j = 0; j < numActions[1]; j++) {
            Qreal[i][j] = new double[2];
            Qreal[i][j][0] = 0.0;
            Qreal[i][j][1] = 0.0;
        }
    }
    Vreal[0] = Vreal[1] = 0.0;
    //printf("done\n"); fflush(stdout);

/*    // long
    int m, n, o, p;
    for (m = 0; m < nRBINS; m++) {
        for (n = 0; n < nRBINS; n++) {
            for (o = 0; o < nRBINS; o++) {
                for (p = 0; p < nRBINS; p++) {
                    cuentasLong[m][n][o][p] = new double*[numActions[0]];
                    for (i = 0; i < numActions[0]; i++) {
                        cuentasLong[m][n][o][p][i] = new double[numActions[1]];
                        for (j = 0; j < numActions[1]; j++) {
                            cuentasLong[m][n][o][p][i][j] = 0.1;
                        }
                    }
                    QbrLong[m][n][o][p] = new double[NUMACTIONS*2];
                    for (i = 0; i < NUMACTIONS*2; i++)
                        QbrLong[m][n][o][p][i] = 0.0;
                    VbrLong[m][n][o][p] = 0.0;
                }
            }
        }
    }
*/    
    // minimax stuff
    mm = new minimaxLog*[2];
    mm[0] = new minimaxLog(numActions, 0);
    mm[1] = new minimaxLog(numActions, 1);
    
    attack = new minimaxLog*[2];
    attack[0] = new minimaxLog(numActions, 0);
    attack[1] = new minimaxLog(numActions, 1);
}

State::~State() {
    int i, j;
    
    for (i = 0; i < numJointActions; i++) {
        for (j = 0; j < mTRANS; j++) {
            delete rewards[i][j];
        }
        delete rewards[i];
        delete transCount[i];
        delete transProb[i];
        delete nextState[i];
    }
    
    delete rewards;
    delete transCount;
    delete transProb;
    delete nextState;
    delete featureVector;
    delete numTrans;
    
    for (i = 0; i < numQsets; i++) {
        delete qsets[i];
    }
    
    delete qsets;
    
    delete mm[0];
    delete mm[1];
    delete mm;
    
    delete attack[0];
    delete attack[1];
    delete attack;
    
    for (i = 0; i < numActions[0]; i++) {
        delete cuentas[i];
    }
    delete cuentas;
    delete Qbr;
    
    for (i = 0; i < numActions[0]; i++) {
        delete cuentas_real[i];
    }
    delete cuentas_real;
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++) {
            delete Qreal[i][j];
        }
        delete Qreal[i];
    }
    delete Qreal;
    
/*    int m, n, o, p;
    for (m = 0; m < nRBINS; m++) {
        for (n = 0; n < nRBINS; n++) {
            for (o = 0; o < nRBINS; o++) {
                for (p = 0; p < nRBINS; p++) {
                    for (i = 0; i < numActions[0]; i++) {
                        delete cuentasLong[m][n][o][p][i];
                    }
                    delete cuentasLong[m][n][o][p];
                    delete QbrLong[m][n][o][p];
                }
            }
        }
    }*/
}

void State::addTransition(State *_sprime, int _actions[2], double _rewards[2]) {
    //printf("adding: s%i, (%i, %i) -> s%i (%.2lf, %.2lf)\n", ID, _actions[0], _actions[1], _sprime->ID, _rewards[0], _rewards[1]);
    
    int jaction = jointCode(_actions);
    int i;
    //printf("jaction = %i\n", jaction);
    
    for (i = 0; i < numTrans[jaction]; i++) {
        if (nextState[jaction][i] != NULL) {
            if (nextState[jaction][i]->ID == _sprime->ID) {
                transCount[jaction][i] ++;
                rewards[jaction][i][0] += _rewards[0];
                rewards[jaction][i][1] += _rewards[1];
                
                return;
            }
        }
    }

    nextState[jaction][numTrans[jaction]] = _sprime;
    transCount[jaction][numTrans[jaction]] = 1;
    rewards[jaction][numTrans[jaction]][0] += _rewards[0];
    rewards[jaction][numTrans[jaction]][1] += _rewards[1];
    numTrans[jaction] ++;
}

int State::jointCode(int _actions[2]) {
    return _actions[0] * numActions[1] + _actions[1];
}

int State::jointCode(int a0, int a1) {
    return a0 * numActions[1] + a1;
}

bool State::sameState(double *fvector) {
    int i;
    
//    if ((ID == 99) && (oid == 106)) {
//        for (i = 0; i < numFeatures; i++) {
//            if (fabs(fvector[i] - featureVector[i]) > 0.00001) {
//                printf("%i: %lf vs %lf\n", i, fvector[i], featureVector[i]);
//                return false;
//            }
//        }
//    }
//    else {
        for (i = 0; i < numFeatures; i++) {
            if (fabs(fvector[i] - featureVector[i]) > 0.00001)
                return false;
        }
//    }
    
    return true;
}

void State::setV(int qst) {
    int i, j;
    double mx = -99999;
    
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++) {
            if (qsets[qst]->Qs[i][j] > mx)
                mx = qsets[qst]->Qs[i][j];
        }
    }
    
    qsets[qst]->V = mx;
}

double State::maxM(int ind, int qst) {
    int i, j;
    double mx = -99999, mx2;
    
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++) {
            if (qsets[qst]->Qs[i][j] > mx) {
                mx = qsets[qst]->Qs[i][j];
                mx2 = qsets[qst]->M[i][j][ind];
            }
        }
    }
    
    return mx2;
}

double State::actualM(int ind) {
    int i, j;
    double mag = 0.0;
    
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++) {
            mag += cuentas_real[i][j];
        }
    }
    
    double val = 0.0;
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++) {
            val += Qreal[i][j][ind] * (cuentas_real[i][j] / mag);
        }
    }
    
    return val;
}

double State::maxM(int ind) {
    int i, j;
    double mx = -99999;
    double prob[NUMACTIONS*2];
    
    getProbs_real(prob, ind);
    
    double v;
    for (i = 0; i < numActions[ind]; i++) {
        v = 0.0;
        for (j = 0; j < numActions[1-ind]; j++) {
            if (ind == 0)
                v += Qreal[i][j][ind] * prob[j];
            else
                v += Qreal[j][i][ind] * prob[j];
        }
        if (v > mx)
            mx = v;
    }

    return mx;
}

void State::getProbs_regular(double prob[NUMACTIONS*2], int ind) {
    int i, j;
    
    double mag = 0.0;
    //printf("cuentas: ");
    for (i = 0; i < numActions[1-ind]; i++) {
        prob[i] = 0.0;
        for (j = 0; j < numActions[ind]; j++) {
            if (ind == 0)
                prob[i] += cuentas[j][i];
            else
                prob[i] += cuentas[i][j];
        }
        mag += prob[i];
        //printf("%.2lf ", prob[i]);
    }
    //printf("\n");
    
    for (i = 0; i < numActions[1-ind]; i++)
        prob[i] /= mag;
}

void State::getProbs_real(double prob[NUMACTIONS*2], int ind) {
    int i, j;
    
    double mag = 0.0;
    //printf("cuentas: ");
    for (i = 0; i < numActions[1-ind]; i++) {
        prob[i] = 0.0;
        for (j = 0; j < numActions[ind]; j++) {
            if (ind == 0)
                prob[i] += cuentas_real[j][i];
            else
                prob[i] += cuentas_real[i][j];
        }
        mag += prob[i];
        //printf("%.2lf ", prob[i]);
    }
    //printf("\n");
    
    for (i = 0; i < numActions[1-ind]; i++)
        prob[i] /= mag;
}


int State::findQset(double _w) {
    int i;
    
    for (i = 0; i < numQsets; i++) {
        if (qsets[i]->w == _w)
            return i;
    }
    
    printf("problem: didn't find qset\n");
    exit(1);
    
    return -1;
}

void State::printFeatureVector() {
    int i;

    printf("v for %i: ", ID);
    for (i = 0; i < numFeatures; i++)
        printf("%.2lf ", featureVector[i]);
    printf("\n");
}
