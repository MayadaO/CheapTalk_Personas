#include "CFR.h"

//extern char partido[1024];

CFR::CFR() {
    printf("incomplete CFR constructor\n");
    exit(1);
}

CFR::CFR(int _me, MarkovGame *_mg, int _maxDepth, char gameName[1024], bool _update) {
    me = _me;
    mg = _mg;
    maxDepth = _maxDepth;
    update = _update;
    
    if (update)
        strcpy(nickname, "CFR");
    else
        strcpy(nickname, "CFR-NE");
    
    int s = 599;
    
    buildGameTree();
    
    computeNE(gameName);
    
    cNode = gameTree[0];
    
    numMovemientos = t = 0;
//    if (partido[0] == 'p')
//        dlyr = new delayer(1);
//    else
//        dlyr = new delayer(2);
}

CFR::~CFR() {
    int i;
    for (i = 0; i < numNodes; i++)
        delete gameTree[i];
    
    delete mg;
//    delete dlyr;
}

void CFR::buildGameTree() {
    printf("starting to build the tree\n"); fflush(stdout);
    
    gameTree[0] = new Node(0, me, mg->states[0], 0, false);
    numNodes = 1;
    
    levelCounter[0] = 0;
    //printf("add levelCounter %i: %i\n", 0, levelCounter[0]);
    
    int d=maxDepth, l;
    int i, j, k, jact, noder, noder2;
    int c;
    for (l = 1; l < d; l++) {
        levelCounter[l] = numNodes;
        //printf("add levelCounter %i: %i\n", l, levelCounter[l]);
        
        initAlready();
        
        for (i = levelCounter[l-1]; i < levelCounter[l]; i++) {
            if (isGoalState(gameTree[i]->s->ID))
                continue;
            
            for (j = 0; j < gameTree[i]->s->numActions[0]; j++) {
                for (k = 0; k < gameTree[i]->s->numActions[1]; k++) {
                    jact = gameTree[i]->s->jointCode(j, k);
                    noder = alreadyExists(gameTree[i]->s->nextState[jact][0]->ID);
                    if (noder < 0) {                        
                        gameTree[numNodes] = new Node(numNodes, me, gameTree[i]->s->nextState[jact][0], 1, isGoalState(gameTree[i]->s->nextState[jact][0]->ID));
                        gameTree[i]->next[jact] = gameTree[numNodes];
                        gameTree[i]->s->nextState[jact][0]->added = numNodes;
                        numNodes++;
                    }
                    else {
                        gameTree[noder]->duplo ++;
                        gameTree[i]->next[jact] = gameTree[noder];
                    }
                }
            }
        }
        c = levelCounter[l] - levelCounter[l-1];
        //printf("%i: numNodes = %i\n", l, c);
        if (c == 0) {
            //l--;
            break;
        }
    }
    if (l == d)
        actualDepth = l;
    else
        actualDepth = l-1;
    
    //printf("l = %i\n", l);
    levelCounter[l] = numNodes;
    //printf("add levelCounter %i: %i\n", l, levelCounter[l]);
    
    //for (i = 0; i < actualDepth+1; i++) {
    //    printf("%i: %i\n", i, levelCounter[i]);
    //}
    
    printf("numNodes = %i\n", numNodes);
    printf("gameDepth = %i\n", actualDepth);
}

int CFR::findNode(State *s, int nivel) {
    int i;
    
    for (i = levelCounter[nivel]; i < levelCounter[nivel+1]; i++) {
        if (gameTree[i]->s->ID == s->ID)
            return i;
    }
    
    printf("got something wrong (findNode)\n");
    exit(1);
    
    return -1;
}

bool CFR::isGoalState(int s) {
    int i;
    for (i = 0; i < mg->numGoalStates; i++) {
        if (s == mg->goalStates[i]->ID)
            return true;
    }
    
    return false;
}

