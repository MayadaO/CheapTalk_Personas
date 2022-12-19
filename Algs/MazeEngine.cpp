#include "MazeEngine.h"

MazeEngine::MazeEngine() {
}

MazeEngine::MazeEngine(char *filename) {
	wallPenalty = 0.0;
	stochastic = 1.0;
	oneGoalNeeded = false;
    
    strcpy(fnombrito, filename);
	ReadTheFile(filename);
    
	actions = new int[numAgents];
	numMoves = new int[numAgents];
	Pos = new int*[numAgents];
	oldPos = new int*[numAgents];
	sPos = new int*[numAgents];
	rewardMove = new double[numAgents];
	hitWall = new bool[numAgents];
	for (int i = 0; i < numAgents; i++) {
		Pos[i] = new int[2];
		sPos[i] = new int[2];
		oldPos[i] = new int[2];
		numMoves[i] = 0;
		rewardMove[i] = 0.0;
	}
	
	possession = 0;

	GetStartPositions();
}

MazeEngine::~MazeEngine() {
	int i, j;

	for (i = 0; i < wWidth; i++) {
		for (j = 0; j < wHeight; j++) {
			delete world[i][j];
			delete currentWorld[i][j];
		}
		delete world[i];
		delete currentWorld[i];
	}
	delete world;
	delete currentWorld;

	if (pR != NULL) {
		for (i = 0; i < numRules; i++)
			delete pR[i];
		delete pR;
	}

	delete actions;
	delete numMoves;
	for (i = 0; i < numAgents; i++) {
		delete Pos[i];
		delete oldPos[i];
		delete sPos[i];
	}
	delete Pos;
	delete oldPos;
	delete sPos;
	delete rewardMove;
	delete hitWall;
}


void MazeEngine::playMinimax(MarkovGame *mg) {
    int i, j, k;
    int iters, moves;
	bool *onthegoal = new bool[numAgents];
	bool validActs1[NUMACTIONS], validActs2[NUMACTIONS];
	int numMovemientos = MAX_MOVES_PER_ROUND-1;
	bool verbose = false;
	int maxIterations = 1;
    int acciones[2];
    
    State *currentState;
    
    for (iters = 0; iters < maxIterations; iters++) {
        
        initRound();
        for (i = 0; i < numAgents; i++)
            onthegoal[i] = false;
        
        for (moves = 0; moves < numMovemientos; moves++) {
            getValidActions(validActs1, 0);
            getValidActions(validActs2, 1);
            currentState = getState(mg, validActs1, validActs2);
            
            outputGame(i);
            
            pickMinimax(currentState, validActs1, validActs2);
            
            MoveAgents();
            
            bool allonit = true;
            
            for (j = 0; j < numAgents; j++) {
                if (!OnGoal(j))
                    allonit = false;
                else
                    onthegoal[j] = true;
            }
            
            if (allonit) {
                break;
            }
        }
    }
}


void MazeEngine::playGame(MarkovGame *mg, double _w) {
    int i, j, k;
    int iters, moves;
	bool *onthegoal = new bool[numAgents];
	bool validActs1[NUMACTIONS], validActs2[NUMACTIONS];
	int numMovemientos = MAX_MOVES_PER_ROUND-1;
	bool verbose = false;
	int maxIterations = 1;
    int acciones[2];
    
    State *currentState;
    
    for (iters = 0; iters < maxIterations; iters++) {
        
        initRound();
        for (i = 0; i < numAgents; i++)
            onthegoal[i] = false;
        
        for (moves = 0; moves < numMovemientos; moves++) {
            getValidActions(validActs1, 0);
            getValidActions(validActs2, 1);
            currentState = getState(mg, validActs1, validActs2);
            
            outputGame(i);
            
            pickJointAction(currentState, validActs1, validActs2, _w);
            
            MoveAgents();
            
            bool allonit = true;
            
            for (j = 0; j < numAgents; j++) {
                if (!OnGoal(j))
                    allonit = false;
                else
                    onthegoal[j] = true;
            }
            
            if (allonit) {
                break;
            }
        }
    }
}

