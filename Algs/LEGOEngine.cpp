#include "LEGOEngine.h"


LEGO::LEGO() {
    printf("incomplete LEGO constructor\n");
    exit(1);
}

LEGO::LEGO(int _shape, int _col) {
    shape = _shape;
    col = _col;
    value = shape + col;
    
    int sqr_wdth = 40;
    int w = 2 * 11 * sqr_wdth;
    int h = 17 * sqr_wdth;
    
    if (shape == SQUARE)
        cy = h / 2 + 100 + sqr_wdth;
    else if (shape == TRIANGLE)
        cy = h / 2 + sqr_wdth;
    else if (shape == CIRCLE)
        cy = h / 2 - 100 + sqr_wdth;
        
    if (col == RED)
        cx = w / 4 - 70;
    else if (col == BLUE)
        cx = w / 4;
    else if (col == GREEN)
        cx = w / 4 + 70;
}

LEGO::~LEGO() {
}

LEGOEngine::LEGOEngine() {
    legos[0] = new LEGO(SQUARE, RED);
    legos[1] = new LEGO(SQUARE, BLUE);
    legos[2] = new LEGO(SQUARE, GREEN);
    legos[3] = new LEGO(TRIANGLE, RED);
    legos[4] = new LEGO(TRIANGLE, BLUE);
    legos[5] = new LEGO(TRIANGLE, GREEN);
    legos[6] = new LEGO(CIRCLE, RED);
    legos[7] = new LEGO(CIRCLE, BLUE);
    legos[8] = new LEGO(CIRCLE, GREEN);
    
    actions = new int[2];
    numAgents = 2;
}

LEGOEngine::~LEGOEngine() {
    int i;
    for (i = 0; i < 9; i++)
        delete legos[i];
    delete actions;
}

State *LEGOEngine::getState(MarkovGame *mg, bool *validActs1, bool *validActs2) {
    int numFeatures = 6;
    int numActs[2];
    double *fvector = new double[numFeatures];
    
    int i;
    for (i = 0; i < numPieces[0]; i++)
        fvector[i] = orderedOwnership[0][i];
    for (i = numPieces[0]; i < 3; i++)
        fvector[i] = -1;
    for (i = 0; i < numPieces[1]; i++)
        fvector[i+3] = orderedOwnership[1][i];
    for (i = numPieces[1]; i < 3; i++)
        fvector[i+3] = -1;
    
    int c = 0;
    for (i = 0; i < 9; i++) {
        if (available[i])
            c++;
    }
    numActs[whosTurn] = c;
    numActs[1-whosTurn] = 1;
    
    //printf("numStates = %i\n", mg->numStates);
    //for (i = 0; i < 6; i++)
    //    printf("%.0lf ", fvector[i]);
    //printf(": %i %i\n", numActs[0], numActs[1]);
    
    return mg->getState(numFeatures, fvector, numActs);
}

void LEGOEngine::MoveAgents() {
    int c = 0, i;
    //printf("selected %i\n", actions[whosTurn]);
    for (i = 0; i < actions[whosTurn]; i++) {
        if (available[i])
            c++;
    }
    rawActs[whosTurn] = c;
    rawActs[1-whosTurn] = 0;
    
    int act = actions[whosTurn];
    //printf("actualized action: %i\n", act);
    
    //printf("taking action %i\n", actions[whosTurn]);
    //if (!available[actions[whosTurn]]) {
    if (!available[act]) {
        printf("invalid move: %i, %i\n", whosTurn, actions[whosTurn]);
        exit(1);
    }
    
    //printf("whosTurn = %i; numPieces = %i\n", whosTurn, numPieces[whosTurn]);
    ownership[whosTurn][numPieces[whosTurn]] = act;//actions[whosTurn];
    orderedOwnership[whosTurn][numPieces[whosTurn]] = act;
    numPieces[whosTurn]++;
    available[act] = false;

    if (numPieces[whosTurn] == 2) {
        int tmp;
        if (orderedOwnership[whosTurn][0] > orderedOwnership[whosTurn][1]) {
            tmp = orderedOwnership[whosTurn][0];
            orderedOwnership[whosTurn][0] = orderedOwnership[whosTurn][1];
            orderedOwnership[whosTurn][1] = tmp;
        }
    }
    else if (numPieces[whosTurn] == 3) {
        int tmp;
        if (orderedOwnership[whosTurn][1] > orderedOwnership[whosTurn][2]) {
            tmp = orderedOwnership[whosTurn][1];
            orderedOwnership[whosTurn][1] = orderedOwnership[whosTurn][2];
            orderedOwnership[whosTurn][2] = tmp;
        }
        if (orderedOwnership[whosTurn][0] > orderedOwnership[whosTurn][1]) {
            tmp = orderedOwnership[whosTurn][0];
            orderedOwnership[whosTurn][0] = orderedOwnership[whosTurn][1];
            orderedOwnership[whosTurn][1] = tmp;
        }
    }
    
    whosTurn = 1 - whosTurn;
}