int CFR::alreadyExists(State *s, int strt, int nd) {
    int i;
    
    //printf("checking for: %i; (%.3lf, %.3lf)\n", s->ID, r[0], r[1]);
    
    for (i = strt; i < nd; i++) {
        if (s->ID == gameTree[i]->s->ID)// && (fabs(r[0] - gameTree[i]->r[0]) < 0.00001) && (fabs(r[1] - gameTree[i]->r[1]) < 0.00001))
            return i;
    }
    
    return -1;
}

int CFR::alreadyExists(State *s) {
    int i;
    
    //printf("checking for: %i; (%.3lf, %.3lf)\n", s->ID, r[0], r[1]);
    
    for (i = 0; i < numNodes; i++) {
        if (s->ID == gameTree[i]->s->ID)// && (fabs(r[0] - gameTree[i]->r[0]) < 0.00001) && (fabs(r[1] - gameTree[i]->r[1]) < 0.00001))
            return i;
    }
    
    return -1;
}

int CFR::alreadyExists(int s) {
    //printf("int s: %i; ", s); fflush(stdout);
    //printf("%i\n", mg->states[s]->added); fflush(stdout);
    
    return mg->states[s]->added;
    //return -1;
}

void CFR::initAlready() {
    int i;
    
    for (i = 0; i < mg->numStates; i++)
        mg->states[i]->added = -1;
}

void CFR::computeNE(char gameName[1024]) {
    int Tmax = 3;
    int T;
    
    if (!strcmp(gameName, "legos"))
        Tmax = 30;
    else if (!strcmp(gameName, "gridwar1"))
        Tmax = 20;
    
    for (T = 1; T < Tmax; T++) {
        reset();
        computeUtilities(0);
        computeUtilities(1);
        printf("utility: %lf, %lf\n", gameTree[0]->utility[0], gameTree[0]->utility[1]); fflush(stdout);
        
        computeReachability(0);
        computeReachability(1);
        //printf("computed reachabilities\n"); fflush(stdout);
        
        updateRegret(0);
        updateRegret(1);
        //printf("computed regrets\n"); fflush(stdout);
        
        updatePolicy(0, T);
        updatePolicy(1, T);
        //printf("updated the policies\n"); fflush(stdout);
    }
    
    if (update)
        initCounts();
}

void CFR::updateMe() {
    //printf("u"); fflush(stdout);
    computeUtilities(me);
    //printf("p"); fflush(stdout);
    computeReachability(me);
    //printf("d"); fflush(stdout);
    updateRegret(me);
    //printf("a"); fflush(stdout);
    updatePolicy(me, 1);
    //printf("te"); fflush(stdout);
}

void CFR::initCounts() {
    int i;
    
    for (i = 0; i < numNodes; i++)
        gameTree[i]->initCounts();
}

void CFR::reset() {
    int i;
    
    for (i = 0; i < numNodes; i++) {
        //gameTree[i]->utility[0] = gameTree[i]->utility[1] = -30;
        gameTree[i]->reachability[0] = gameTree[i]->reachability[1] = 0.0;
        gameTree[i]->visited[0] = gameTree[i]->visited[1] = false;
    }
}

void CFR::computeUtilities(int index) {
    int nivel, j;
    
    for (nivel = actualDepth-1; nivel >= 0; nivel--) {
        //printf("utilities: %i (%i to %i)\n", nivel, levelCounter[nivel], levelCounter[nivel+1]); fflush(stdout);
        for (j = levelCounter[nivel]; j < levelCounter[nivel+1]; j++) {
            if (isGoalState(gameTree[j]->s->ID) || (nivel == actualDepth-1)) {
                gameTree[j]->utility[index] = 0.0;
                continue;
            }
            
            //printf("%i ", j); fflush(stdout);
            gameTree[j]->computeUtility(index);
        }
    }
}