void MazeEngine::pickJointAction(State *s, bool validActs1[NUMACTIONS], bool validActs2[NUMACTIONS], double _w) {
    int i, j;
    int acts[2];
    double mx = -99999;
    
    int qind = s->findQset(_w);
    
    printf("Combined: \n");
    for (i = 0; i < s->numActions[0]; i++) {
        for (j = 0; j < s->numActions[1]; j++) {
            printf("%.2lf\t", s->qsets[qind]->Qs[i][j]);
        }
        printf("\n");
    }

    double v1, v2;
    printf("Separate: \n");
    for (i = 0; i < s->numActions[0]; i++) {
        for (j = 0; j < s->numActions[1]; j++) {
            v1 = s->qsets[qind]->M[i][j][0] - s->mm[0]->mv;
            v2 = s->qsets[qind]->M[i][j][1] - s->mm[1]->mv;
            printf("%.2lf, %.2lf (%.2lf)\t", v1, v2, v1 * 0.5 + v2 * 0.5);
        }
        printf("\n");
    }

    
    for (i = 0; i < s->numActions[0]; i++) {
        for (j = 0; j < s->numActions[1]; j++) {
            if (s->qsets[qind]->Qs[i][j] > mx) {
                mx = s->qsets[qind]->Qs[i][j];
                acts[0] = i;
                acts[1] = j;
            }
            else if ((s->qsets[qind]->Qs[i][j] == mx) && (rand() % 2)) {
                acts[0] = i;
                acts[1] = j;
            }
        }
    }

    convertActions(acts, validActs1, validActs2);
}

void MazeEngine::pickMinimax(State *s, bool validActs1[NUMACTIONS], bool validActs2[NUMACTIONS]) {
    int i, j;
    int acts[2];
    double mx = -99999;
    
    for (j = 0; j < 2; j++) {
        printf("%i: %.2lf; ", j, s->mm[j]->mv);
        for (i = 0; i < s->numActions[j]; i++)
            printf("%.2lf ", s->mm[j]->ms[i]);
        printf("\n");
    }
    
    acts[0] = acts[1] = -1;
    double val = rand() / (double)(RAND_MAX);
    double sum = 0.0;
    for (i = 0;i < s->numActions[0]; i++) {
        sum += s->mm[0]->ms[i];
        if (sum >= val) {
            acts[0] = i;
            break;
        }
    }
    
    val = rand() / (double)(RAND_MAX);
    sum = 0.0;
    for (i = 0;i < s->numActions[1]; i++) {
        sum += s->mm[1]->ms[i];
        if (sum >= val) {
            acts[1] = i;
            break;
        }
    }
    
    if ((acts[0] < 0) || (acts[1] < 0)) {
        printf("got a problem\n");
        exit(1);
    }
    
    convertActions(acts, validActs1, validActs2);
}

void MazeEngine::convertActions(int acts[2], bool validActs1[NUMACTIONS], bool validActs2[NUMACTIONS]) {
    int i, j;
    
    printf("selection: %i, %i\n", acts[0], acts[1]);
    
    int c0 = 0, c1 = 0;
    for (i = 0; i < NUMACTIONS; i++) {
        if (validActs1[i])
            c0++;
        c1 = 0;
        for (j = 0; j < NUMACTIONS; j++) {
            if (validActs2[j]) {
                c1++;
                
                if (((c0-1) == acts[0]) && ((c1-1) == acts[1])) {
                    actions[0] = i+1;
                    actions[1] = j+1;
                    
                    if (actions[0] > NUMACTIONS)
                        actions[0] = 0;
                    if (actions[1] > NUMACTIONS)
                        actions[1] = 0;
                    
                    printf("translated: %i, %i\n", actions[0], actions[1]);
                    return;
                }
            }
        }
    }
}

State *MazeEngine::getState(MarkovGame *mg, bool *validActs1, bool *validActs2) {
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
    
    return mg->getState(7, fvector, nActions);
}

void MazeEngine::getStateFeatureVector(double fvector[7]) {
    fvector[0] = Pos[0][0];
    fvector[1] = Pos[0][1];
    fvector[2] = Pos[1][0];
    fvector[3] = Pos[1][1];
    
    if (!strcmp(fnombrito, "prisoners_small")) {
        if (currentWorld[3][7][0] == '0')
            fvector[4] = 0;
        else
            fvector[4] = 1;
        if (currentWorld[3][5][0] == '0')
            fvector[5] = 0;
        else
            fvector[5] = 1;
        if (currentWorld[3][3][0] == '0')
            fvector[6] = 0;
        else
            fvector[6] = 1;
    }
    else {
        if (currentWorld[4][12][0] == '0')
            fvector[4] = 0;
        else
            fvector[4] = 1;
        if (currentWorld[4][9][0] == '0')
            fvector[5] = 0;
        else
            fvector[5] = 1;
        if (currentWorld[4][5][0] == '0')
            fvector[6] = 0;
        else
            fvector[6] = 1;
    }
    
    //printf("fvector: %.0lf %.0lf %.0lf %.0lf %.0lf %.0lf %.0lf\n", fvector[0], fvector[1], fvector[2], fvector[3], fvector[4], fvector[5], fvector[6]);
}



