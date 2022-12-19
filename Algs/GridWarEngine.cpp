#include "GridWarEngine.h"

bool printITALL = false;

// ****************************************************
//
//  Load class
//
// ****************************************************
Load::Load() {
    printf("incomplete Load constructor\n");
    exit(1);
}

Load::Load(int _strt, int _fin, double _valor, double _sz, int _id) {
    strt = _strt;
    fin = _fin;
    valor = _valor;
    sz = _sz;
    ID = _id;
    
    //printf("created load %i: t = [%i-%i); value = %.2lf, size = %.2lf\n", ID, strt, fin, valor, sz);
}

Load::~Load() {}



// ****************************************************
//
//  GridWarEngine class
//
// ****************************************************
GridWarEngine::GridWarEngine() {
    printf("incomplete GridWarEngine constructor\n");
    exit(1);
}

GridWarEngine::GridWarEngine(char filename[1024]) {
    readTheFile(filename);

    printf("read completed\n"); fflush(stdout);
    
    numAgents = 2;
    oneGoalNeeded = false;
    rewardMove = new double[numAgents];
    actions = new int[numAgents];
}

GridWarEngine::~GridWarEngine() {
    int i;
    
    for (i = 0; i < numLoads1; i++)
        delete loads1[i];
    delete loads1;
    delete currentLoads1;
    for (i = 0; i < numLoads2; i++)
        delete loads2[i];
    delete loads2;
    delete currentLoads2;
    
    delete rewardMove;
    delete actions;
}

void GridWarEngine::readTheFile(char filename[1024]) {
    char fname[1024];
    
    sprintf(fname, "../Games/%s.txt", filename);
    printf("going to read file %s\n", fname); fflush(stdout);
    FILE *fp = fopen(fname, "r");
    char buf[1024];
    
    fscanf(fp, "%s", buf);  // "Generation
    fscanf(fp, "%s", buf);  // "Hour"
    fscanf(fp, "%s", buf);  // "Amount"
    
    int i, hora;
    for (i = 0; i < NUM_HOURS; i++) {
        fscanf(fp, "%i", &hora);
        fscanf(fp, "%lf", &(generation[i]));
    }
    
    fscanf(fp, "%s", buf);  // "StorageCapacity
    fscanf(fp, "%lf", &storageCapacity);
    
    printf("storageCapacity = %.1lf\n", storageCapacity);
    
    fscanf(fp, "%s", buf);  // "Agent1"
    fscanf(fp, "%i", &numLoads1);
    
    printf("Agent1 has %i loads\n", numLoads1);
    
    loads1 = new Load*[numLoads1];
    currentLoads1 = new Load*[numLoads1];
    fscanf(fp, "%s", buf);  // "start"
    fscanf(fp, "%s", buf);  // "end"
    fscanf(fp, "%s", buf);  // "value"
    fscanf(fp, "%s", buf);  // "sz"
    
    int strt, fin;
    double valor, sz;
    for (i = 0; i < numLoads1; i++) {
        fscanf(fp, "%i %i %lf %lf", &strt, &fin, &valor, &sz);
        loads1[i] = new Load(strt, fin, valor, sz, i);
    }

    fscanf(fp, "%s", buf);  // "Agent2"
    fscanf(fp, "%i", &numLoads2);
    
    printf("Agent2 has %i loads\n", numLoads2);
    
    loads2 = new Load*[numLoads2];
    currentLoads2 = new Load*[numLoads2];
    fscanf(fp, "%s", buf);  // "start"
    fscanf(fp, "%s", buf);  // "end"
    fscanf(fp, "%s", buf);  // "value"
    fscanf(fp, "%s", buf);  // "sz"
    
    for (i = 0; i < numLoads2; i++) {
        fscanf(fp, "%i %i %lf %lf", &strt, &fin, &valor, &sz);
        loads2[i] = new Load(strt, fin, valor, sz, i);
    }
    
    
    fclose(fp);
}