void CFR::computeReachability(int index) {
    int nivel, i, j, k;
    int jact;
    double pi;
    
    gameTree[0]->reachability[index] = 1.0;
    for (nivel = 0; nivel < (actualDepth-1); nivel++) {
        //printf("reachability: %i (%i to %i)\n", nivel, levelCounter[nivel], levelCounter[nivel+1]); fflush(stdout);
        for (i = levelCounter[nivel]; i < levelCounter[nivel+1]; i++) {
            //gameTree[i]->reachability[index] = 1.0;

            if (isGoalState(gameTree[i]->s->ID))
                continue;
            
            for (j = 0; j < gameTree[i]->s->numActions[0]; j++) {
                for (k = 0; k < gameTree[i]->s->numActions[1]; k++) {
                    jact = gameTree[i]->s->jointCode(j, k);
                    if (index == 0)
                        pi = gameTree[i]->sigma_t[1][k];
                    else
                        pi = gameTree[i]->sigma_t[0][j];
                    
                    if ((gameTree[i]->reachability[index] * pi) > gameTree[i]->next[jact]->reachability[index])
                        gameTree[i]->next[jact]->reachability[index] = gameTree[i]->reachability[index] * pi;
                        
                    //gameTree[i]->next[jact]->reachability[index] += gameTree[i]->reachability[index] * pi;
                    
                    //if (gameTree[i]->next[jact]->reachability[index] > 1.0) {
                    //    printf("%i: %lf (duplo = %i)\n", gameTree[i]->next[jact]->nID, gameTree[i]->next[jact]->reachability[index], gameTree[i]->next[jact]->duplo);
                    //}
                }
            }
        }
    }

    //printf("%i\n", i);
}

void CFR::updateRegret(int index) {
    int i;
    
    for (i = 0; i < levelCounter[actualDepth-1]/*numNodes*/; i++) {
        //printf("r for %i %i\n", i, gameTree[i]->s->ID); fflush(stdout);
        gameTree[i]->updateRegret(index);
    }
}

void CFR::updatePolicy(int index, int T) {
    int i;
    for (i = 0; i < levelCounter[actualDepth-1]/*numNodes*/; i++) {
        gameTree[i]->updatePolicy(index, T);
    }
    
    //if (index == 0) {
    //    gameTree[2350]->print(index, T);
    //    gameTree[604]->print(index, T);
    //}
    //else
    //    gameTree[1360]->print(index, T);
    //printf("\n");
}

int CFR::Move(GameEngine *ge) {
    ge->getValidActions(lastValidActs[0], 0);
    ge->getValidActions(lastValidActs[1], 1);
    State *s = ge->getState(mg, lastValidActs[0], lastValidActs[1]);
    if (cNode->s->ID != s->ID) {
        printf("que pasa? %i vs %i!\n", cNode->s->ID, s->ID); fflush(stdout);
        exit(1);
    }
    
    
    //printf("<%i>", cNode->s->numActions[me]); fflush(stdout);
    int b = generateAction(cNode->sigma_t[me], cNode->s->numActions[me]);
    //printf("<%i>", b); fflush(stdout);
    int a = ge->convertIt(b, lastValidActs[me], NUMACTIONS*2);
    //printf("<%i>", a); fflush(stdout);
    
    //printf("(%i<%i>: %i -> %i)  ", me, cNode->s->ID, b, a);
    
    //if (s->numActions[me] > 1)
    //    dlyr->delay(t, numMovemientos);
    
    return a;
}

int CFR::moveUpdate(GameEngine *ge, int actions[2], double dollars[2]) {
    int acts[2];
    ge->deduceActions(acts, actions, lastValidActs);
    int jact = cNode->s->jointCode(acts);
    
    //printf("(<%i,%i><%i,%i;%i><%i,%i;%i>)", actions[0], actions[1], acts[0], acts[1], jact,cNode->s->numActions[0],cNode->s->numActions[1],cNode->numNext); fflush(stdout);
    
    if (update)
        cNode->cuentita[acts[1-me]] += 1.0;
    
    cNode = cNode->next[jact];
    //printf("(%i, %i) produces cNode ID: %i\n", acts[0], acts[1], cNode->s->ID); fflush(stdout);
    
    numMovemientos++;    
    
    //printf("(State %i; Node %i;)\n", cNode->s->ID, cNode->nID); fflush(stdout);
    
    return 0;
}

