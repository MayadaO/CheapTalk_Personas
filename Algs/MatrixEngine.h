#ifndef MATRIXENGINE_H
#define MATRIXENGINE_H

#include "defs.h"
#include "State.h"
#include "GameEngine.h"
#include "MarkovGame.h"

class MatrixEngine : public GameEngine {
public:
    MatrixEngine();
	~MatrixEngine();
    
    State *getState(MarkovGame *mg, bool *validActs1, bool *validActs2);
	void MoveAgents();
	void initRound();
    
	bool OnGoal(int index);
	void getValidActions(bool *validActs, int index);
	double getCurrentReward(int index);
    
	int convertIt(int accion, bool *validActions, int numActs);
    void deduceActions(int rawActions[2], int geActions[2], bool lastValidActs[2][2*NUMACTIONS]);
    
    bool moveTime;
    State *s;
};

#endif