void MazeEngine::MoveAgents() {
	//printf("pos: (%i, %i); (%i, %i); actions: %i, %i\n", Pos[0][0], Pos[0][1], Pos[1][0], Pos[1][1], actions[0], actions[1]);
	
	int i, j;
	bool *executed = new bool[numAgents];

	for (i = 0; i < numAgents; i++) {
		executed[i] = false;
		if (!OnGoal(i))
			numMoves[i] ++;
		oldPos[i][0] = Pos[i][0];
		oldPos[i][1] = Pos[i][1];
	}

	for (i = 0; i < numPhysicsRules; i++) {
		if ((pR[i]->CheckConditions(Pos, actions, currentWorld)) && (pR[i]->type == PRE)) {
			//printf("physics rule passed: %i\n", i);
			//printf("pos: (%i, %i); (%i, %i); actions: %i, %i\n", Pos[0][0], Pos[0][1], Pos[1][0], Pos[1][1], actions[0], actions[1]);
			// execute each resultado
			for (j = 0; j < pR[i]->numResultados; j++) {
				if (pR[i]->Resultados[j][0] == 'c') {
					int x, y, index;
					index = ReadCoord(pR[i]->Resultados[j] + 1, &x, &y) + 1;
					index ++;
					strcpy(currentWorld[x][y], pR[i]->Resultados[j] + index);
				}
				else if (pR[i]->Resultados[j][0] == 'p') {
					int index = 1, numero;
					numero = atoi(pR[i]->Resultados[j] + 1);

					if (!executed[numero]) {
						while (pR[i]->Resultados[j][++index] != '=');
						index ++;
						ReadCoord(pR[i]->Resultados[j] + index, &(Pos[numero][0]), &(Pos[numero][1]));
						executed[numero] = true;
					}
				}
				else if (pR[i]->Resultados[j][0] == '?') {
					int index = 2;
					int val = atoi(pR[i]->Resultados[j] + index);
					while (pR[i]->Resultados[j][++index] != ';');
					index ++;
					if ((rand() % 100) >= val) {
						while (pR[i]->Resultados[j][++index] != ';');
						index ++;
					}
					int numero = atoi(pR[i]->Resultados[j] + index + 1);
					if (!executed[numero]) {
						while (pR[i]->Resultados[j][++index] != '=');
						index ++;
						ReadCoord(pR[i]->Resultados[j] + index, &(Pos[numero][0]), &(Pos[numero][1]));
						executed[numero] = true;
					}
				}
				else {
					printf("i don't understand this physics resultado\n");//: %s\n", pR[i]->Resultados);
				}
			}
		}
	}

	// move agents according to normal
	//	they bounce off walls
	//	for now, assume that they may occupy the same square as another agent
	int nPos[2];
	int rAction;
	for (i = 0; i < numAgents; i++) {
		hitWall[i] = false;

		if (executed[i])
			continue;

		nPos[0] = Pos[i][0];
		nPos[1] = Pos[i][1];

		rAction = getActualAction(i, actions[i]);
		
		switch (rAction) {
			case 1: nPos[1] --; break;
			case 2: nPos[1] ++; break;
			case 3: nPos[0] ++; break;
			case 4: nPos[0] --; break;
		}
		if (currentWorld[nPos[0]][nPos[1]][0] != 'w') {
			Pos[i][0] = nPos[0];
			Pos[i][1] = nPos[1];
		}
		else
			hitWall[i] = true;
	}

	for (i = 0; i < numPhysicsRules; i++) {
		if ((pR[i]->CheckConditions(Pos, actions, currentWorld)) && (pR[i]->type == POST)) {
			// execute each resultado
			for (j = 0; j < pR[i]->numResultados; j++) {
				if (pR[i]->Resultados[j][0] == 'c') {
					int x, y, index;
					index = ReadCoord(pR[i]->Resultados[j] + 1, &x, &y) + 1;
					index ++;
					strcpy(currentWorld[x][y], pR[i]->Resultados[j] + index);
				}
				else if (pR[i]->Resultados[j][0] == 'p') {
					int index = 1, numero;
					numero = atoi(pR[i]->Resultados[j] + 1);
					while (pR[i]->Resultados[j][++index] != '=');
					index ++;
					ReadCoord(pR[i]->Resultados[j] + index, &(Pos[numero][0]), &(Pos[numero][1]));
					executed[numero] = true;
				}
				else if (pR[i]->Resultados[j][0] == 'e') {  // two agents on same cell
					int index, n1, n2;
					index = ReadCoord(pR[i]->Resultados[j] + 1, &n1, &n2);
					if ((Pos[n1][0] == Pos[n2][0]) && (Pos[n1][1] == Pos[n2][1])) {
						while (pR[i]->Resultados[j][index++] != '=');
						if (pR[i]->Resultados[j][index] == 'r') {
							Pos[n1][0] = oldPos[n1][0];
							Pos[n1][1] = oldPos[n1][1];
							Pos[n2][0] = oldPos[n2][0];
							Pos[n2][1] = oldPos[n2][1];
							chocaron = true;
						}
					}
					else {
						chocaron = false;
					}
				}
				else {
					printf("i don't understand this physics resultado\n");//: %s\n", pR[i]->Resultados);
				}
			}	
		}
	}
    
    //printf("pos: (%i, %i); (%i, %i)\n", Pos[0][0], Pos[0][1], Pos[1][0], Pos[1][1]);

	delete[] executed;
}