int CFR::roundUpdate() {
    cNode = gameTree[0];
    
    if (update) {
        int i;
        
        for (i = 0; i < numNodes; i++) {
            gameTree[i]->updateSigmaNot();
        }
        
        updateMe();
    }
    
    numMovemientos = 0;
    t++;
    
    return 0;
}

int CFR::generateAction(double *v, int numActs) {
    int i;
    
    double num = rand() / (double)RAND_MAX;
    double sum = 0.0;
    for (i = 0; i < numActs; i++) {
        sum += v[i];
        if (num <= sum)
            return i;
    }
    
    //printStrategy(v, numActs);
    
    printf("CFR: generate action failed: %lf\n", num);
    
    return numActs-1;
}

/*
 void CFR::printGameTree(char gameName[1024]) {
 int i, j, k;
 char nombre[1024];
 sprintf(nombre, "cfr_%s.txt", gameName);
 FILE *fp = fopen(nombre, "w");
 
 fprintf(fp, "%i\n\n", actualDepth);
 for (i = 0; i < actualDepth; i++) {
 fprintf(fp, "%i\n", levelCounter[i+1]-levelCounter[i]);
 for (j = levelCounter[i]; j < levelCounter[i+1]; j++) {
 fprintf(fp, "%i\t", gameTree[j]->s->ID);
 for (k = 0; k < gameTree[j]->numNext; k++) {
 fprintf(fp, "%i\t", gameTree[j]->next[k]->s->ID);
 }
 fprintf(fp, "\n");
 }
 fprintf(fp, "\n");
 }
 
 fclose(fp);
 }
 
 // rather than derive the "tree structure" of the game from scratch, read it in from a file
 void CFR::readGameTree(char gameName[1024]) {
 int i, j, k, m;
 int levelNodes, n, dummy;
 int jact, lookedUp;
 char nombre[1024];
 sprintf(nombre, "cfr_%s.txt", gameName);
 
 // first, create all of the nodes in each level
 FILE *fp = fopen(nombre, "r");
 fscanf(fp, "%i", &actualDepth);
 levelCounter[0] = 0;
 for (i = 0; i < actualDepth; i++) {
 fscanf(fp, "%i", &levelNodes);
 for (j = 0; j < levelNodes; j++) {
 fscanf(fp, "%i", &n);
 gameTree[numNodes] = new Node(numNodes, me, mg->states[n], 1, isGoalState(mg->states[n]->ID));
 numNodes++;
 
 // read (but ignore for now) the connected nodes
 if (!isGoalState(mg->states[n]->ID)) {
 for (k = 0; k < mg->states[n]->numActions[0]; k++) {
 for (m = 0; m < mg->states[n]->numActions[1]; m++) {
 fscanf(fp, "%i", &dummy);
 }
 }
 }
 }
 levelCounter[i+1] = numNodes;
 }
 fclose(fp);
 
 printf("numNodes: %i\n", numNodes); fflush(stdout);
 
 // next, connect up the tree
 fp = fopen(nombre, "r");
 fscanf(fp, "%i", &actualDepth);
 for (i = 0; i < actualDepth; i++) {
 //printf("connecting at depth %i\n", i);
 fscanf(fp, "%i", &levelNodes);
 for (j = levelCounter[i]; j < levelCounter[i+1]; j++) {
 fscanf(fp, "%i", &n);
 if (!isGoalState(mg->states[n]->ID)) {
 for (k = 0; k < mg->states[n]->numActions[0]; k++) {
 for (m = 0; m < mg->states[n]->numActions[1]; m++) {
 fscanf(fp, "%i", &dummy);
 jact = mg->states[n]->jointCode(k, m);
 lookedUp = findNode(mg->states[dummy], i+1);
 gameTree[j]->next[jact] = gameTree[lookedUp];
 }
 }
 }
 }
 }
 fclose(fp);
 }
 */