void GridWarEngine::createGame() {
    MarkovGame *mg = new MarkovGame();
    State *currentState, *newState;

    initRound();
    
    bool validActs1[MAX_LOADS], validActs2[MAX_LOADS];
    getValidActions(validActs1, 0);
    getValidActions(validActs2, 1);
    
    //printState();
    
    currentState = getState(mg, validActs1, validActs2);
    mg->addStartState(currentState);
    
    int i, j, k, s, sS, eS = 0, tau;
    for (tau = 0; tau < 24; tau++) {
        printf("tau = %i\n", tau); fflush(stdout);
        sS = eS;
        eS = mg->numStates;
        for (s = sS; s < eS; s++) {
            //printf("**************************for state %i\n", s);
            currentState = mg->states[s];
            
            for (i = 0; i < currentState->numActions[0]; i++) {
                for (j = 0; j < currentState->numActions[1]; j++) {
                    t = tau;
                    currentStorageLevel = currentState->featureVector[currentState->numFeatures-1];
                    numCurLoads1 = numCurLoads2 = 0;

                    if (currentState->ID == 1253) {
                        printITALL = true;
                        printf("%i (%i, %i) -> \n", currentState->ID, i, j);
                    }
                    
                    for (k = 0; k < numLoads1; k++) {
                        if (currentState->featureVector[k+1] > 0.5) {
                            currentLoads1[numCurLoads1] = loads1[k];
                            numCurLoads1++;
                            if (currentState->ID == 1253) {
                                printf("%i ", loads1[k]->ID);
                            }
                        }
                    }
                    //printf("\nloads for 1: ");
                    for (k = 0; k < numLoads2; k++) {
                        if (currentState->featureVector[k+numLoads1+1] > 0.5) {
                            currentLoads2[numCurLoads2] = loads2[k];
                            numCurLoads2++;
                            if (currentState->ID == 1253) {
                                printf("%i ", loads2[k]->ID);
                            }
                        }
                    }
                    //printf("\n");

                    if (currentState->ID == 1253) {
                        printf("\tnumLoads: %i, %i\n", numCurLoads1, numCurLoads2);
                    }
                    //printState();
                    
                    actions[0] = i;
                    actions[1] = j;
                    //printf("actions: %i, %i\n", actions[0], actions[1]);

                    //if ((currentState->featureVector[11] > 0.5) && (actions[0] == 0) && (actions[1] == 0)) {
                    //    printState();
                    //    currentState->printFeatureVector();
                    //}
                    
                    MoveAgents();
                    
                    //if ((currentState->featureVector[11] > 0.5) && (actions[1] == 0))
                    //    printState();
                    
                    getValidActions(validActs1, 0);
                    getValidActions(validActs2, 1);
                    newState = getState(mg, validActs1, validActs2);
                    
                    if (currentState->ID == 1253) {
                        printf("%i\n", newState->ID);
                        printITALL = false;
                    }
                    
                    
                    currentState->addTransition(newState, actions, rewardMove);
                    
                    //printState();
                    //printf("----------------\n\n");
                    
                    if (OnGoal(0) && (OnGoal(1))) {
                        mg->addGoalState(newState);
                    }
                }
            }
        }
    }
    
    printf("completed: %i, %i, %i\n", mg->numStates, mg->numStartStates, mg->numGoalStates); fflush(stdout);

    char msge[1024];
    strcpy(msge, "MG_gridwar1.txt");
    mg->writeGame(msge);
}

State *GridWarEngine::getState(MarkovGame *mg, bool *validActs1, bool *validActs2) {
    int numFeatures;
    int numActs[2] = {0, 0};
    numFeatures = 1 + numLoads1 + numLoads2 + 1;
    double *fvector = new double[numFeatures];
    int i;
    
    fvector[0] = t;
    
    if (t >= NUM_HOURS) {
        for (i = 1; i < numFeatures-1; i++) {
            fvector[i] = 0.0;
        }
        fvector[numFeatures-1] = currentStorageLevel;
        
        numActs[0] = 1;
        numActs[1] = 1;
        
        return mg->getState(numFeatures, fvector, numActs);
    }
    
    for (i = 0; i < numLoads1; i++) {
        fvector[i+1] = (double)(validActs1[i]);
        if (validActs1[i])
            numActs[0]++;
    }
    for (i = 0; i < numLoads2; i++) {
        fvector[i+1+numLoads1] = (double)(validActs2[i]);
        if (validActs2[i])
            numActs[1]++;
    }
    fvector[numFeatures-1] = currentStorageLevel;
    
    numActs[0] = pow(2, numActs[0]);
    numActs[1] = pow(2, numActs[1]);
    
    //printf("the current state has %i, %i actions\n", numActs[0], numActs[1]);
    
    return mg->getState(numFeatures, fvector, numActs);
}