bool MazeEngine::OnGoal(int index) {
	char chara[3];
	if (world[Pos[index][0]][Pos[index][1]][0] == 's') {
		sprintf(chara, "%i", 1-index);
		if (world[Pos[index][0]][Pos[index][1]][1] == chara[0]) {  // handles up to two agents
			return true;
		}
/*		else if (strlen(world[Pos[index][0]][Pos[index][1]]) > 2) {
			if (world[Pos[index][0]][Pos[index][1]][2] == chara[0]) {
				return true;
			}
		}
*/	}

	return false;
}

void MazeEngine::initRound() {
	int i, j;

	for (i = 0; i < numAgents; i++) {
		numMoves[i] = 0;

		Pos[i][0] = sPos[i][0];
		Pos[i][1] = sPos[i][1];
	}

	// set the current world
	for (i = 0; i < wWidth; i++) {
		for (j = 0; j < wHeight; j++) {
			if (world[i][j][0] == 'w')
				strcpy(currentWorld[i][j], "w");
			else
				strcpy(currentWorld[i][j], "0");
		}
	}
}

void MazeEngine::setPos(int px1, int py1, int px2, int py2, int g1, int g2, int g3) {
	int i, j;
    
	for (i = 0; i < numAgents; i++) {
		numMoves[i] = 0;
    }
    
    Pos[0][0] = px1;
	Pos[0][1] = py1;
    Pos[1][0] = px2;
	Pos[1][1] = py2;
    
	// set the current world
	for (i = 0; i < wWidth; i++) {
		for (j = 0; j < wHeight; j++) {
			if (world[i][j][0] == 'w')
				strcpy(currentWorld[i][j], "w");
			else
				strcpy(currentWorld[i][j], "0");
		}
	}
    
    if (!strcmp(fnombrito, "prisoners_small")) {
        if (g1)
            strcpy(currentWorld[3][7], "w");
        if (g2)
            strcpy(currentWorld[3][5], "w");
        if (g3)
            strcpy(currentWorld[3][3], "w");
    }
    else {
        if (g1)
            strcpy(currentWorld[4][12], "w");
        if (g2)
            strcpy(currentWorld[4][9], "w");
        if (g3)
            strcpy(currentWorld[4][5], "w");
    }
}



