#include "PhysicsRule.h"

PhysicsRule::PhysicsRule() {
	Resultados = NULL;
}

PhysicsRule::PhysicsRule(char *tipo, int _nAgents) {
	if (!strncmp(tipo, "pre", 3))
		type = PRE;
	else if (!strncmp(tipo, "post", 4))
		type = POST;
	else if (!strncmp(tipo, "action", 6))
		type = ACTION;
	else if (!strncmp(tipo, "payoff", 6))
		type = PAYOFF;

	nAgents = _nAgents;

	int i;
	Posi = new int*[nAgents];
	acciones = new int[nAgents];
	bacciones = new bool[nAgents];
	bPosi = new bool[nAgents];
	for (i = 0; i < nAgents; i++) {
		Posi[i] = new int[2];
	}

	Resultados = NULL;

	precondition[0] = 't';
}

PhysicsRule::~PhysicsRule() {
	int i;
	if (Resultados != NULL) {
		for (i = 0; i < numResultados; i++) {
			delete Resultados[i];
		}
		delete Resultados;
	}

	for (i = 0; i < nAgents; i++) {
		delete Posi[i];
	}
	delete Posi;
	delete acciones;
	delete bacciones;
	delete bPosi;
}

void PhysicsRule::ReadCell(char *strng) {
	int index = 1, i;

	for (i = 0; i < nAgents; i++) {
		if (strng[index] == '!') {
			bPosi[i] = false;
			index += 2;
		}
		else {
			bPosi[i] = true;
			index ++;
		}

		if (strng[index] == 'i')
			Posi[i][0] = WILD;
		else
			Posi[i][0] = atoi(strng + index);

		while (strng[++index] != ',');
		index ++;

		if (strng[index] == 'i')
			Posi[i][1] = WILD;
		else
			Posi[i][1] = atoi(strng + index);

		if ((i + 1) != nAgents) {
			while (strng[++index] != ',');
			index ++;
		}
	}
}

void PhysicsRule::ReadActionSelection(char *strng) {
	int index = atoi(strng + 1);

	if (strng[3] == 'i')
		acciones[index] = WILD;
	else
		acciones[index] = atoi(strng + 3);
	bacciones[index] = true;
}

void PhysicsRule::initResultados(int _numResultados) {
	numResultados = _numResultados;
	Resultados = new char*[numResultados];
	for (int i = 0; i < numResultados; i++)
		Resultados[i] = new char[100];
}

void PhysicsRule::SetAvailableActions(int a, char *strng) {
	agente = a;

	int i;
	for (i = 0; i < NUMACTIONS; i++) {
		if (strng[i] == '0')
			viable[i] = false;
		else
			viable[i] = true;
	}
}

bool PhysicsRule::CheckConditions(int **Pos, int *actions, char ***mundo) {
	int i, indi = 0, x, y;

	// first check the precondition
	switch (precondition[0]) {
		case 't': break;
		case 'c':
			indi = 2;

			x = atoi(precondition + indi);
			while (precondition[++indi] != ',');
			indi ++;
			y = atoi(precondition + indi);
			while (precondition[++indi] != ')');
			indi += 2;

			if (strcmp(precondition + indi, mundo[x][y]))
				return false;
			break;
		default: printf("don't understand precondition\n"); break;
	}

	for (i = 0; i < nAgents; i++) {
		if (bPosi[i]) {
			if (!Equals(Pos[i][0], Posi[i][0]))
				return false;
			if (!Equals(Pos[i][1], Posi[i][1]))
				return false;
		}
		else {
			if (Equals(Pos[i][0], Posi[i][0]) && Equals(Pos[i][1], Posi[i][1]))
				return false;
		}

		if (!Equals(actions[i], acciones[i]))
			return false;
	}

	return true;
}

bool PhysicsRule::CheckConditions(int **Pos, int index, char ***mundo) {
    //printf("agente vs. index = (%i, %i)\n", agente, index);
	if (agente != index) {
    //    printf("como?");
		return false;
    }

	int i, indi = 0, x, y;

	// first check the precondition
	switch (precondition[0]) {
		case 't': break;
		case 'c':
			indi = 2;

			x = atoi(precondition + indi);
			while (precondition[++indi] != ',');
			indi ++;
			y = atoi(precondition + indi);
			while (precondition[++indi] != ')');
			indi += 2;

			if (strcmp(precondition + indi, mundo[x][y])) {
                //printf("????");
				return false;
            }
			break;
		default: printf("don't understand precondition\n"); break;
	}

	for (i = 0; i < nAgents; i++) {
        //printf("h");
		if (bPosi[i]) {
			if (!Equals(Pos[i][0], Posi[i][0]))
				return false;
			if (!Equals(Pos[i][1], Posi[i][1]))
				return false;
		}
		else {
            //printf("here");
			if (Equals(Pos[i][0], Posi[i][0]) && Equals(Pos[i][1], Posi[i][1]))
				return false;
		}
	}

	return true;
}

bool PhysicsRule::Equals(int v1, int v2) {
	if ((v1 == v2) || (v2 == WILD))
		return true;
	else
		return false;
}