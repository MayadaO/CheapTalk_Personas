#include "MarkovGame.h"
//#include "GameEngine.h"

MarkovGame::MarkovGame() {
    numStates = 0;
    numStartStates = 0;
    numGoalStates = 0;
    states = new State*[mSTATES];
    startStates = new State*[mSTART_STATES];
    goalStates = new State*[mGOAL_STATES];

    int dvdr = 3 + rand() % 3;
    wvals[0] = 0.05;     // low
    wvals[1] = 0.95;   // high
    wvals[2] = (wvals[1] - wvals[0]) / dvdr;     // inc
}

MarkovGame::MarkovGame(char *fname) {
    numStates = 0;
    numStartStates = 0;
    numGoalStates = 0;
    states = new State*[mSTATES];
    startStates = new State*[mSTART_STATES];
    goalStates = new State*[mGOAL_STATES];
    
    //printf("going to load the Markov Game\n"); fflush(stdout);
    
    int dvdr = 4 + rand() % 3;//3+(1-_me);//
    wvals[0] = 0.05 + 0.01 * (rand() % 11);     // low
    wvals[1] = 0.95 - 0.01 * (rand() % 11);   // high
    wvals[2] = (wvals[1] - wvals[0]) / dvdr;     // inc
    
    //printf("about to load the game\n"); fflush(stdout);
    
    loadMG(fname);
/*
    int i, j;
    int ja = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("(%.2lf, %.2lf)\t", states[0]->rewards[ja][0][0], states[0]->rewards[ja][0][1]);
            ja ++;
        }
        printf("\n");
    }
*/    
    //printf("finished loading the Markov Game\n"); fflush(stdout);
}

MarkovGame::~MarkovGame() {
    int i;
    
    for (i = 0; i < numStates; i++) {
        delete states[i];
    }
    delete states;
    
    delete startStates;
    delete goalStates;
}

void MarkovGame::loadMG(char *fname) {
    FILE *fp = fopen(fname, "r");
    char buf[1024], buf2[1024];
    int nStates = 0;
    
    fscanf(fp, "%s %i", buf, &nStates);
    //printf("game has %i states\n", nStates);
    
    int nfeatures;
    fscanf(fp, "%s %i", buf, &nfeatures);
    
    // read in all of the states
    int i, j, ID, nacts[2];
    double *fvector = new double[nfeatures];
    for (i = 0; i < nStates; i++) {
        fscanf(fp, "%i", &ID);
        for (j = 0; j < nfeatures; j++) {
            fscanf(fp, "%lf", &(fvector[j]));
        }
        fscanf(fp, "%i %i", &(nacts[0]), &(nacts[1]));
        
        getState(nfeatures, fvector, nacts);
    }
    
    //printf("read states\n"); fflush(stdout);
    
    // read in the start states
    int nStart, ind;
    fscanf(fp, "%s %s %i", buf, buf2, &nStart);
    for (i = 0; i < nStart; i++) {
        fscanf(fp, "%i", &ind);
        //printf("start: %i\n", ind); fflush(stdout);
        addStartState(states[ind]);
    }
    
    //printf("read start states\n"); fflush(stdout);
    
    // read in the goal states
    int nGoal;
    fscanf(fp, "%s %s %i", buf, buf2, &nGoal);
    for (i = 0; i < nGoal; i++) {
        fscanf(fp, "%i", &ind);
        //printf("goal: %i\n", ind); fflush(stdout);
        addGoalState(states[ind]);
    }
    
    //printf("read goal states\n"); fflush(stdout);
    
    fscanf(fp, "%s", buf); // "actionLabels:"
    fscanf(fp, "%s", actionDescription);
    fscanf(fp, "%i", &numLabels);
    for (i = 0; i < numLabels; i++) {
        fscanf(fp, "%s", actionLabels[i]);
        //printf("%s\n", actionLabels[i]);
        
        for (j = 0; j < strlen(actionLabels[i]); j++) {
            if (actionLabels[i][j] == '_')
                actionLabels[i][j] = ' ';
        }
    }
    
    //printf("read labels\n"); fflush(stdout);
    
    // read in the state transitions
    int num, nuevo;
    double dollars[2];
    fscanf(fp, "%s %s", buf, buf2);
    while (true) {
        num = fscanf(fp, "%i %i %i", &ID, &(nacts[0]), &(nacts[1]));
        //printf("%i-", num); fflush(stdout);
        if (num < 0)
            break;
        
        fscanf(fp, "%s %i %lf %lf %s", buf, &nuevo, &(dollars[0]), &(dollars[1]), buf2);
        
        states[ID]->addTransition(states[nuevo], nacts, dollars);
    }
    
    //printf("read transitions\n"); fflush(stdout);
    
    delete[] fvector;
    
    fclose(fp);
}