void GridWarEngine::MoveAgents() {
    rewardMove[0] = rewardMove[1] = 0.0;

    //printf("\n");
    
    // consume loads as specified
    double totalAvailable = currentStorageLevel + generation[t];
    double desiredConsumption1 = 0.0, desiredConsumption2 = 0.0, desiredConsumption;
    
    // remove all of the loads that were consumed
    int bitmask = 1;
    int i;
    for (i = 0; i < numCurLoads1; i++) {
        if (actions[0] & bitmask) { // trying to consume this load
            desiredConsumption1 += currentLoads1[i]->sz;
        }
        bitmask *= 2;
    }
    bitmask = 1;
    for (i = 0; i < numCurLoads2; i++) {
        if (actions[1] & bitmask) { // trying to consume this load
            desiredConsumption2 += currentLoads2[i]->sz;
        }
        bitmask *= 2;
    }
    desiredConsumption = desiredConsumption1 + desiredConsumption2;
    
    //if (printITALL) {
    //    printf("totalAvailable = %.9lf (%lf, %lf; %i)\n", totalAvailable, currentStorageLevel, generation[t], t);
    //    printf("desiredConsumption = %.9lf\n", desiredConsumption);
    //}
    
    //printf("time: %i\n", t);
    if (desiredConsumption <= totalAvailable+0.00000001) {  // give them the consumption
        //printf("desired consumption (%.20lf > %.20lf) is okay\n", desiredConsumption, totalAvailable);
        bitmask = pow(2,numCurLoads1-1);
        //printf("0 consumed: ");
        for (i = numCurLoads1-1; i >= 0; i--) {
            if (actions[0] & bitmask) { // trying to consume this load
                rewardMove[0] += currentLoads1[i]->valor;
                //if (printITALL)
                //    printf("\t1-%i\n", currentLoads1[i]->ID+1);
                removeLoad(i, 0);
                //if (printITALL)
                //    printf("numCurLoads1 = %i\n", numCurLoads1);
            }
            bitmask /= 2;
        }

        //bitmask = 1;
        bitmask = pow(2,numCurLoads2-1);
        for (i = numCurLoads2-1; i >= 0; i--) {
            if (actions[1] & bitmask) { // trying to consume this load
                rewardMove[1] += currentLoads2[i]->valor;
                //if (printITALL)
                //    printf("\t2-%i\n", currentLoads2[i]->ID+11);
                removeLoad(i, 1);
            }
            bitmask /= 2;
        }
        //printf("\n");
        //printf("total consumption: %.2lf (out of %.2lf)\n", desiredConsumption, totalAvailable);
        
        currentStorageLevel = totalAvailable - desiredConsumption;
        
        //printf("currentStorageLevel: %.2lf\n", currentStorageLevel);
        
        if (currentStorageLevel > storageCapacity)
            currentStorageLevel = storageCapacity;
    }
    else { // consumption fails
        //printf("desired consumption (%.20lf > %.20lf) is too high\n", desiredConsumption, totalAvailable);
        currentStorageLevel = 0;
        rewardMove[0] = rewardMove[1] = 0.0;
        if ((desiredConsumption1 > 0.00001) && (desiredConsumption2 > 0.00001))
            rewardMove[0] = rewardMove[1] = -1.0;
        else if (desiredConsumption1 > 0.00001)
            rewardMove[0] = -2.0;
        else
            rewardMove[1] = -2.0;
    }
    
    t++;
    // remove expired loads
    for (i = 0; i < numCurLoads1; i++) {
        if (currentLoads1[i]->fin <= t) {
            //printf("shed: ");
            removeLoad(i, 0);
        }
    }
    for (i = 0; i < numCurLoads2; i++) {
        if (currentLoads2[i]->fin <= t) {
            //printf("shed: ");
            removeLoad(i, 1);
        }
    }
    
    // add new loads
    addLoads(t);
    
    //if (t > 23)
    //    t = 0;
    //printf("\n");
}