void LEGOEngine::initRound() {
    whosTurn = 0;
    numPieces[0] = numPieces[1] = 0;
    int i;
    for (i = 0; i < 9; i++)
        available[i] = true;
}

bool LEGOEngine::OnGoal(int index) {
    if ((numPieces[0] == 3) && (numPieces[1] == 3))
        return true;
    
    return false;
}

void LEGOEngine::getValidActions(bool *validActs, int index) {
    int i;
    
    for (i = 0; i < 9; i++)
        validActs[i] = available[i];
}

double LEGOEngine::getCurrentReward(int index) {
    //printf("in the LEGOEngine getCurrentReward\n"); fflush(stdout);
    
    if (!OnGoal(-1))
        return 0.0;
    
    if (isValidConstruction(index))
        return cardSum(index);
    else
        return -(cardSum(index) / 4.0);
}

int LEGOEngine::convertIt(int accion, bool *validActions, int numActs) {
	int i;
	int count = 0;
	
	for (i = 0; i < NUMACTIONS*2; i++) {
		if (validActions[i]) {
			if (count == accion) {
				return i;
            }
			count ++;
		}
	}
	
	return -1;
    //return accion;
}

int LEGOEngine::deduceIt(int accion, bool *validActions, int numActs) {
    int i;
    int c = 0;
    for (i = 0; i < accion; i++) {
        if (available[i])
            c++;
    }
    
    return c;
    //return accion;
}

void LEGOEngine::deduceActions(int rawActions[2], int geActions[2], bool lastValidActs[2][2*NUMACTIONS]) {
    //rawActions[0] = geActions[0];
    //rawActions[1] = geActions[1];
    rawActions[0] = rawActs[0];
    rawActions[1] = rawActs[1];
}

bool LEGOEngine::isValidConstruction(int index) {
    if (!allSameShape(index) && !allDiffShape(index))
        return false;
    
    if (!allSameCol(index) && !allDiffCol(index))
        return false;
    
    return true;
}

bool LEGOEngine::allSameShape(int index) {
    if (legos[ownership[index][0]]->shape != legos[ownership[index][1]]->shape)
        return false;
    
    if (legos[ownership[index][0]]->shape != legos[ownership[index][2]]->shape)
        return false;
    
    return true;
}

bool LEGOEngine::allDiffShape(int index) {
    if (legos[ownership[index][0]]->shape == legos[ownership[index][1]]->shape)
        return false;
    
    if (legos[ownership[index][0]]->shape == legos[ownership[index][2]]->shape)
        return false;

    if (legos[ownership[index][1]]->shape == legos[ownership[index][2]]->shape)
        return false;
    
    return true;
}

bool LEGOEngine::allSameCol(int index) {
    if (legos[ownership[index][0]]->col != legos[ownership[index][1]]->col)
        return false;
    
    if (legos[ownership[index][0]]->col != legos[ownership[index][2]]->col)
        return false;
    
    return true;
}

bool LEGOEngine::allDiffCol(int index) {
    if (legos[ownership[index][0]]->col == legos[ownership[index][1]]->col)
        return false;
    
    if (legos[ownership[index][0]]->col == legos[ownership[index][2]]->col)
        return false;
    
    if (legos[ownership[index][1]]->col == legos[ownership[index][2]]->col)
        return false;
    
    return true;
}

int LEGOEngine::cardSum(int index) {
    int i;
    int sum = 0;
    for (i = 0; i < 3; i++)
        sum += legos[ownership[index][i]]->value;
    
    return sum;
}