void MazeEngine::ReadTheFile(char *filename) {
	char filenombre[500];

	sprintf(filenombre, "../../Games/%s.txt", filename);
	//printf("%s\n", filenombre);

	FILE *fp = fopen(filenombre, "r");

	if (fp == NULL) {
		// check in an alternate directory before giving up
		sprintf(filenombre, "../Games/%s.txt", filename);
        //printf("%s\n", filenombre);
		fp = fopen(filenombre, "r");
		if (fp == NULL) {
			printf("file %s not found\n", filename);
			exit(1);
		}
	}

	int i, j;

	fscanf(fp, "%i %i %i", &numAgents, &wWidth, &wHeight);
	world = new char**[wWidth];
	currentWorld = new char**[wWidth];
	for (i = 0; i < wWidth; i++) {
		world[i] = new char*[wHeight];
		currentWorld[i] = new char*[wHeight];
		for (j = 0; j < wHeight; j++) {
			world[i][j] = new char[10];
			currentWorld[i][j] = new char[10];
		}
	}


	for (i = 0; i < wHeight; i++) {
		for (j = 0; j < wWidth; j++) {
			fscanf(fp, "%s", world[j][i]);
		}
	}

	char tmp[100];

	fscanf(fp, "%s", tmp);  // "rules"
	if (tmp[0] == 'o') {
		oneGoalNeeded = true;
		fscanf(fp, "%s", tmp);  // "rules"
	}

	fscanf(fp, "%i", &numRules);
	fscanf(fp, "%s", tmp);  // "physics"
	fscanf(fp, "%i", &numPhysicsRules);

	if (numRules != 0)
		pR = new PhysicsRule*[numRules];
	else
		pR = NULL;

	for (i = 0; i < numPhysicsRules; i++) {
		fscanf(fp, "%s", tmp);
		pR[i] = new PhysicsRule(tmp, numAgents);

		fscanf(fp, "%s", pR[i]->precondition);
		fscanf(fp, "%s", tmp);
		pR[i]->ReadCell(tmp);
		fscanf(fp, "%s", tmp);
		pR[i]->ReadActionSelection(tmp);
		fscanf(fp, "%s", tmp);
		pR[i]->ReadActionSelection(tmp);

		int itmp, j;
		fscanf(fp, "%i", &itmp);
		pR[i]->initResultados(itmp);

		for (j = 0; j < itmp; j++) {
			fscanf(fp, "%s", pR[i]->Resultados[j]);
		}
	}

	fscanf(fp, "%s", tmp);  // "payoffs"
	fscanf(fp, "%i", &numPayoffRules);

    char buf[1024];
	for (i = numPhysicsRules; i < (numPhysicsRules + numPayoffRules); i++) {
        strcpy(buf, "payoff");
		pR[i] = new PhysicsRule(buf, numAgents);

		fscanf(fp, "%s", tmp);
		pR[i]->ReadCell(tmp);

		fscanf(fp, "%i %lf", &(pR[i]->agente), &(pR[i]->valu));
	}

	fscanf(fp, "%s", tmp);  // "actions" or "wall"

	if (tmp[0] == 'w') {
		fscanf(fp, "%lf", &wallPenalty);
		printf("there is a wall penalty of %lf\n", wallPenalty);
		fscanf(fp, "%s", tmp);  // "actions" or "stochastic"
		if (tmp[0] == 's') {
			fscanf(fp, "%lf", &stochastic);
			fscanf(fp, "%s", tmp);  // "actions"

		}
		else {
			stochastic = 1.0;
		}
	}

	fscanf(fp, "%i", &numActionRules);

	for (i = (numPhysicsRules + numPayoffRules); i < numRules; i++) {
        strcpy(buf, "action");
		pR[i] = new PhysicsRule(buf, numAgents);

		fscanf(fp, "%s", tmp);
		pR[i]->ReadCell(tmp);

		int a;
		fscanf(fp, "%i", &a);
		fscanf(fp, "%s", tmp);
		pR[i]->SetAvailableActions(a, tmp);
	}

	fclose(fp);
}

void MazeEngine::GetStartPositions() {
	int i, j, k;
	char chara[3];

	for (i = 0; i < wWidth; i++) {
		for (j = 0; j < wHeight; j++) {
			if (world[i][j][0] == 's') {
				for (k = 0; k < numAgents; k++) {
					sprintf(chara, "%i", k);
					if (world[i][j][1] == chara[0]) {  // handles up to two agents for now
						sPos[k][0] = i;
						sPos[k][1] = j;
					}
					else if (strlen(world[i][j]) > 2) {
						if (world[i][j][2] == chara[0]) {
							sPos[k][0] = i;
							sPos[k][1] = j;
						}
					}
				}
			}
		}
	}
}

int MazeEngine::ReadCoord(char *strng, int *x, int *y) {
	int index = 1;

	*x = atoi(strng + index);
	while (strng[++index] != ',');
	index ++;
	*y = atoi(strng + index);
	while (strng[++index] != ')');
	index ++;

	return index;
}

