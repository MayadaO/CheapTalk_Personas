#ifndef PHYSICSRULE_H
#define PHYSICSRULE_H

#include "defs.h"

#define PRE			0
#define POST		1
#define ACTION		2
#define PAYOFF		3

#define WILD		-1

class PhysicsRule {
public:
	PhysicsRule();
	PhysicsRule(char *tipo, int _nAgents);
	~PhysicsRule();

	void ReadCell(char *strng);
	void ReadActionSelection(char *strng);
	void initResultados(int _numResultados);
	void SetAvailableActions(int a, char *strng);
	bool CheckConditions(int **Pos, int *actions, char ***mundo);
	bool CheckConditions(int **Pos, int index, char ***mundo);
	bool Equals(int v1, int v2);

	int type;  // PRE, POST, or ACTION
	int nAgents;

	int **Posi;
	int *acciones;
	bool *bacciones;  // booleans to say whether above are inclusive or non-inclusive
	bool *bPosi;  // booleans to say whether above are inclusive or non-inclusive

	double valu;

	int agente;
	bool viable[NUMACTIONS];

	char **Resultados;
	int numResultados;

	char precondition[100];
};


#endif