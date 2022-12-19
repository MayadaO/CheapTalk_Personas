#include "gameWriter.h"


gameWriter::gameWriter() {
    printf("incomplete gameWriter constructor\n");
    exit(1);
}

gameWriter::gameWriter(char *filename) {
    
    strcpy(fnombrito, filename);
    
    ge = new MazeEngine(filename);
    bruteGame();
}

gameWriter::~gameWriter() {
    
}

void gameWriter::bruteGame() {
	int i, j, k, m;
	bool *onthegoal = new bool[ge->numAgents];
	bool validActs1[NUMACTIONS], validActs2[NUMACTIONS];
	int numMovemientos = MAX_MOVES_PER_ROUND-1;
	bool verbose = false;
	int maxIterations = 1000;
    int nActions[2];
    
    MarkovGame *mg = new MarkovGame();
    State *currentState, *newState;
    double dollars[2];
    int acciones[2];
    
    int px1, px2, py1, py2, g1, g2, g3, gate;
    int a0, a1;
    
    
    ge->initRound();
    ge->getValidActions(validActs1, 0);
    ge->getValidActions(validActs2, 1);
    currentState = getState(mg, validActs1, validActs2);
    mg->addStartState(currentState);
    
    for (px1 = 1; px1 < ge->wWidth-1; px1++) {
        for (py1 = 1; py1 < ge->wHeight-1; py1++) {
            for (px2 = 1; px2 < ge->wWidth-1; px2++) {
                for (py2 = 1; py2 < ge->wHeight-1; py2++) {
                    for (gate = 0; gate < 4; gate++) {
                        switch (gate) {
                            case 0: g1 = 0; g2 = 0; g3 = 0; break;
                            case 1: g1 = 1; g2 = 0; g3 = 0; break;
                            case 2: g1 = 1; g2 = 1; g3 = 0; break;
                            case 3: g1 = 1; g2 = 1; g3 = 1; break;
                        }
                        
                        if ((!strcmp(ge->world[px1][py1],"w")) || (!strcmp(ge->world[px2][py2],"w")))
                            break;
                        
                        ge->setPos(px1, py1, px2, py2, g1, g2, g3);
                        ge->getValidActions(validActs1, 0);
                        ge->getValidActions(validActs2, 1);
                        currentState = getState(mg, validActs1, validActs2);
                        
                        if (ge->OnGoal(0) && ge->OnGoal(1))
                            break;
                        
                        
                        for (a0 = 0; a0 < currentState->numActions[0]; a0++) {
                            for (a1 = 0; a1 < currentState->numActions[1]; a1++) {
                                ge->setPos(px1, py1, px2, py2, g1, g2, g3);
                                ge->getValidActions(validActs1, 0);
                                ge->getValidActions(validActs2, 1);
                                currentState = getState(mg, validActs1, validActs2);
                                for (j = 0; j < ge->numAgents; j++) {
                                    onthegoal[j] = ge->OnGoal(j);
                                }
                                
                                //ge->outputGame(i);
                                
                                
                                //printf("<");
                                //for (j = 0; j < ge->numAgents; j++) {
                                //	ge->getValidActions(validActs, j);
                                //	ge->actions[j] = players[j]->Move(validActs, ge->OnGoal(j), verbose);
                                //}
                                
                                // keyboard input
                                //ge->actions[0] = keyboardInput(0);
                                //ge->actions[1] = keyboardInput(1);
                                
                                printf("(%i, %i), (%i, %i): ", px1, py1, px2, py2);
                                
                                jointExplore(currentState, validActs1, validActs2);
                                
                                acciones[0] = acciones[1] = 0;
                                for (m = 0; m < ge->actions[0]-1; m++) {
                                    if (validActs1[m])
                                        acciones[0]++;
                                }
                                for (m = 0; m < ge->actions[1]-1; m++) {
                                    if (validActs2[m])
                                        acciones[1]++;
                                }
                                
                                ge->MoveAgents();
                                
                                for (j = 0; j < ge->numAgents; j++) {
                                    if (!onthegoal[j]) {
                                        //printf("Player %i gets %lf\n", j, ge->getCurrentReward(j));
                                        dollars[j] = ge->getCurrentReward(j);
                                    }
                                    else {
                                        //players[j]->Update(ge->Pos, ge->actions, 0.0, true);
                                        dollars[j] = 0.0;
                                    }
                                }
                                
                                ge->getValidActions(validActs1, 0);
                                ge->getValidActions(validActs2, 1);
                                newState = getState(mg, validActs1, validActs2);
                                
                                currentState->addTransition(newState, acciones, dollars);
                                
                                bool allonit = true;
                                
                                for (j = 0; j < ge->numAgents; j++) {
                                    if (!ge->OnGoal(j))
                                        allonit = false;
                                    else
                                        onthegoal[j] = true;
                                }
                                
                                if (allonit) {
                                    mg->addGoalState(newState);
                                    //break;
                                }
                            }
                        }
                        //if (currentState->ID == 26315) {
                        //    printf("found it\n");
                        //    exit(1);
                        //}
                        
                    }
                }
            }
        }
    }
    
    char buf[1024];
    strcpy(buf, "MG_prisoners.txt");
    mg->writeGame(buf);
    
    delete mg;
}