void MazeEngine::getValidActions(bool *validActs, int index) {
	int i, j;

    //printf("num action rules: %i\n", numRules - (numPhysicsRules + numPayoffRules));    
    bool rulefound = false;
	for (i = (numPhysicsRules + numPayoffRules); i < numRules; i++) {
    //    printf("%i, %i, %i, %i\n", pR[i]->Posi[0][0], pR[i]->Posi[0][1], pR[i]->Posi[1][0], pR[i]->Posi[1][1]);
		if (pR[i]->CheckConditions(Pos, index, currentWorld)) {
    //        printf("found\n");
            //printf("Rule %i\n", i);
            rulefound = true;
			for (j = 0; j < NUMACTIONS; j++) {
				validActs[j] = pR[i]->viable[j];
			}
		}
	}
    
    if (!rulefound) {
        printf("Pos: %i, %i, %i, %i\n", Pos[0][0], Pos[0][1], Pos[1][0], Pos[1][1]);
        printf("index = %i\n", index);
        printf("we have a problem\n");
        exit(1);
    }
}

void MazeEngine::getValidActions(int **Posicion, bool validActs[NUMACTIONS], int index) {
	int i, j;

	for (i = (numPhysicsRules + numPayoffRules); i < numRules; i++) {
		if (pR[i]->CheckConditions(Posicion, index, currentWorld)) {
			for (j = 0; j < NUMACTIONS; j++) {
				validActs[j] = pR[i]->viable[j];
			}
		}
	}
}

void MazeEngine::getNumActions(int *nActs) {
	int i, j;
	bool valids[NUMACTIONS];
	int count;

	for (i = 0; i < numAgents; i++) {
		getValidActions(valids, i);
		count = 0;
		for (j = 0; j < NUMACTIONS; j++) {
			if (valids[j])
				count ++;
		}
		nActs[i] = count;
	}
}

double MazeEngine::getCurrentReward(int index) {
	int i;
	double reward = 0.0;

	if (hitWall[index])
		reward += wallPenalty;
	if (chocaron && oneGoalNeeded)
		reward -= 1;

	for (i = numPhysicsRules; i < (numPhysicsRules + numPayoffRules); i++) {
        //printf("i-");
		if (pR[i]->CheckConditions(Pos, index, currentWorld)) {
        //    printf("rule: %i\n", i);
			reward += pR[i]->valu;
		}
	}

	rewardMove[index] = reward;

	return reward;
}