State *MarkovGame::getState(int _numFeatures, double *_featureVector, int _numActions[2]) {
    //printf("enterin' ... "); fflush(stdout);
    State *s = stateDefined(_featureVector);
    //printf("stateDefined ... "); fflush(stdout);
    if (s == NULL) {
        states[numStates] = new State(numStates, _numFeatures, _featureVector, _numActions, wvals);
        numStates ++;
    
        //printf("numStates = %i\n", numStates);
        //printf("add newone\n"); fflush(stdout);
        
        return states[numStates-1];
    }

    //printf("state %i already exists\n", s->ID); fflush(stdout);
    
    return s;
}

State *MarkovGame::stateDefined(double *_featureVector) {
    int i;
    
    for (i = 0; i < numStates; i++) {
        if (states[i]->sameState(_featureVector)) {
            return states[i];
        }
    }
    
    return NULL;
}

void MarkovGame::addStartState(State *s) {
    int i;
    
    for (i = 0; i < numStartStates; i++) {
        if (s->ID == states[startStates[i]->ID]->ID)
            return;
    }
    
    startStates[numStartStates] = s;
    numStartStates ++;
}

void MarkovGame::addGoalState(State *s) {
    int i;
    
    //printf("in addGoalStates: %i\n", numGoalStates); fflush(stdout);
    
    for (i = 0; i < numGoalStates; i++) {
        //printf("gID = %i\n", goalStates[i]->ID); fflush(stdout);
        if (s->ID == states[goalStates[i]->ID]->ID)
            return;
    }
    
    //printf("numGoalsStates: %i\n", numGoalStates); fflush(stdout);
    
    goalStates[numGoalStates] = s;
    numGoalStates ++;
}

void MarkovGame::writeGame(char *filename) {
    FILE *fp = fopen(filename, "w");
    int i, j, k, m;
    
    fprintf(fp, "numStates: %i\nnumFeatures: %i\n", numStates, states[0]->numFeatures);
    for (i = 0; i < numStates; i++) {
        fprintf(fp, "%i \t", states[i]->ID);
        for (j = 0; j < states[i]->numFeatures; j++) {
            fprintf(fp, "%.3lf\t", states[i]->featureVector[j]);
        }
        fprintf(fp, "%i\t%i\n", states[i]->numActions[0], states[i]->numActions[1]);
    }
    fprintf(fp, "\nStart states: %i\n", numStartStates);
    for (i = 0; i < numStartStates; i++)
        fprintf(fp, "%i\n", startStates[i]->ID);
    fprintf(fp, "\nGoal states: %i\n", numGoalStates);
    for (i = 0; i < numGoalStates; i++)
        fprintf(fp, "%i\n", goalStates[i]->ID);
    
    fprintf(fp, "\nState/reward transitions\n");
    int jointAct;
    for (i = 0; i < numStates; i++) {
        for (j = 0; j < states[i]->numActions[0]; j++) {
            for (k = 0; k < states[i]->numActions[1]; k++) {
                jointAct = states[i]->jointCode(j, k);
                for (m = 0; m < states[i]->numTrans[jointAct]; m++) {
                    if (states[i]->nextState[jointAct][m] != NULL)
                        fprintf(fp, "%i %i %i -> %i %.1lf %.1lf (%i)\n", states[i]->ID, j, k, states[i]->nextState[jointAct][m]->ID, states[i]->rewards[jointAct][m][0] / states[i]->transCount[jointAct][m], states[i]->rewards[jointAct][m][1] / states[i]->transCount[jointAct][m], states[i]->transCount[jointAct][m]);
                }
            }
        }
    }
    
    fclose(fp);
}