int gameWriter::keyboardInput(int index) {
    char ch[1024];
    
    printf("enter move for player %i ... ", index);
    scanf("%s", ch);
    int dir;
    switch (ch[0]) {
        case 'i': dir = 1; break;
        case 'j': dir = 4; break;
        case 'k': dir = 2; break;
        case 'l': dir = 3; break;
        default: dir = 0; break;
    }

    return dir;
}

State *gameWriter::getState(MarkovGame *mg, bool validActs1[NUMACTIONS], bool validActs2[NUMACTIONS]) {
    int m;
    double fvector[7];
    int nActions[2];
    
    getStateFeatureVector(fvector);
    nActions[0] = nActions[1] = 0;
    for (m = 0; m < NUMACTIONS; m++) {
        if (validActs1[m])
            nActions[0]++;
        if (validActs2[m])
            nActions[1]++;
    }
    
    //printf("state getState?\n"); fflush(stdout);
    
    return mg->getState(7, fvector, nActions);
}

void gameWriter::getStateFeatureVector(double fvector[7]) {
    fvector[0] = ge->Pos[0][0];
    fvector[1] = ge->Pos[0][1];
    fvector[2] = ge->Pos[1][0];
    fvector[3] = ge->Pos[1][1];
    
    if (!strcmp(fnombrito, "prisoners_small")) {
        if (ge->currentWorld[3][7][0] == '0')
            fvector[4] = 0;
        else
            fvector[4] = 1;
        if (ge->currentWorld[3][5][0] == '0')
            fvector[5] = 0;
        else
            fvector[5] = 1;
        if (ge->currentWorld[3][3][0] == '0')
            fvector[6] = 0;
        else
            fvector[6] = 1;
    }
    else {
        if (ge->currentWorld[4][12][0] == '0')
            fvector[4] = 0;
        else
            fvector[4] = 1;
        if (ge->currentWorld[4][9][0] == '0')
            fvector[5] = 0;
        else
            fvector[5] = 1;
        if (ge->currentWorld[4][5][0] == '0')
            fvector[6] = 0;
        else
            fvector[6] = 1;
    }
    
    //printf("fvector: %.0lf %.0lf %.0lf %.0lf %.0lf %.0lf %.0lf\n", fvector[0], fvector[1], fvector[2], fvector[3], fvector[4], fvector[5], fvector[6]);
}

void gameWriter::jointExplore(State *cState, bool validActs1[NUMACTIONS], bool validActs2[NUMACTIONS]) {
    int i, j;
    int acts[2] = {-1, -1}, jointAct;
    
    bool done = false;
    for (i = 0; i < cState->numActions[0]; i++) {
        if (done)
            break;
        for (j = 0; j < cState->numActions[1]; j++) {
            jointAct = cState->jointCode(i, j);
            if (cState->nextState[jointAct][0] == NULL) {
                acts[0] = i;
                acts[1] = j;
                done = true;
                break;
            }
        }
    }
    
    printf("jointExplore: %i, %i\n", acts[0], acts[1]);
    
    if (acts[0] < 0) {
        acts[0] = rand() % cState->numActions[0];
        acts[1] = rand() % cState->numActions[1];
    }

    int c0 = 0, c1 = 0;
    for (i = 0; i < NUMACTIONS; i++) {
        if (validActs1[i])
            c0++;
        c1 = 0;
        for (j = 0; j < NUMACTIONS; j++) {
            if (validActs2[j]) {
                c1++;
                
                if (((c0-1) == acts[0]) && ((c1-1) == acts[1])) {
                    ge->actions[0] = i+1;
                    ge->actions[1] = j+1;
                    
                    if (ge->actions[0] > NUMACTIONS)
                        ge->actions[0] = 0;
                    if (ge->actions[1] > NUMACTIONS)
                        ge->actions[1] = 0;
                    
                    //printf("translated: %i, %i\n", ge->actions[0], ge->actions[1]);
                    return;
                }
            }
        }
    }
    

    
    printf("joint action selection problem\n");
    printf("numActions[0] = %i; numActions[1] = %i\n", cState->numActions[0], cState->numActions[1]);
    for (i = 0; i < 5; i++) {
        printf("%i", (int)(validActs1[i]));
    }
    printf("\n");
    for (i = 0; i < 5; i++) {
        printf("%i", (int)(validActs2[i]));
    }
    printf("\n");
    
    exit(1);
}