void GridWarEngine::initRound() {
    t = 0;
    currentStorageLevel = 0;
    numCurLoads1 = 0;
    numCurLoads2 = 0;
    addLoads(t);
}

bool GridWarEngine::OnGoal(int index) {
    if (t >= NUM_HOURS)
        return true;
    else
        return false;
}

void GridWarEngine::getValidActions(bool *validActs, int index) {
    int i;
    
    if (index == 0) {
        //printf("0: ");
        for (i = 0; i < numLoads1; i++) {
            validActs[i] = false;
        }
        for (i = 0; i < numCurLoads1; i++) {
            //printf("%i ", currentLoads1[i]->ID);
            validActs[currentLoads1[i]->ID] = true;
        }
        //printf("\n");
    }
    else {
        //printf("1: ");
        for (i = 0; i < numLoads2; i++) {
            validActs[i] = false;
        }
        for (i = 0; i < numCurLoads2; i++) {
            //printf("%i ", currentLoads2[i]->ID);
            validActs[currentLoads2[i]->ID] = true;
        }
        //printf("\n");
    }
}

int GridWarEngine::convertIt(int accion, bool *validActions, int numActs) {
    return accion;
}

void GridWarEngine::deduceActions(int rawActions[2], int geActions[2], bool lastValidActs[2][2*NUMACTIONS]) {
    rawActions[0] = geActions[0];
    rawActions[1] = geActions[1];
}


double GridWarEngine::getCurrentReward(int index) {
    return rewardMove[index];
}

void GridWarEngine::addLoads(int tau) {
    int i;
    
    //printf("Adding loads at time %i\n", tau);
    for (i = 0; i < numLoads1; i++) {
        if (loads1[i]->strt == tau) {
            //printf("\t0: adding load %i (%i %i)\n", i, loads1[i]->strt, loads1[i]->fin); fflush(stdout);
            currentLoads1[numCurLoads1] = loads1[i];
            numCurLoads1++;
        }
    }
    
    for (i = 0; i < numLoads2; i++) {
        if (loads2[i]->strt == tau) {
            //printf("\t1: adding load %i (%i %i)\n", i, loads2[i]->strt, loads2[i]->fin); fflush(stdout);
            currentLoads2[numCurLoads2] = loads2[i];
            numCurLoads2++;
        }
    }
}

void GridWarEngine::removeLoad(int l, int index) {
    if (index == 0) {
        //printf("0: removing load %i\n", currentLoads1[l]->ID); fflush(stdout);
        
        int i;
        numCurLoads1--;
        for (i = l; i < numCurLoads1; i++) {
            currentLoads1[i] = currentLoads1[i+1];
        }
    }
    else {
        //printf("1: removing load %i\n", currentLoads2[l]->ID); fflush(stdout);
        
        int i;
        numCurLoads2--;
        for (i = l; i < numCurLoads2; i++) {
            currentLoads2[i] = currentLoads2[i+1];
        }
    }
}

void GridWarEngine::printState() {
    bool validActs1[MAX_LOADS], validActs2[MAX_LOADS];
    getValidActions(validActs1, 0);
    getValidActions(validActs2, 1);
    
    int i;
    printf("\nThe CURRENT STATE of world\n");
    printf("\tHour: %i\n", t);
    printf("\tAgent1's loads: ");
    for (i = 0; i < numLoads1; i++) {
        printf("%i", (int)(validActs1[i]));
    }
    printf("\n");
    printf("\tAgent2's loads: ");
    for (i = 0; i < numLoads2; i++) {
        printf("%i", (int)(validActs2[i]));
    }
    printf("\n");
    if (t < NUM_HOURS)
        printf("\tUnits available: %.2lf (%.2lf, %.2lf)\n\n", currentStorageLevel + generation[t], currentStorageLevel, generation[t]);
    else
        printf("\tUnits available: %.2lf (%.2lf, %.2lf)\n\n", currentStorageLevel, currentStorageLevel, 0.0);
}