void MarkovGame::vIteration(double w1, double w2) {
    int s, i, j, k, jaction;
    double reward, Vprime;
    double r[2], Vp[2];
    double old, tchange = 0.0;

    int qind = states[0]->findQset(w1);
    
    for (s = 0; s < numStates; s++) {
        
        for (i = 0; i < states[s]->numActions[0]; i++) {
            for (j = 0; j < states[s]->numActions[1]; j++) {
                reward = 0.0;
                Vprime = 0.0;
                
                r[0] = r[1] = Vp[0] = Vp[1] = 0.0;
                
                jaction = states[s]->jointCode(i, j);
                
                for (k = 0; k < states[s]->numTrans[jaction]; k++) {
                    reward = w1 * states[s]->rewards[jaction][k][0] + w2 * states[s]->rewards[jaction][k][1];
                    Vprime += states[s]->nextState[jaction][k]->qsets[qind]->V;
                    
                    r[0] = states[s]->rewards[jaction][k][0];
                    r[1] = states[s]->rewards[jaction][k][1];
                    Vp[0] = states[s]->nextState[jaction][k]->maxM(0, qind);
                    Vp[1] = states[s]->nextState[jaction][k]->maxM(1, qind);
                }
                //reward -= w1 * states[s]->mm[0]->mv + w2 * states[s]->mm[1]->mv;
                
                old = states[s]->qsets[qind]->Qs[i][j];
                states[s]->qsets[qind]->Qs[i][j] = reward + Vprime;
                states[s]->qsets[qind]->M[i][j][0] = r[0] + Vp[0];
                states[s]->qsets[qind]->M[i][j][1] = r[1] + Vp[1];
                tchange += fabs(states[s]->qsets[qind]->Qs[i][j] - old);
            }
        }
    }
    
    for (s = 0; s < numStates; s++) {
        states[s]->setV(qind);
    }
    
    //printf("tchange: %.2lf\n", tchange);
    if (tchange > 0.01)
        vIteration(w1, w2);
}

void MarkovGame::solveZero(int ind) {
    int s, i, j, k, jaction;
    double reward, Vprime;
    double old, tchange = 0.0;
    
    for (s = 0; s < numStates; s++) {
        for (i = 0; i < states[s]->numActions[0]; i++) {
            for (j = 0; j < states[s]->numActions[1]; j++) {
                reward = 0.0;
                Vprime = 0.0;
                jaction = states[s]->jointCode(i, j);
                
                for (k = 0; k < states[s]->numTrans[jaction]; k++) {
                    reward += states[s]->rewards[jaction][k][ind];
                    Vprime += states[s]->nextState[jaction][k]->qsets[0]->V;
                }
                
                old = states[s]->qsets[0]->Qs[i][j];
                states[s]->qsets[0]->Qs[i][j] = reward + Vprime;
                tchange += fabs(states[s]->qsets[0]->Qs[i][j] - old);
            }
        }
    }
    
    for (s = 0; s < numStates; s++) {
        //printf("%i, %i\n", states[s]->numActions[0], states[s]->numActions[1]); fflush(stdout);
        states[s]->mm[ind]->getMinimax(states[s]->numActions, ind, states[s]->qsets[0]->Qs);
        states[s]->qsets[0]->V = states[s]->mm[ind]->mv;
    }
    
    //printf("tchange: %.2lf\n", tchange);
    if (tchange > 0.01)
        solveZero(ind);
}