void MazeEngine::outputGame(int iter) {
	int i, j, k;
	bool b;

	//system("clear");

	for (i = 0; i < wHeight; i++) {
		for (j = 0; j < wWidth; j++) {
			switch (currentWorld[j][i][0]) {
			case 'w':
				b = true;
				for (k = 0; k < numAgents; k++) {
					if ((i != Pos[k][1]) || (j != Pos[k][0]))
						b = false;
				}
				if (b) {
					printf("A");
					break;
				}
				
				b = true;
				for (k = 0; k < numAgents; k++) {
					if ((i == Pos[k][1]) && (j == Pos[k][0])) {
						printf("%i", k);
						b = false;
						break;
					}
				}

				if (b)
					printf("%c", 219); 
				break;
			default: 
				b = true;
				for (k = 0; k < numAgents; k++) {
					if ((i != Pos[k][1]) || (j != Pos[k][0]))
						b = false;
				}
				if (b) {
					printf("A");
					break;
				}
				
				b = true;
				for (k = 0; k < numAgents; k++) {
					if ((i == Pos[k][1]) && (j == Pos[k][0])) {
						printf("%i", k);
						b = false;
						break;
					}
				}

				if (b)
					printf("%c", ' '); 
				break;
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("\nIteration: %i\n", iter);
	printf("Move Reward: (");
	for (i = 0; i < numAgents; i++) {
		printf("%.0lf, ", rewardMove[i]);
	}
	printf("\b\b)\n");
}

int MazeEngine::getActualAction(int index, int intendedAct) {
	int i;
	bool validActs[NUMACTIONS];

	getValidActions(validActs, index);

	int num = rand() % 1000;

	if (num < (stochastic * 1000.0)) {
		return intendedAct;
	}

	int counter = 0;
	for (i = 0; i < NUMACTIONS; i++) {
		if (validActs[i] && (i != intendedAct))
			counter ++;
	}

	if (counter == 0)
		return intendedAct;

	num = rand() % counter;
	counter = 0;
	for (i = 0; i < NUMACTIONS; i++) {
		if (validActs[i] && (i != intendedAct)) {
			if (counter == num)
				return i;
			counter ++;
		}
	}

	return -1;
}


/* *************************************************************
//
//		Soccer Game Stuff Only
//
// *************************************************************/
void MazeEngine::outputGameSoccer(int iter, int *score) {
	int i, j, k;
	bool b;

	//system("clear");

	for (i = 0; i < wHeight; i++) {
		for (j = 0; j < wWidth; j++) {
			switch (world[j][i][0]) {
			case 'w':
				b = true;
				for (k = 0; k < numAgents; k++) {
					if ((i != Pos[k][1]) || (j != Pos[k][0]))
						b = false;
				}
				
				b = true;
				for (k = 0; k < numAgents; k++) {
					if ((i == Pos[k][1]) && (j == Pos[k][0])) {
						if (possession == k) {
							if (k == 0)
								printf("A");
							else
								printf("B");
						}
						else {
							if (k == 0)
								printf("a");
							else
								printf("b");
						}
						b = false;
						break;
					}
				}

				if (b)
					printf("%c", 219); 
				break;
			default: 
				b = true;
				for (k = 0; k < numAgents; k++) {
					if ((i != Pos[k][1]) || (j != Pos[k][0]))
						b = false;
				}
				
				b = true;
				for (k = 0; k < numAgents; k++) {
					if ((i == Pos[k][1]) && (j == Pos[k][0])) {
						if (possession == k) {
							if (k == 0)
								printf("A");
							else
								printf("B");
						}
						else {
							if (k == 0)
								printf("a");
							else
								printf("b");
						}
						b = false;
						break;
					}
				}

				if (b)
					printf("%c", ' '); 
				break;
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("\nIteration: %i\n", iter);
	printf("Move Reward: (");
	for (i = 0; i < numAgents; i++) {
		printf("%.0lf, ", rewardMove[i]);
	}
	printf("\b\b)\n");
	printf("score = (%i, %i)\n", score[0], score[1]);
}

void MazeEngine::MoveAgentsSoccer(){
	int first = rand() % 2;
	int nPos[2];
	bool notMove[2];
	
	notMove[0] = notMoveQuestion(0, actions[0], Pos[0][0], Pos[0][1]);
	notMove[1] = notMoveQuestion(1, actions[1], Pos[1][0], Pos[1][1]);

	nPos[0] = Pos[first][0];
	nPos[1] = Pos[first][1];
	int rAction = getActualAction(first, actions[first]);

	switch (rAction) {
		case 1: nPos[1] --; break;
		case 2: nPos[1] ++; break;
		case 3: nPos[0] ++; break;
		case 4: nPos[0] --; break;
	}
	
	//printf("(%i, %i) vs. (%i, %i)\n", nPos[0], nPos[1], Pos[1-first][0], Pos[1-first][1]);
	if ((nPos[0] == Pos[1-first][0]) && (nPos[1] == Pos[1-first][1])) {
		//printf("changing possession\n");		
		if (possession == first) {
			possession = 1-possession;
			if (possession == 0)
				strcpy(currentWorld[0][0], "0");
			else
				strcpy(currentWorld[0][0], "1");

			return;
		}
		else if (notMove[1-first]) {
			// change possession about 20% of the time
			//if ((rand() % 5) == 0) {
				possession = first;
				if (possession == 0)
					strcpy(currentWorld[0][0], "0");
				else
					strcpy(currentWorld[0][0], "1");

				return;
			//}
		}
	}
	else if (currentWorld[nPos[0]][nPos[1]][0] != 'w') {
		Pos[first][0] = nPos[0];
		Pos[first][1] = nPos[1];
	}

	first = 1-first;
	nPos[0] = Pos[first][0];
	nPos[1] = Pos[first][1];
	rAction = getActualAction(first, actions[first]);

	switch (rAction) {
		case 1: nPos[1] --; break;
		case 2: nPos[1] ++; break;
		case 3: nPos[0] ++; break;
		case 4: nPos[0] --; break;
	}

	//printf("(%i, %i) vs. (%i, %i)\n", nPos[0], nPos[1], Pos[1-first][0], Pos[1-first][1]);	
	if ((nPos[0] == Pos[1-first][0]) && (nPos[1] == Pos[1-first][1])) {
		//printf("changing possession\n");
		if (possession == first) {
			possession = 1-possession;
			if (possession == 0)
				strcpy(currentWorld[0][0], "0");
			else
				strcpy(currentWorld[0][0], "1");

			return;
		}
		else if (notMove[1-first]) {
			// change possession about 20% of the time
			//if ((rand() % 5) == 0) {
				possession = first;
				if (possession == 0)
					strcpy(currentWorld[0][0], "0");
				else
					strcpy(currentWorld[0][0], "1");

				return;
			//}
		}
	}
	else if (currentWorld[nPos[0]][nPos[1]][0] != 'w') {
		Pos[first][0] = nPos[0];
		Pos[first][1] = nPos[1];
	}
}

bool MazeEngine::notMoveQuestion(int index, int a, int x, int y) {
	int rAction = getActualAction(index, a);
	
	switch (rAction) {
		case 1: y --; break;
		case 2: y ++; break;
		case 3: x ++; break;
		case 4: x --; break;
	}
	
	if (currentWorld[x][y][0] == 'w')
		return true;
	
	return false;
}

void MazeEngine::initRoundSoccer() {
	int i, j;

	for (i = 0; i < numAgents; i++) {
		numMoves[i] = 0;

		Pos[i][0] = sPos[i][0];
		Pos[i][1] = sPos[i][1];
	}

	// set the current world
	for (i = 0; i < wWidth; i++) {
		for (j = 0; j < wHeight; j++) {
			if (world[i][j][0] == 'w')
				strcpy(currentWorld[i][j], "w");
			else
				strcpy(currentWorld[i][j], "0");
		}
	}

	possession = 1-possession;//rand() % 2;
	if (possession == 0)
		strcpy(currentWorld[0][0], "0");
	else
		strcpy(currentWorld[0][0], "1");
}

bool MazeEngine::OnGoalSoccer(int index) {
	if (possession != index)
		return false;
	
	char chara[3];
	if (world[Pos[index][0]][Pos[index][1]][0] == 'g') {
		sprintf(chara, "%i", index);
		if (world[Pos[index][0]][Pos[index][1]][1] == chara[0]) {  // handles up to two agents
			return true;
		}
		else if (strlen(world[Pos[index][0]][Pos[index][1]]) > 2) {
			if (world[Pos[index][0]][Pos[index][1]][2] == chara[0]) {
				return true;
			}
		}
	}
	
	return false;
}


bool MazeEngine::endEpisodeSoccer(int k, int numMovemientos) {
	if (k == numMovemientos)
		return true;

	if (possession == 0) {
		if ((Pos[0][0] == 1) || (Pos[0][0] == 5))
			return true;
	}
	else {
		if ((Pos[1][0] == 1) || (Pos[1][0] == 5))
			return true;
	}

	return false;
}

double MazeEngine::getCurrentRewardSoccer(int j) {
	if (possession == 0) {
		if (Pos[0][0] == 1) {
			if (j == 0)
				return -1.0;
			else
				return 1.0;
		}
		else if (Pos[0][0] == 5) {
			if (j == 0)
				return 1.0;
			else
				return -1.0;
		}
	}
	else {
		if (Pos[1][0] == 1) {
			if (j == 0)
				return -1.0;
			else
				return 1.0;
		}
		else if (Pos[1][0] == 5) {
			if (j == 0)
				return 1.0;
			else
				return -1.0;
		}
	}

	return 0.0;
}

int MazeEngine::convertIt(int accion, bool *validActions, int numActs) {
	int i;
	int count = 0;
	
	for (i = 0; i < numActs; i++) {
		if (validActions[i]) {
			if (count == accion)
				return (i + 1);
			count ++;
		}
	}
	
	return -1;
}

void MazeEngine::deduceActions(int rawActions[2], int geActions[2], bool lastValidActs[2][2*NUMACTIONS]) {
    //printf("acts: (%i, %i)\n", geActions[0], geActions[1]);
    
    for (int j = 0; j < 2; j++) {
        if (geActions[j] == 0) {
            rawActions[j] = 0;
            continue;
        }
        
        int i;
        int c = 0;
        rawActions[j] = -1;
        for (i = 0; i < NUMACTIONS; i++) {
            if (lastValidActs[j][i])
                c++;
            if (i == (geActions[j]-1))
                rawActions[j] = c-1;
        }
        
        if (rawActions[j] < 0) {
            printf("%i: raw action problem from %i; c = %i\n", j, geActions[j], c);
            exit(1);
        }
    }
    
    //printf("reduced: (%i, %i)\n", rawActions[0], rawActions[1]);
}