void LEGOEngine::createGame() {
    MarkovGame *mg = new MarkovGame();
    State *currentState, *newState[6];
    
    initRound();
    
    //bool validActs1[2*NUMACTIONS], validActs2[2*NUMACTIONS];
    //getValidActions(validActs1, 0);
    //getValidActions(validActs2, 1);
    
    //printState();
    
    //currentState = getState(mg, validActs1, validActs2);
    currentState = getState(mg, NULL, NULL);
    mg->addStartState(currentState);

    int trn[6];
    for (trn[0] = 0; trn[0] < 9; trn[0]++) {
        //printf("trn0 = %i\n", trn[0]); fflush(stdout);
        currentState = mg->states[0];
        setGame(currentState);
        actions[0] = trn[0];
        MoveAgents();
        newState[0] = getState(mg, NULL, NULL);
        addTransition(currentState, newState[0]);
        
        for (trn[1] = 0; trn[1] < 9; trn[1]++) {
            //printf("\ttrn1 = %i\n", trn[1]); fflush(stdout);
            currentState = newState[0];
            setGame(currentState);
            if (!available[trn[1]]) {
                continue;
            }
            actions[1] = trn[1];
            MoveAgents();
            newState[1] = getState(mg, NULL, NULL);
            addTransition(currentState, newState[1]);
 
            for (trn[2] = 0; trn[2] < 9; trn[2]++) {
                //printf("\t\ttrn2 = %i\n", trn[2]); fflush(stdout);
                currentState = newState[1];
                setGame(currentState);
                if (!available[trn[2]]) {
                    continue;
                }
                actions[0] = trn[2];
                MoveAgents();
                newState[2] = getState(mg, NULL, NULL);
                addTransition(currentState, newState[2]);
                //printf("numStates = %i\n", mg->numStates); fflush(stdout);
                
                for (trn[3] = 0; trn[3] < 9; trn[3]++) {
                    currentState = newState[2];
                    setGame(currentState);
                    if (!available[trn[3]]) {
                        continue;
                    }
                    actions[1] = trn[3];
                    MoveAgents();
                    newState[3] = getState(mg, NULL, NULL);
                    addTransition(currentState, newState[3]);

                    for (trn[4] = 0; trn[4] < 9; trn[4]++) {
                        currentState = newState[3];
                        setGame(currentState);
                        if (!available[trn[4]]) {
                            continue;
                        }
                        actions[0] = trn[4];
                        MoveAgents();
                        newState[4] = getState(mg, NULL, NULL);
                        addTransition(currentState, newState[4]);
                        
                        for (trn[5] = 0; trn[5] < 9; trn[5]++) {
                            currentState = newState[4];
                            setGame(currentState);
                            if (!available[trn[5]]) {
                                continue;
                            }
                            actions[1] = trn[5];
                            MoveAgents();
                            newState[5] = getState(mg, NULL, NULL);
                            addTransition(currentState, newState[5]);
                            
                            mg->addGoalState(newState[5]);
                        }
                    }
                }
            }
        }
    }
    
    
    printf("completed: %i, %i, %i\n", mg->numStates, mg->numStartStates, mg->numGoalStates); fflush(stdout);
    char msge[1024];
    strcpy(msge, "MG_legos.txt");
    mg->writeGame(msge);
}

void LEGOEngine::setGame(State *s) {
    int i;
    for (i = 0; i < 9; i++)
        available[i] = true;

    numPieces[0] = numPieces[1] = 0;
    for (i = 0; i < 3; i++) {
        ownership[0][i] = (int)(s->featureVector[i]);
        orderedOwnership[0][i] = (int)(s->featureVector[i]);
        if (s->featureVector[i] >= 0) {
            available[ownership[0][i]] = false;
            numPieces[0]++;
        }
        
        ownership[1][i] = (int)(s->featureVector[i+3]);
        orderedOwnership[1][i] = (int)(s->featureVector[i+3]);
        if (s->featureVector[i+3] >= 0) {
            available[ownership[1][i]] = false;
            numPieces[1]++;
        }
    }
    if (numPieces[0] == numPieces[1])
        whosTurn = 0;
    else
        whosTurn = 1;
}

void LEGOEngine::printState() {
    printf("%i: (%i, %i, %i), (%i, %i, %i)\n", whosTurn, ownership[0][0], ownership[0][1], ownership[0][2], ownership[1][0], ownership[1][1], ownership[1][2]);
}

void LEGOEngine::addTransition(State *currentState, State *newState) {
    double rewardMove[2];
    int acts[2];
    
    rewardMove[0] = getCurrentReward(0);
    rewardMove[1] = getCurrentReward(1);
    acts[1-whosTurn] = deduceIt(actions[1-whosTurn], NULL, 0);
    acts[whosTurn] = 0;
    currentState->addTransition(newState, acts, rewardMove);
}