void MarkovGame::solveAttack(int ind) {
    int s, i, j, k, jaction;
    double reward, Vprime;
    double old, tchange = 0.0;
    
    for (s = 0; s < numStates; s++) {
        for (i = 0; i < states[s]->numActions[0]; i++) {
            for (j = 0; j < states[s]->numActions[1]; j++) {
                reward = 0.0;
                Vprime = 0.0;
                jaction = states[s]->jointCode(i, j);
                
                for (k = 0; k < states[s]->numTrans[jaction]; k++) {
                    reward -= states[s]->rewards[jaction][k][1-ind] - (0.01*states[s]->rewards[jaction][k][ind]);
                    Vprime += states[s]->nextState[jaction][k]->qsets[0]->V;
                }
                
                old = states[s]->qsets[0]->Qs[i][j];
                states[s]->qsets[0]->Qs[i][j] = reward + Vprime;
                tchange += fabs(states[s]->qsets[0]->Qs[i][j] - old);
            }
        }
    }
    
    for (s = 0; s < numStates; s++) {
        states[s]->attack[ind]->getMinimax(states[s]->numActions, ind, states[s]->qsets[0]->Qs);
        states[s]->qsets[0]->V = states[s]->attack[ind]->mv;
    }
    
    //printf("tchange: %.2lf\n", tchange);
    if (tchange > 0.01)
        solveAttack(ind);
}

void MarkovGame::computeBR(int ind, bool first) {
    int s, i, j, k, jaction;
    double reward, Vprime;
    double old, tchange = 0.0;
    
    double prb[NUMACTIONS*2];
    //double v[NUMACTIONS*2];
    double mag;
    double val;
    
    for (s = 0; s < numStates; s++) {
        for (i = 0; i < NUMACTIONS*2; i++)
            prb[i] = 0.0;
        
        // compute the prob he takes each action
        for (i = 0; i < states[s]->numActions[0]; i++) {
            for (j = 0; j < states[s]->numActions[1]; j++) {
                if (ind == 0)
                    prb[j] += states[s]->cuentas[i][j];
                else
                    prb[i] += states[s]->cuentas[i][j];
            }
        }
        mag = 0.0;
        for (i = 0; i < states[s]->numActions[1-ind]; i++)
            mag += prb[i];
        for (i = 0; i < states[s]->numActions[1-ind]; i++)
            prb[i] /= mag;
        
        for (i = 0; i < states[s]->numActions[ind]; i++) {
            old = states[s]->Qbr[i];
            val = 0.0;
            for (j = 0; j < states[s]->numActions[1-ind]; j++) {
                reward = 0.0;
                Vprime = 0.0;
                if (ind == 0)
                    jaction = states[s]->jointCode(i, j);
                else
                    jaction = states[s]->jointCode(j, i);
                
                for (k = 0; k < states[s]->numTrans[jaction]; k++) {
                    reward += states[s]->rewards[jaction][k][ind];
                    //if (first)
                    Vprime += states[s]->nextState[jaction][k]->Vbr;
                    //else
                    //    Vprime += (states[s]->nextState[jaction][k]->Vbr + states[s]->nextState[jaction][k]->fV) / 2.0;
                }
                
                val += prb[j] * (reward + Vprime);
            }
            states[s]->Qbr[i] = val;
            tchange += fabs(states[s]->Qbr[i] - old);
        }
    }
    
    //printf("this needs to change\n");
    //exit(1);

    for (s = 0; s < numStates; s++) {
        states[s]->Vbr = -99999.0;
        
        for (i = 0; i < states[s]->numActions[ind]; i++) {
            if (states[s]->Vbr < states[s]->Qbr[i])
                states[s]->Vbr = states[s]->Qbr[i];
        }
    }

    
/*    double mx;
    for (s = 0; s < numStates; s++) {
        mx = -99999;
        states[s]->getProbs_regular(prb, ind);
        for (i = 0; i < states[s]->numActions[ind]; i++) {
            v[i] = 0.0;
            for (j = 0; j < states[s]->numActions[1-ind]; j++) {
                if (ind == 0)
                    v[i] += prb[j] * states[s]->Qbr[i][j][ind];
                else
                    v[i] += prb[j] * states[s]->Qbr[j][i][ind];
            }
            if (v[i] > mx)
                mx = v[i];
        }
        states[s]->Vbr = mx;
    }
*/

/*    printf("Qs[0]: ");
    for (i = 0; i < states[0]->numActions[ind]; i++) {
        printf("%.2lf ", states[0]->Qbr[i]);
    }
    printf("\n");
*/    
    //printf("tchange: %.2lf\n", tchange);
    if (tchange > 0.01)
        computeBR(ind, first);
}

void MarkovGame::computeBRu(int ind, bool first, int iter) {
    int s, i, j, k, jaction;
    double reward, Vprime, v;
    double old, tchange = 0.0;
    
    double prb[NUMACTIONS*2];
    //double v[NUMACTIONS*2];
    double mag;
    double val;
    
    for (s = 0; s < numStates; s++) {
        for (i = 0; i < NUMACTIONS*2; i++)
            prb[i] = 0.0;
        
        // compute the prob he takes each action
        for (i = 0; i < states[s]->numActions[0]; i++) {
            for (j = 0; j < states[s]->numActions[1]; j++) {
                if (ind == 0)
                    prb[j] += states[s]->cuentas[i][j];
                else
                    prb[i] += states[s]->cuentas[i][j];
            }
        }
        mag = 0.0;
        for (i = 0; i < states[s]->numActions[1-ind]; i++)
            mag += prb[i];

        if (mag <= 0.00001) {
            for (i = 0; i < states[s]->numActions[ind]; i++) {
                old = states[s]->Qbr[i];
                val = -99999.0;
                for (j = 0; j < states[s]->numActions[1-ind]; j++) {
                    reward = 0;
                    Vprime = 0.0;
                    if (ind == 0)
                        jaction = states[s]->jointCode(i, j);
                    else
                        jaction = states[s]->jointCode(j, i);
                    
                    for (k = 0; k < states[s]->numTrans[jaction]; k++) {
                        reward += states[s]->rewards[jaction][k][ind]+states[s]->rewards[jaction][k][1-ind];
                        //if (first)
                        Vprime += states[s]->nextState[jaction][k]->Vbr;
                    }
                    v = reward + Vprime;
                    
                    if (v > val)
                        val = v;
                }
                states[s]->Qbr[i] = val;
                tchange += fabs(states[s]->Qbr[i] - old);
            }
        }
        else {
            for (i = 0; i < states[s]->numActions[1-ind]; i++)
                prb[i] /= mag;
            
            for (i = 0; i < states[s]->numActions[ind]; i++) {
                old = states[s]->Qbr[i];
                val = 0.0;
                for (j = 0; j < states[s]->numActions[1-ind]; j++) {
                    reward = 0.0;
                    Vprime = 0.0;
                    if (ind == 0)
                        jaction = states[s]->jointCode(i, j);
                    else
                        jaction = states[s]->jointCode(j, i);
                    
                    for (k = 0; k < states[s]->numTrans[jaction]; k++) {
                        reward += states[s]->rewards[jaction][k][ind]+states[s]->rewards[jaction][k][1-ind];
                        //if (first)
                        Vprime += states[s]->nextState[jaction][k]->Vbr;
                        //else
                        //    Vprime += (states[s]->nextState[jaction][k]->Vbr + states[s]->nextState[jaction][k]->fV) / 2.0;
                    }
                    
                    val += prb[j] * (reward + Vprime);
                }
                states[s]->Qbr[i] = val;
                tchange += fabs(states[s]->Qbr[i] - old);
            }
        }
    }
    
    //printf("this needs to change\n");
    //exit(1);

    for (s = 0; s < numStates; s++) {
        states[s]->Vbr = -99999.0;
        
        for (i = 0; i < states[s]->numActions[ind]; i++) {
            if (states[s]->Vbr < states[s]->Qbr[i])
                states[s]->Vbr = states[s]->Qbr[i];
        }
    }

    
/*    double mx;
    for (s = 0; s < numStates; s++) {
        mx = -99999;
        states[s]->getProbs_regular(prb, ind);
        for (i = 0; i < states[s]->numActions[ind]; i++) {
            v[i] = 0.0;
            for (j = 0; j < states[s]->numActions[1-ind]; j++) {
                if (ind == 0)
                    v[i] += prb[j] * states[s]->Qbr[i][j][ind];
                else
                    v[i] += prb[j] * states[s]->Qbr[j][i][ind];
            }
            if (v[i] > mx)
                mx = v[i];
        }
        states[s]->Vbr = mx;
    }
*/

/*    printf("Qs[0]: ");
    for (i = 0; i < states[0]->numActions[ind]; i++) {
        printf("%.2lf ", states[0]->Qbr[i]);
    }
    printf("\n");
    
    printf("tchange: %.2lf\n", tchange);
*/    if ((tchange > 0.01) && (iter <= 40))
        computeBRu(ind, first, iter+1);
}

double MarkovGame::computeBestDefection(int sol, int me) {
    int s, i, j;
    for (s = 0; s < numStates; s++) {
        for (i = 0; i < states[s]->numActions[0]; i++) {
            states[s]->Qbr[i] = 0.0;
        }
        states[s]->Vbr = 0.0;
    }
    
    computeDefectBR_VI(sol, me);
    
    return startStates[0]->Vbr;
}

void MarkovGame::computeDefectBR_VI(int sol, int me) {
    int s, i, j, k, jaction;
    double reward, Vprime;
    double old, tchange = 0.0;
    double probs[2*NUMACTIONS];
    
    for (s = 0; s < numStates; s++) {
        //printf("%i (%i)\n", s, sol); fflush(stdout);
        for (i = 0; i < states[s]->numActions[1-me]; i++) {
            states[s]->getProbs_regular(probs, me);
            j = states[s]->qsets[sol]->highestAction(me, probs);
            reward = 0.0;
            Vprime = 0.0;
            if (me == 1)
                jaction = states[s]->jointCode(i, j);
            else
                jaction = states[s]->jointCode(j, i);
                
            for (k = 0; k < states[s]->numTrans[jaction]; k++) {
                reward += states[s]->rewards[jaction][k][1-me];
                Vprime += states[s]->nextState[jaction][k]->Vbr;
            }
                
            old = states[s]->Qbr[i];
            states[s]->Qbr[i] = reward + Vprime;
            tchange += fabs(states[s]->Qbr[i] - old);
        }
    }

    //printf("this needs to change\n");
    //exit(1);
    
    for (s = 0; s < numStates; s++) {
        states[s]->Vbr = -99999.0;
        
        for (i = 0; i < states[s]->numActions[1-me]; i++) {
            //if (s == 0)
            //    printf("%.3lf  ", states[0]->Qbr[i]);
        
            if (states[s]->Vbr < states[s]->Qbr[i])
                states[s]->Vbr = states[s]->Qbr[i];
        }        
    }
    
    //printf(" : tchange: %.2lf\n", tchange);
    if (tchange > 0.01)
        computeDefectBR_VI(sol, me);
}

void MarkovGame::computeReal(bool first) {
    int s, i, j, k, jaction;
    double r[2], Vp[2];
    double old, tchange = 0.0;
    
    for (s = 0; s < numStates; s++) {
        for (i = 0; i < states[s]->numActions[0]; i++) {
            for (j = 0; j < states[s]->numActions[1]; j++) {
                r[0] = r[1] = Vp[0] = Vp[1] = 0.0;
                
                jaction = states[s]->jointCode(i, j);
                
                //if (s == 26315) {
                //    printf("%i, %i: %i; numTrans = %i\n", i, j, jaction, states[s]->numTrans[jaction]);
                //}
                
                for (k = 0; k < states[s]->numTrans[jaction]; k++) {
                    r[0] += states[s]->rewards[jaction][k][0];
                    r[1] += states[s]->rewards[jaction][k][1];
                    Vp[0] += states[s]->nextState[jaction][k]->Vreal[0];//actualM(0);
                    Vp[1] += states[s]->nextState[jaction][k]->Vreal[1];//actualM(1);
                }
                //reward -= w1 * states[s]->mm[0]->mv + w2 * states[s]->mm[1]->mv;
                
                old = states[s]->Qreal[i][j][0];
                states[s]->Qreal[i][j][0] = r[0] + Vp[0];
                states[s]->Qreal[i][j][1] = r[1] + Vp[1];
                tchange += fabs(states[s]->Qreal[i][j][0] - old);
            }
        }
    }
    
    for (s = 0; s < numStates; s++) {
        for (j = 0; j < 2; j++) {
            if (first)
                states[s]->Vreal[j] = states[s]->maxM(j);
            else
                states[s]->Vreal[j] = states[s]->actualM(j);
        }
    }
    
    //printf("tchange: %.2lf\n", tchange);
    if (tchange > 0.01)
        computeReal(first);
}

void MarkovGame::computePrevent(int _iter) {//int ind, double myWeight) {
    //printf("to compute the preventitive strategy\n"); fflush(stdout);
    
    if (_iter >= 50)
        return;
    
    
    int s, i, j, k, jaction;
    double old, old2, tchange = 0.0;
    double probs[2*NUMACTIONS];
    double v[2*NUMACTIONS];
    
    int test_s = 0;//26321;//26313;//26315;//25979;
    
    for (s = 0; s < numStates; s++) {
        //printf("s = %i\n", s); fflush(stdout);
        for (i = 0; i < states[s]->numActions[0]; i++) {
            for (j = 0; j < states[s]->numActions[1]; j++) {
                jaction = states[s]->jointCode(i, j);
                
                v[0] = v[1] = 0.0;
                for (k = 0; k < states[s]->numTrans[jaction]; k++) {
                    v[0] += states[s]->rewards[jaction][k][0];
                    v[1] += states[s]->rewards[jaction][k][1];
                    v[0] += states[s]->nextState[jaction][k]->Vreal[0];
                    v[1] += states[s]->nextState[jaction][k]->Vreal[1];
                }
                
                old = states[s]->Qreal[i][j][0];
                old2 = states[s]->Qreal[i][j][1];
                if (states[s]->Qreal[i][j][0] <= -50.0)
                    states[s]->Qreal[i][j][0] = -50.0;
                else
                    states[s]->Qreal[i][j][0] = v[0];
                if (states[s]->Qreal[i][j][1] <= -50.0)
                    states[s]->Qreal[i][j][1] = -50.0;
                else
                    states[s]->Qreal[i][j][1] = v[1];
                tchange += fabs(states[s]->Qreal[i][j][0] - old) + fabs(states[s]->Qreal[i][j][1] - old2);
            }
        }
    }
    
    double mag;
    for (s = 0; s < numStates; s++) {
        mag = 0.0;
        for (i = 0; i < states[s]->numActions[0]; i++) {
            for (j = 0; j < states[s]->numActions[1]; j++) {
                mag += states[s]->cuentas_real[i][j];
            }
        }
        
        states[s]->Vreal[0] = states[s]->Vreal[1] = 0.0;
        for (i = 0; i < states[s]->numActions[0]; i++) {
            for (j = 0; j < states[s]->numActions[1]; j++) {
                states[s]->Vreal[0] += states[s]->Qreal[i][j][0] * states[s]->cuentas_real[i][j] / mag;
                states[s]->Vreal[1] += states[s]->Qreal[i][j][1] * states[s]->cuentas_real[i][j] / mag;
            }
        }

    }
    
    //printf("tchange: %.2lf\n", tchange);
    if (tchange > 0.01)
        computePrevent(_iter+1);//ind, myWeight);
}
